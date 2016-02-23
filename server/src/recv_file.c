#include"head.h"
void recv_file(int sfd)
{	
	data buf;
	bzero(&buf,sizeof(buf));
	recv(sfd,&buf.len,4,0);
	recv(sfd,buf.buf,buf.len,0);//接收文件名
	printf("downloading file[%s]...\n",buf.buf);
	int fd = open(buf.buf,O_WRONLY|O_CREAT,0666);
	if(-1 == fd)
		err("open")
	while(1)
	{
		bzero(&buf,sizeof(buf));
		recv(sfd,&buf.len,4,0);
		if(0 == buf.len)
			break;
		recv_n(sfd,&buf);
		write(fd,buf.buf,buf.len);
	}
	printf("100%% done!\n");
	close(fd);
}
void recv_n(int sfd,data *p)
{
	int total = 0;
	int size;
	while(p->len > total)
	{
		size = recv(sfd,p->buf+total,p->len-total,0);
		total += size;
	}
}
