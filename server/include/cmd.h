#ifndef __CMD__
#include "head.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <dirent.h>
#include <string.h>
#include <strings.h>
#define SENDERR(sfd) {\
					bzero(&buf,sizeof(buf));\
					buf.len = 4;\
					*(int *)buf.buf = -1;\
					send(sfd,&buf,strlen(buf.buf)+4,0);\
					perror(" ");\
					return;\
				}
#define SENDREADY(sfd) {\
					bzero(&buf,sizeof(buf));\
					buf.len = 4;\
					*(int *)buf.buf = 1;\
					send(sfd,&buf,strlen(buf.buf)+4,0);\
				}
#define SENDPACK(sfd) send(sfd,&buf,strlen(buf.buf)+4,0);

typedef struct cmd_msg{
	int cmdno;
	int len; 
	char buf[128];
	} cmd_msg ;
void cmd_cd(int sfd,char *pathname);
void cmd_ls(int sfd,char *pathname);
void cmd_puts(int sfd);
void cmd_gets(int sfd,char *filename);
void cmd_pwd(int sfd);
void cmd_remove(int sfd,char *pathname);
void cmd_default();
void recv_n(int sfd,data *p);
void recv_file(int sfd);
void send_n(int newfd,data *p);
void send_file(int newfd,char *filename);
#endif
