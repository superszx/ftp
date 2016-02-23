#include"head.h"
void send_file(int newfd,char *name)
{
	int size;
	data buf;
	buf.len = strlen(name);
	strcpy(buf.buf,name);
	printf("begin send file [%s]\n",buf.buf);
	size = send(newfd,&buf.len,4,0);
	if(-1 == size)
		err("send name_len")
	size = send(newfd,buf.buf,buf.len,0);
	if(-1 == size)
		err("send name")
	int fd = open(name,O_RDONLY);
	if(-1 == fd)
		err("open")
	int ret;
	while(bzero(&buf,sizeof(buf)),(buf.len = read(fd,buf.buf,sizeof(buf))) > 0)
	{
		size = send(newfd,&buf.len,4,0);
		if(-1 == size)
			err("send content_len")
		send_n(newfd,&buf);
	}
	bzero(&buf,sizeof(buf));
	size = send(newfd,&buf.len,4,0);
	if(-1 == size)
		err("send end tag")
}
void send_n(int newfd,data *p)
{
	int total = 0;
	int size;
	while(total < p->len)
	{
		size = send(newfd,&p->buf+total,p->len-total,0);
		total += size;
	}
}
