import socket
from pwn import *

class Server:
    key = b"\x16\x1c\xa8\xe1\x14\xcd\xed\x99\xf5\xdd\xae\xf6\xeb"
    keylen = len(key)

    buf = ""

    host = "127.0.0.1"
    port = 0
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    def __init__(self, port:int):
        self.port = port

    def Encrypt(self):
        buf_len = len(self.buf)
        buf_list = list(self.buf)

        for i in range(0, buf_len, 1):
            buf_list[i] = p8(self.buf[i] ^ self.key[i % self.keylen])
        self.buf = b''.join(buf_list)

    def Decrypt(self):
        buf_len = len(self.buf)
        buf_list = list(self.buf)

        for i in range(0, buf_len, 1):
            buf_list[i] = p8(self.buf[i] ^ self.key[i % self.keylen])
        self.buf = b''.join(buf_list)

    def Send(self):
        self.Encrypt()
        self.client.sendall(self.buf)

    def Recv(self):
        self.buf = self.client.recv(0x10000)
        self.Decrypt()
        return self.buf

    def Listen(self):
        self.server.bind((self.host, self.port))
        self.server.listen(1)
        print(f"Listening on {self.host}:{self.port}")

    def WaitConnect(self):
        self.client, self.client_adr = self.server.accept()
        print(f"Connect from {self.client_adr}")

        self.buf = input("$ ").encode()
        if self.buf == "\n":
            self.buf = "nothing to do"
        self.Send()
        return self.Recv().decode()

    def Close(self):
        self.server.close()

i = 23333
while(True):
    try:
        i = i + 1
        server = Server(i)
        server.Listen()
        break
    except OSError as e:
        continue

while(True):
    print(server.WaitConnect())
