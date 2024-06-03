/**********************************************************
* file:  client.c
* Author:github.com/rongweihe
* Data:  2019/04/09
***********************************************************/
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<netdb.h>
#include<errno.h>
const int PORT    = 2345;
const int MAXSIZE = 1024;

int main(int argc,const char *argv[]){
    int sockfd;
    struct sockaddr_in server_addr;

    //1. 创建套接字
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        fprintf(stderr, "Socket Error is %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    //2. 客户端发出请求
    if(connect(sockfd, (struct sockaddr *)(&server_addr), sizeof(struct sockaddr)) == -1){
        fprintf(stderr, "Connect failed\n");
        exit(EXIT_FAILURE);
    }

    char sendbuf[MAXSIZE];
    char recvbuf[MAXSIZE];
    while (1){
        //从标准输入中读取数据
        fgets(sendbuf, sizeof(sendbuf), stdin);
        //发送数据
        send(sockfd, sendbuf, strlen(sendbuf), 0);
        if (strcmp(sendbuf, "exit\n") == 0) break;
        //接收数据并显示到终端
        recv(sockfd, recvbuf, sizeof(recvbuf), 0);
        fputs(recvbuf, stdout);
        memset(sendbuf, 0, sizeof(sendbuf));
        memset(recvbuf, 0, sizeof(recvbuf));
    }
    close(sockfd);
    exit(EXIT_SUCCESS);
    return 0;
}
