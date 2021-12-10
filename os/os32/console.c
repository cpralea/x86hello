#include "os32.h"



void printc(char c, int pos);



void clrscr()
{
    for (int i = 0; i < VGA_TB_COL * VGA_TB_ROW; i++)
        printc(' ', i);
}



void print(char* s)
{
    int i, j;

    // move the entire screen up 1 line
    memcpy(VGA_TB, VGA_TB + (1 * VGA_TB_COL), (VGA_TB_ROW - 1) * (VGA_TB_COL * 2));

    // clear last line
    for (j = 0; j < VGA_TB_COL; j++)
        printc(' ', VGA_TB_COL * (VGA_TB_ROW - 1) + j);

    // print 's' on the last line
    for (i = 0; s[i] != 0 && i < VGA_TB_COL; i++)
        printc(s[i], VGA_TB_COL * (VGA_TB_ROW - 1) + i);
}



void printc(char c, int pos)
{
    VGA_TB[pos] = c + VGA_DEF_ATTRS;
}
