# Overview

x86hello is a "Hello world!" type of program running directly on x86 bare metal.

It features protected mode x86 execution and preemptive scheduling of two processes responsible for printing "hello" and "world" respectively.


# Prerequisites

* Netwide Assembler (NASM, https://www.nasm.us)
* SmallerC C compiler (https://github.com/alexfru/SmallerC)
* QEMU and/or VirtualBox


# Building and running

```
$ make
$ make run
```

Here is a sample output:

```
hello
world
hello
world
hello
```


# Other resources

Please see x86.txt file.
