#include"cmd.h"
void make_child(pchild pch,int num)
{
	int ret ;
	int i;
	int pid;
	for(i=0;i<num;i++)
	{
		int fds[2];
		ret = socketpair(AF_LOCAL,SOCK_STREAM,0,fds);
		if(-1 == ret )
			err("socketpair")
		pid = fork();
		if(!pid)
		{
			int newfd;
			int flag;
			cmd_msg cmd;
			data buf;
			close(fds[1]);
			while(1)
			{
				recv_fd(fds[0],&newfd);
				while(1)
				{
					printf("wait a comand\n");
					bzero(&cmd,sizeof(cmd));
					printf("cmdno wait\n");
					if(!recv(newfd,&cmd.cmdno,sizeof(int),0))
						break;
					printf("len wait\n");
					recv(newfd,&cmd.len,sizeof(int),0);
					printf("buf wait\n");
					recv(newfd,cmd.buf,cmd.len,0);
					printf("cmd_msg: cmdno=%d,len=%d,buf:%s\n",cmd.cmdno,cmd.len,cmd.buf);
					switch(cmd.cmdno)
						{
							case 1: cmd_cd(newfd,cmd.buf);break;
							case 2: cmd_ls(newfd,cmd.buf);break;
							case 3: cmd_puts(newfd);break;
							case 4: cmd_gets(newfd,cmd.buf);break;
							case 5: cmd_remove(newfd,cmd.buf);break;
							case 6: cmd_pwd(newfd);break;
							default: cmd_default(newfd);break;
						}
					
				}
				flag = 0;
				write(fds[0],&flag,4);
			}
		}
		else
		{
			close(fds[0]);
			pch[i].pid = pid;
			pch[i].sfd = fds[1];
			pch[i].busy = 0;
		}
	}
}
