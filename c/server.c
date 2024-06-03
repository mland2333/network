/**********************************************************
* file:  server.c
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

int main(int argc, char *argv[]){
    int sockfd, newsockfd;

    //1. 定义服务端套接字数据结构,客户端套接口数据结构
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;

    //2. 创建套接字
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        fprintf(stderr, "create socket failed\n");
        exit(EXIT_FAILURE);
    }
    printf("create socket success and sockfd is %d\n",sockfd);

    //清空表示地址的结构体变量,设置addr的成员变量信息,设置 ip 为本机 IP
    bzero(&server_addr,  sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    //3. 绑定套接字
    if (bind(sockfd, (struct sockaddr*)(&server_addr), sizeof(struct sockaddr)) < 0){
        fprintf(stderr, "bind failed \n");
        exit(EXIT_FAILURE);
    }
    printf("bind success...\n");

    //4. 监听套接字
    if (listen(sockfd, 10) < 0){
        perror("listen fail\n");
        exit(EXIT_FAILURE);
    }
    printf("listen success...\n");

    int sin_size = sizeof(struct sockaddr_in);
    printf("sin_size is %d\n", sin_size);
    //5. 三次握手完成后，服务器调用 accept() 接受连接
    if ((newsockfd = accept(sockfd, (struct sockaddr *)(&client_addr), &sin_size)) < 0){
        perror("accept error");
        exit(EXIT_FAILURE);
    }
    printf("accepted a new connetction...\n");
    printf("new socket id is %d\n", newsockfd);
    printf("Accept clent ip is %s\n", inet_ntoa(client_addr.sin_addr));
    printf("Connect successful please input message:");

    //发送，接收数据缓冲区
    char sendbuf[1024];
    char recvbuf[1024];
    while(1){
        int len = recv(newsockfd, recvbuf, sizeof(recvbuf), 0);
        if (strcmp(recvbuf, "exit\n") == 0) break;
        fputs(recvbuf, stdout);
        send(newsockfd, recvbuf, len, 0);
        memset(sendbuf, 0,sizeof(sendbuf));
        memset(recvbuf, 0, sizeof(recvbuf));
    }
    close(newsockfd);
    close(sockfd);
    puts("exit success");
    exit(EXIT_SUCCESS);
    return 0;
}
