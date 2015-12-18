/*利用多路复用的方式，实现多个客户端和服务器的聊天程序，
要求客户端发给服务器键盘的输入信息，发送给服务端。
	服务端可以通过ID:String的方式向指定的客服端发送信息。*/

#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define STDIN 0
#define MAXDATASIZE 1024

int main(int argc, char *argv[])
{
    char buf[MAXDATASIZE];
    char send_str[MAXDATASIZE];
    int recvbytes;
    int sockfd,k;
    struct hostent *hp;
    struct sockaddr_in addr;
    char *ip;
    int port;
    int ret;
    struct timeval timeout;
    fd_set rfd_set,wfd_set,efd_set;
    fd_set rfd_set2,wfd_set2,efd_set2;

    if(argc < 3)
    {
        printf("client ip port\n");
        return -1; 
    }
    ip = argv[1];
    port = atoi(argv[2]);
    hp = gethostbyname(ip);
    if (hp == NULL)
    {
       perror("hostent error" );
       return -2;
    }

    if((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror( "socket error" );
        return( -1 ); 
    }

    memset(&addr,0,sizeof(struct sockaddr_in));
    addr.sin_family=PF_INET;
    addr.sin_port=htons(port);
    addr.sin_addr.s_addr=inet_addr(ip);
    memcpy(&addr.sin_addr,hp->h_addr_list[0],hp->h_length);
    //addr.sin_addr.s_addr= htonl(INADDR_ANY);
    printf("connecting with %s:%d\n",ip,port);
    if(connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("connect");
        exit(1);
    }
    
    FD_ZERO(&rfd_set2);
    FD_ZERO(&wfd_set2);
    FD_ZERO(&efd_set2);
    FD_SET(0, &rfd_set2);
    FD_SET(sockfd, &rfd_set2);
    FD_SET(sockfd, &efd_set2);

    while(1)
    {
        rfd_set = rfd_set2;
        wfd_set = wfd_set2;
        efd_set = efd_set2;
        timeout.tv_sec = 10;
        timeout.tv_usec = 0;
        
        ret = select(sockfd + 1, &rfd_set, &wfd_set, &efd_set, &timeout);
        if(ret == 0)
          continue;
        if(ret < 0)
        {
            perror("select error: ");
            exit(-1);
        }
        
        if(FD_ISSET(0, &rfd_set))
        {
            fgets(send_str, MAXDATASIZE, stdin);
            send_str[strlen(send_str) - 1] = 0;
            if(strncmp("quit", send_str, 4) == 0)
            {
                close(sockfd);
                exit(0);
            }
            send(sockfd, send_str, strlen(send_str), 0);
        }

        if(FD_ISSET(sockfd, &rfd_set))
        {
            recvbytes = recv(sockfd, buf, MAXDATASIZE, 0);
            if(recvbytes == 0)
            {
                close(sockfd);
                exit(0);
            }
            buf[recvbytes] = 0;
            printf("-----------\nserver said:%s\n", buf);
            fflush(stdout);
        }

        if(FD_ISSET(sockfd, &efd_set))
        {
            close(sockfd);
            exit(0);
        }
    }
}
