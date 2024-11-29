from pwn import *
context(
    terminal = ['tmux','splitw','-h'],
    os = "linux",
    arch = "amd64",
    # arch = "i386",
    log_level="debug",
)

call_code = 0x1190
code_start = 0x11BC

call = b'\xE8' + p32(code_start - call_code - 5)

code = asm(
f"""
mov rax, 0x0
mov rdi, 0x0
mov rdx, 0x10
syscall
ret
""")

print(code)
with open("pwn", "rb+") as f:
    f.seek(call_code)
    f.write(call)
    f.seek(code_start)
    f.write(code)