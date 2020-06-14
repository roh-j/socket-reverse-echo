#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30

void error_handling(char *message);
void reverse_message(char *reverse, char *message);

int main(int argc, char *argv[])
{
    int serv_sock;
    int clnt_sock;
    int nSockOpt = 1;

    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;

    int str_len;
    char message[BUF_SIZE];
    char reverse[BUF_SIZE];

    if (argc != 2)
    {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);

    if (serv_sock == -1)
        error_handling("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &nSockOpt, sizeof(nSockOpt));

    if (bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error");

    if(listen(serv_sock, 5) == -1)
        error_handling("listen() error");

    clnt_addr_size = sizeof(clnt_addr);
    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);

    if (clnt_sock == -1)
        error_handling("accept() error");

    str_len = read(clnt_sock, message, BUF_SIZE-1);

    reverse_message(reverse, message);
    printf("Reverse Message : %s \n", reverse);
    write(clnt_sock, reverse, BUF_SIZE);

    close(clnt_sock);
    close(serv_sock);
    return 0;
}

void reverse_message(char *reverse, char *message)
{
    /*
     * 문자열 reverse 함수
     * call by reference
     */
    int reverse_idx = 0;

    for (int i = strlen(message)-2; i >= 0; i--)
    {
        reverse[reverse_idx] = message[i];
        reverse_idx++;
    }
    reverse[BUF_SIZE] = '\0';
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
