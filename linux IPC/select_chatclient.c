#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
/*�ͻ��ˣ�ͨ��select��ʵ������I/O�ĸ���
	tcp���������ڴ���ͻ������������ͬʱ���������ӿͻ�ʵ��echo����*/

#include <netinet/in.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>

#define BUFLEN 1024

int main(int argc, char **argv)
{
    int sockfd;
    struct sockaddr_in s_addr;
    socklen_t len;
    unsigned int port;
    char buf[BUFLEN];
    fd_set rfds;
    struct timeval tv;
    int retval, maxfd;    
    
    /*����socket*/
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("socket");
        exit(errno);
    }else
        printf("socket create success!\n");

    /*���÷������˿�*/    
    if(argv[2])
        port = atoi(argv[2]);
    else
        port = 4567;
    /*���÷�����ip*/
    bzero(&s_addr, sizeof(s_addr));
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(port);
    if (inet_aton(argv[1], (struct in_addr *)&s_addr.sin_addr.s_addr) == 0) {
        perror(argv[1]);
        exit(errno);
    }
    /*��ʼ���ӷ�����*/    
    if(connect(sockfd,(struct sockaddr*)&s_addr,sizeof(struct sockaddr)) == -1){
        perror("connect");
        exit(errno);
    }else
        printf("conncet success!\n");
    
    while(1){
        /*�ѿɶ��ļ��������ļ������*/
        FD_ZERO(&rfds);
        /*�ѱ�׼������ļ����������뵽������*/
        FD_SET(0, &rfds);
        maxfd = 0;
        /*�ѵ�ǰ���ӵ��ļ����������뵽������*/
        FD_SET(sockfd, &rfds);
        /*�ҳ��ļ������������������ļ�������*/    
        if(maxfd < sockfd)
            maxfd = sockfd;
        /*���ó�ʱʱ��*/
        tv.tv_sec = 5;
        tv.tv_usec = 0;
        /*�ȴ�����*/
        retval = select(maxfd+1, &rfds, NULL, NULL, &tv);
        if(retval == -1){
            printf("select�����ͻ��˳����˳�\n");
            break;
        }else if(retval == 0){
            printf("�ͻ���û���κ�������Ϣ�����ҷ�����Ҳû����Ϣ������waiting...\n");
            continue;
        }else{
            /*��������������Ϣ*/
            if(FD_ISSET(sockfd,&rfds)){
                /******������Ϣ*******/
                bzero(buf,BUFLEN);
                len = recv(sockfd,buf,BUFLEN,0);
                if(len > 0)
                    printf("��������������Ϣ�ǣ�%s,�����ֽ�����: %d\n",buf,len);
                else{
                    if(len < 0 )
                        printf("������Ϣʧ�ܣ�\n");
                    else
                        printf("�������˳��ˣ�������ֹ��\n");
                break;    
                }
            }
            /*�û�������Ϣ��,��ʼ������Ϣ������*/
            if(FD_ISSET(0, &rfds)){
            _retry:    
                /******������Ϣ*******/    
                bzero(buf,BUFLEN);
                /*fgets�����������ж�ȡBUFLEN-1���ַ�*/
                fgets(buf,BUFLEN,stdin);
                /*��ӡ���͵���Ϣ*/
                //fputs(buf,stdout);
                if(!strncasecmp(buf,"quit",4)){
                    printf("client ������ֹ����!\n");
                    break;
                }
                /*���������ַ���ֻ��"\n"�����س�����ô����������*/
                if(!strncmp(buf,"\n",1)){
                    printf("������ַ�ֻ�лس�������ǲ���ȷ�ģ�����\n");
                    goto _retry;
                }
                /*���buf�к���'\n'����ôҪ��strlen(buf)-1��ȥ��'\n'*/    
                if(strchr(buf,'\n'))
                    len = send(sockfd,buf,strlen(buf)-1,0);
                /*���buf��û��'\n'������buf����������strlen(buf)*/    
                else
                    len = send(sockfd,buf,strlen(buf),0);
                if(len > 0)
                    printf("\t��Ϣ���ͳɹ������ι����͵��ֽ����ǣ�%d\n",len);            
                else{
                    printf("��Ϣ����ʧ��!\n");
                    break;            
                }    
            }
        }
    
    }
    /*�ر�����*/
    close(sockfd);

    return 0;
}