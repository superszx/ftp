#include"cmd.h"

void cmd_cd(int sfd,char *pathname)
{
	printf("pathname:%s\n",pathname);
	data buf;
	bzero(&buf,sizeof(buf));
	int ret;
	ret = chdir(pathname);
	if(ret == -1)
	{	
		SENDERR(sfd)
		perror("chdir");
	}
	else
	{
		getcwd(buf.buf,sizeof(buf.buf));
		buf.len = strlen(buf.buf);
		SENDPACK(sfd)
		printf("curdir: %s\n",buf.buf);
	}
}

void cmd_ls(int sfd,char *pathname)
{
	data buf;
	bzero(&buf,sizeof(buf));
	DIR *df;
	df = opendir(pathname);
	if(NULL == df)
		SENDERR(sfd)
	struct dirent *dr; 
	while(1)
	{
		dr = readdir(df);
		if(dr == NULL)
			break;
		strcat(buf.buf,dr->d_name);
		strcat(buf.buf,"\n");	 
	}
	closedir(df);
	buf.len = strlen(buf.buf);
	send(sfd,&buf,buf.len+sizeof(int)-1,0);
}

void cmd_puts(int sfd)
{
	data buf;
	SENDREADY(sfd)
	recv_file(sfd);
}

void cmd_gets(int sfd,char *filename)
{
	data buf;
	DIR *df;
	df = opendir("./");
	if(NULL == df)
		SENDERR(sfd)
	struct dirent *dr; 
	while(1)
	{
		dr = readdir(df);
		printf("%s\n",dr->d_name);
		if(dr == NULL)
			SENDERR(sfd)
		if(!strcmp(filename,dr->d_name))
		{
			SENDREADY(sfd)
			break;
		}
	}
	closedir(df);
	bzero(&buf,sizeof(buf));
	recv(sfd,&buf.len,4,0);
	recv(sfd,buf.buf,buf.len,0);
	if(buf.len == 4 && *(int *)buf.buf == 1)
		 send_file(sfd,filename);
}

void cmd_remove(int sfd,char *pathname)
{
	data buf;
	int ret = unlink(pathname);
	if(-1 == ret)
		SENDERR(sfd)
	else
		SENDREADY(sfd)
}

void cmd_pwd(int sfd)
{
	data buf;
	bzero(&buf,sizeof(buf));
	getcwd(buf.buf,sizeof(buf.buf));
	buf.len = strlen(buf.buf);
	SENDPACK(sfd)
}
void cmd_default(int sfd)
{
	data buf;
	SENDERR(sfd)
}
