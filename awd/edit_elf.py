from pwn import *
context(
    os = "linux",
    arch = "amd64",
)

call_code = 0x1190
code_start = 0x11BC

code = asm(
f"""
mov rax, 0x0
mov rdi, 0x0
mov rdx, 0x10
syscall
ret
""")

def EditElf(call_code, code_start, code, file):
    call = b'\xE8' + p32(code_start - call_code - 5)

    with open(file, "rb+") as f:
        f.seek(call_code)
        f.write(call)
        f.seek(code_start)
        f.write(code)
