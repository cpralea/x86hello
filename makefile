.PHONY: all run debug images mkbdirs clean


all: mkbdirs images
run: all
	qemu-system-i386 -machine q35 -m 4M ${x86H_QEMU_NCURSES} ${x86H_QEMU_MONITOR} -drive file=hello.img,format=raw,index=0,if=ide,media=disk
debug: all
	qemu-system-i386 -machine q35 -m 4M ${x86H_QEMU_NCURSES} ${x86H_QEMU_MONITOR} -drive file=hello.img,format=raw,index=0,if=ide,media=disk -s -S
images: hello.img hello.vdi
mkbdirs:
	@if [ ! -d .build ]; then \
		mkdir -p .build; \
		mkdir -p .build/os; \
		mkdir -p .build/os/os16init; \
		mkdir -p .build/os/os32; \
		mkdir -p .build/proc; \
	fi
clean:
	rm -rf .build
	rm -f hello.img
	rm -f hello.vdi


.build/boot.bin: boot.asm
	nasm -f bin $^ -o $@


OS16_OBJS :=	.build/os/os16init/os16.o \
				.build/os/os16init/gdt.o \
				.build/os/os16init/pm.o \
				.build/os/os16init/util.o
.build/os/os16init/%.o: os/os16init/%.asm
	nasm -f elf $^ -o $@
.build/os/os16init/%.o: os/os16init/%.c os/os.h os/os16init/os16.h
	smlrcc -Wall -flat16 -c $< -o $@
.build/os/os16init/os16.bin: ${OS16_OBJS}
	smlrcc -Wall -flat16 -origin 0x0500 -entry _init $^ -o $@


OS32_OBJS :=	.build/os/os32/os32.o \
				.build/os/os32/console.o \
				.build/os/os32/idt.o \
				.build/os/os32/idt-asm.o \
				.build/os/os32/int.o \
				.build/os/os32/pic.o \
				.build/os/os32/sched.o \
				.build/os/os32/proc.o \
				.build/os/os32/syscalls.o \
				.build/os/os32/task.o \
				.build/os/os32/util.o \
				.build/os/os32/util-asm.o
.build/os/os32/%.o: os/os32/%.asm
	nasm -f elf32 $^ -o $@
.build/os/os32/%.o: os/os32/%.c os/os.h os/os32/os32.h
	smlrcc -Wall -flat32 -c $< -o $@
.build/os/os32/os32.bin: ${OS32_OBJS}
	smlrcc -Wall -flat32 -origin 0x1000 -entry _run $^ -o $@


PROC1_OBJS :=	.build/proc/proc1.o \
				.build/proc/rt.o
PROC2_OBJS :=	.build/proc/proc2.o \
				.build/proc/rt.o
.build/proc/%.o: proc/%.c proc/rt.h
	smlrcc -Wall -flat32 -c $< -o $@
.build/proc/proc1.bin: ${PROC1_OBJS}
	smlrcc -Wall -flat32 -origin 0x0 -entry _main $^ -o $@
.build/proc/proc2.bin: ${PROC2_OBJS}
	smlrcc -Wall -flat32 -origin 0x0 -entry _main $^ -o $@


.build/proc/proc.img: .build/proc/proc1.bin .build/proc/proc2.bin
	dd if=/dev/zero of=.build/proc/proc.img bs=512 count=16 > /dev/null 2>&1
	dd conv=notrunc if=.build/proc/proc1.bin of=.build/proc/proc.img bs=512 count=8 seek=0 > /dev/null 2>&1
	dd conv=notrunc if=.build/proc/proc2.bin of=.build/proc/proc.img bs=512 count=8 seek=8 > /dev/null 2>&1

hello.img: .build/boot.bin .build/os/os16init/os16.bin .build/os/os32/os32.bin .build/proc/proc.img
	dd if=/dev/zero of=hello.img bs=512 count=2880 > /dev/null 2>&1
	dd conv=notrunc if=.build/boot.bin of=hello.img bs=512 count=1 seek=0 > /dev/null 2>&1
	dd conv=notrunc if=.build/os/os16init/os16.bin of=hello.img bs=512 count=2 seek=1 > /dev/null 2>&1
	dd conv=notrunc if=.build/os/os32/os32.bin of=hello.img bs=512 count=32 seek=3 > /dev/null 2>&1
	dd conv=notrunc if=.build/proc/proc.img of=hello.img bs=512 count=16 seek=35 > /dev/null 2>&1

hello.vdi: hello.img
	rm -f hello.vdi
	VBoxManage convertfromraw hello.img hello.vdi --format vdi --uuid ${VBOX_IMG_UUID}
