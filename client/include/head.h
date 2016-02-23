#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<strings.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/epoll.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/stat.h>
#include<fcntl.h>
#include <dirent.h>
#define err(msg) {perror(msg);exit(-1);}
					
#define SENDERR(sfd) {\
						buf.len = 4;\
						*(int *)buf.buf = -1;\
						send(sfd,&buf,strlen(buf.buf)+4,0);\
						return;\
					}
#define SENDREADY(sfd) {\
							bzero(&buf,sizeof(buf));\
							buf.len = 4;\
							*(int *)buf.buf = 1;\
							send(sfd,&buf,strlen(buf.buf)+4,0);\
						}
#define SENDPACK(sfd) send(sfd,&buf,strlen(buf.buf)+4,0);

typedef struct data_pro{
	int len;
	char buf[1000];
	} data;

typedef struct cmd_msg{
	int cmdno;
	int len;
	char buf[128];
	} cmd_msg;

void init(char *ip,char *port);
void recv_n(int sfd,data *p);
//循环接收数据块
void recv_file(int sfd);
//通过socket描述符sfd接收一个文件，与客户端的send_file配合使用
void send_n(int newfd,data *p);
void send_file(int newfd,char *filename);
char *getpath(char *p);
