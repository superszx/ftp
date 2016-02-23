#ifndef __HEAD__
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
#include<signal.h>
#define FILENAME "file"
#define err(msg) {perror(msg);exit(-1);}
typedef struct child_pro{
	pid_t pid;
	int sfd;
	int busy;
	} child,*pchild;

typedef struct data_buf{
	int len;
	char buf[1000];
	} data;

void init(char *ip,char *port,int *pnum);
void send_fd(int sfd,int fd);
void recv_fd(int sfd,int *fd);
void recv_n(int sfd,data *p);
void recv_file(int sfd);
void send_n(int newfd,data *p);
void send_file(int newfd,char *filename);
void make_child(pchild pch,int num);
#endif
