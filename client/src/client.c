#include"head.h"

int main(int argc,char *argv[])
{
	char ip[16];
	char port[8]; 
	init(ip,port);
	int sfd = socket(AF_INET,SOCK_STREAM,0);
	if(-1 == sfd)
		err("socket")
	struct sockaddr_in server;
	bzero(&server,sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(port));
	server.sin_addr.s_addr = inet_addr(ip);
	int ret;
	ret = connect(sfd,(struct sockaddr*)&server,sizeof(server));
	if(-1 == ret)
		err("connect")

	char p[128];
	char name[128];
	cmd_msg cmd;
	data buf;
	int i;
	while(1)
	{
		bzero(&cmd,sizeof(cmd));
		bzero(p,sizeof(p));
		bzero(name,sizeof(name));
		printf("enter a conmand:\n");
		read(STDIN_FILENO,p,sizeof(p));
		i = 0;
		if(!memcmp(p,"cd",2))
		{
			cmd.cmdno = 1;
			strcpy(cmd.buf,p+strlen("cd")+1);
			cmd.len = strlen(cmd.buf)-1;
			send(sfd,&cmd,cmd.len+2*sizeof(int),0);
			bzero(&buf,sizeof(buf));
			recv(sfd,&buf.len,4,0);
			recv(sfd,buf.buf,buf.len,0);
			if(buf.len != 4 || *(int *)buf.buf != -1)	
				printf("%s\n",buf.buf);
			else
				printf("error!\n");
		}
		if(!memcmp(p,"ls",2))
		{
			cmd.cmdno = 2;
			strcpy(cmd.buf,p+strlen("ls")+1);
			cmd.len = strlen(cmd.buf)-1;
			send(sfd,&cmd,cmd.len+2*sizeof(int),0);
			bzero(&buf,sizeof(buf));
			recv(sfd,&buf.len,4,0);
			recv(sfd,buf.buf,buf.len,0);
			if(buf.len != 4 || *(int *)buf.buf != -1)	
				printf("%s\n",buf.buf);
			else
				printf("error!\n");
		}
		if(!memcmp(p,"puts",4))
		{
			cmd.cmdno = 3;
			strcpy(cmd.buf,p+strlen("puts")+1);
			cmd.len = strlen(cmd.buf)-1;
			memcpy(&name,cmd.buf,cmd.len);
			send(sfd,&cmd,cmd.len+2*sizeof(int),0);
			bzero(&buf,sizeof(buf));
			recv(sfd,&buf.len,4,0);
			recv(sfd,buf.buf,buf.len,0);	
			if(buf.len == 4 && *(int *)buf.buf == 1)	
				send_file(sfd,name);
			else
				printf("error!\n");
		}		
		if(!memcmp(p,"gets",4))
		{
			cmd.cmdno = 4;
			strcpy(cmd.buf,p+strlen("gets")+1);
			cmd.len = strlen(cmd.buf)-1;
			memcpy(&name,cmd.buf,cmd.len);
			send(sfd,&cmd,cmd.len+2*sizeof(int),0);
			bzero(&buf,sizeof(buf));
			recv(sfd,&buf.len,4,0);
			recv(sfd,buf.buf,buf.len,0);	
			if(buf.len == 4 && *(int *)buf.buf == 1)	
			{	
				SENDREADY(sfd);
				recv_file(sfd);
			}
			else
				printf("error!\n");
		}		
		if(!memcmp(p,"remove",strlen("remove")))
		{
			cmd.cmdno = 5;
			strcpy(cmd.buf,p+strlen("remove")+1);
			cmd.len = strlen(cmd.buf)-1;
			send(sfd,&cmd,cmd.len+2*sizeof(int),0);
			bzero(&buf,sizeof(buf));
			recv(sfd,&buf.len,4,0);
			recv(sfd,buf.buf,buf.len,0);
			if(buf.len == 4 && *(int *)buf.buf == 1)	
				printf("remove success!\n");
			else if(buf.len == 4 && *(int *)buf.buf == -1)
				printf("error!\n");
		}
		if(!memcmp(p,"pwd",3))
		{
			cmd.cmdno = 6;
			cmd.len = 4;
			send(sfd,&cmd,cmd.len+2*sizeof(int),0);
			bzero(&buf,sizeof(buf));
			recv(sfd,&buf.len,4,0);
			recv(sfd,buf.buf,buf.len,0);
			if(buf.len != 4 || *(int *)buf.buf != -1)	
				printf("%s\n",buf.buf);
			else
				printf("error!\n");
		}


		

		
	}
	exit(0);
}
void init(char *ip,char *port)
{
	FILE *pf = fopen("config","rb");
	if(NULL == pf)
		err("open config")
	int ret ;
	ret = fscanf(pf,"%s",ip);
	if(-1 == ret)
		err("ip init")
	printf("ip: %s\n",ip);
	ret = fscanf(pf,"%s",port);
	if(-1 == ret)
		err("port init")
	printf("port: %s\n",port);
}
//char *getpath(char *p)
//{
//	while(*p != ' ')
//		p++;
//	p++;
//	return p;
//
//}
