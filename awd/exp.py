from pwn import *
context(
    terminal = ['tmux','splitw','-h'],
    os = "linux",
    arch = "amd64",
    # arch = "i386",
    log_level="debug",
)
def debug(io):
    gdb.attach(io,
'''
'''
)

if sys.argv[1]:
    io = remote(sys.argv[1], sys.argv[2])
else:
    io = process("./pwn")
    debug(io)

io.interactive()

io.sendline("cat flag")
flag = b'flag{' + io.recvuntil("}")
with open('./flags', 'ab') as f:
    f.write(flag + b'\n')
