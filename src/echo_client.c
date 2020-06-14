#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30

void error_handling(char *message);

int main(int argc, char *argv[])
{
    int sock;
    int nSockOpt = 1;

    struct sockaddr_in serv_addr;

    int str_len;
    char message[BUF_SIZE];
    char send[BUF_SIZE];

    if (argc != 3)
    {
        printf("Usage : %s <IP> <port> \n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);

    if (sock == -1)
        error_handling("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &nSockOpt, sizeof(nSockOpt));

    fputs("메세지를 입력하세요. \n", stdout);
    fgets(send, BUF_SIZE, stdin);

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("connect() error!");

    write(sock, send, sizeof(send));
    str_len = read(sock, message, BUF_SIZE-1);

    printf("Message form server : %s \n", message);
    printf("Funcion read call count %d \n", str_len);
    close(sock);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
