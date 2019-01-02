#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <limits.h>
#include <poll.h>
#include <signal.h>
#include <arpa/inet.h>

#define SA struct sockaddr
#define MYPORT 12345 //连接时使用的端口
#define MAXCLINE 5   //连接队列中的个数
#define BUF_SIZE 200

struct pollfd client[MAXCLINE + 1];
int conn_amount; //当前的连接数

void showclient()
{
    int i;
    printf("client amount:%d\n", conn_amount);
    for (i = 1; i <= MAXCLINE; i++)
    {
        printf("[%d]:%d ", i, client[i].fd);
    }
    printf("\n\n");
}

int main()
{
    int listenfd, connfd, sockfd, i, maxi;
    int ret;
    socklen_t clilen;
    ssize_t n;
    int yes = 1;
    char buf[BUF_SIZE];
    struct sockaddr_in servaddr, cliaddr;
    //创建监听套接字
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("socket() error!");
        exit(0);
    }
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
    {
        perror("setsockopt error \n");
        exit(1);
    }

    //先要对协议地址进行清零
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(MYPORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    //绑定套接口到本地协议地址
    if (bind(listenfd, (SA *)&servaddr, sizeof(servaddr)) < 0)
    {
        printf("bind() error!");
        exit(0);
    }
    //服务器开始监听
    if (listen(listenfd, MAXCLINE) < 0)
    {
        printf("listen() error!");
        exit(0);
    }
    client[0].fd = listenfd;
    client[0].events = POLLRDNORM; //关心监听套机字的读事件
    for (i = 1; i <= MAXCLINE; ++i)
    {
        client[i].fd = -1;
    }
    for (;;)
    {
        ret = poll(client, MAXCLINE + 1, 30 * 1000);
        if (ret < 0) //没有找到有效的连接 失败
        {
            perror("select error!\n");
            break;
        }
        else if (ret == 0) // 指定的时间到，
        {
            printf("timeout \n");
            continue;
        }
        if (client[0].revents & POLLRDNORM)
        {
            clilen = sizeof(cliaddr);
            //accept 的后面两个参数都是值-结果参数，他们的保留的远程连接电脑的信息，如果不管新远程连接电脑的信息，可以将这两个参数设置为 NULL
            connfd = accept(listenfd, (SA *)&cliaddr, &clilen);
            if (connfd < 0)
            {
                continue;
            }
            for (i = 1; i <= MAXCLINE; i++)
            {
                if (client[i].fd < 0)
                {
                    client[i].fd = connfd;
                    break;
                }
            }
            conn_amount++;
            printf("new connection client[%d]%s:%d\n", i, inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));
            if (i > MAXCLINE)
            {
                printf("too many clients");
                exit(0);
            }
            client[i].events = POLLRDNORM;
            if (i > conn_amount)
            {
                conn_amount = i;
            }
            if (--ret <= 0)
            {
                showclient();
                continue;
            }
        }
        for (i = 1; i < MAXCLINE; ++i)
        {
            if ((sockfd = client[i].fd) < 0)
            {
                continue;
            }
            if (client[i].revents & POLLRDNORM)
            {
                if ((n = read(sockfd, buf, BUF_SIZE)) < 0)
                {
                    if (errno == ECONNRESET)
                    {
                        close(sockfd);
                        client[i].fd = -1;
                    }
                    else
                    {
                        printf("read error!\n");
                    }
                }
                else if (n == 0)
                {
                    close(sockfd);
                    client[i].fd = -1;
                    conn_amount--;
                }
                else
                {
                    printf("client[%d] send:%s\n", i, buf);
                    //write(sockfd, buf, n);
                }
                if (--ret <= 0)
                    break;
            }
        }
        showclient();
    }
}