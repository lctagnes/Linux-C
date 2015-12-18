/*��������ͨ��select��ʵ������I/O�ĸ���
	tcp���������ڴ���ͻ������������ͬʱ���������ӿͻ�ʵ��echo����*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>

#define BUFLEN 1024

int main(int argc, char **argv)
{
    int sockfd, newfd;
    struct sockaddr_in s_addr, c_addr;
    char buf[BUFLEN];
    socklen_t len;
    unsigned int port, listnum;
    fd_set rfds;
    struct timeval tv;    
    int retval,maxfd;
    
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
    /*�����������г���*/
    if(argv[3])
        listnum = atoi(argv[3]);
    else
        listnum = 3;
    /*���÷�����ip*/
    bzero(&s_addr, sizeof(s_addr));
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(port);
    if(argv[1])
        s_addr.sin_addr.s_addr = inet_addr(argv[1]);
    else
        s_addr.sin_addr.s_addr = INADDR_ANY;
    /*�ѵ�ַ�Ͷ˿ڰﶨ���׽�����*/
    if((bind(sockfd, (struct sockaddr*) &s_addr,sizeof(struct sockaddr))) == -1){
        perror("bind");
        exit(errno);
    }else
        printf("bind success!\n");
    /*�������ض˿�*/
    if(listen(sockfd,listnum) == -1){
        perror("listen");
        exit(errno);    
    }else
        printf("the server is listening!\n");
    while(1){
        printf("*****************���쿪ʼ***************\n");
        len = sizeof(struct sockaddr);
        if((newfd = accept(sockfd,(struct sockaddr*) &c_addr, &len)) == -1){
            perror("accept");        
            exit(errno);
        }else
            printf("������������Ŀͻ����ǣ�%s: %d\n",inet_ntoa(c_addr.sin_addr),ntohs(c_addr.sin_port));
        while(1){
            /*�ѿɶ��ļ��������ļ������*/
            FD_ZERO(&rfds);
            /*�ѱ�׼������ļ����������뵽������*/
            FD_SET(0, &rfds);
            maxfd = 0;
            /*�ѵ�ǰ���ӵ��ļ����������뵽������*/
            FD_SET(newfd, &rfds);
            /*�ҳ��ļ������������������ļ�������*/    
            if(maxfd < newfd)
                maxfd = newfd;
            /*���ó�ʱʱ��*/
            tv.tv_sec = 5;
            tv.tv_usec = 0;
            /*�ȴ�����*/
            retval = select(maxfd+1, &rfds, NULL, NULL, &tv);
            if(retval == -1){
                printf("select������ÿͻ������ӵĳ����˳�\n");
                break;
            }else if(retval == 0){
                printf("������û���κ�������Ϣ�����ҿͻ���Ҳû����Ϣ������waiting...\n");
                continue;
            }else{
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
                        printf("server ������ֹ����!\n");
                        break;
                    }
                    /*���������ַ���ֻ��"\n"�����س�����ô����������*/
                    if(!strncmp(buf,"\n",1)){
                        printf("������ַ�ֻ�лس�������ǲ���ȷ�ģ�����\n");
                        goto _retry;
                    }    
                    /*���buf�к���'\n'����ôҪ��strlen(buf)-1��ȥ��'\n'*/            
                    if(strchr(buf,'\n'))
                        len = send(newfd,buf,strlen(buf)-1,0);
                    /*���buf��û��'\n'������buf����������strlen(buf)*/    
                    else
                        len = send(newfd,buf,strlen(buf),0);
                    if(len > 0)
                        printf("\t��Ϣ���ͳɹ������ι����͵��ֽ����ǣ�%d\n",len);            
                    else{
                        printf("��Ϣ����ʧ��!\n");
                        break;            
                    }
                }
                /*�ͻ��˷�������Ϣ*/
                if(FD_ISSET(newfd, &rfds)){
                    /******������Ϣ*******/
                    bzero(buf,BUFLEN);
                    len = recv(newfd,buf,BUFLEN,0);
                    if(len > 0)
                        printf("�ͻ��˷�������Ϣ�ǣ�%s,�����ֽ�����: %d\n",buf,len);
                    else{
                        if(len < 0 )
                            printf("������Ϣʧ�ܣ�\n");
                        else
                            printf("�ͻ����˳��ˣ�������ֹ��\n");
                        break;    
                    }
                }
            }
        }
        /*�ر�������׽���*/
        close(newfd);
        /*�Ƿ��˳�������*/
        printf("�������Ƿ��˳�����y->�ǣ�n->��? ");
        bzero(buf, BUFLEN);
        fgets(buf,BUFLEN, stdin);
        if(!strncasecmp(buf,"y",1)){
            printf("server �˳�!\n");
            break;
        }
    }
    /*�رշ��������׽���*/
    close(sockfd);
    return 0;
}