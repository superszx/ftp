#include"head.h"

int main()
{
	char ip[16]={0};
	char port[8]={0};
	int num;
	init(ip,port,&num);
	int sfd = socket(AF_INET,SOCK_STREAM,0);
	if(-1 == sfd)
		err("socket")
	struct sockaddr_in server;
	bzero(&server,sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(port));
	server.sin_addr.s_addr = inet_addr(ip);
	int ret = bind(sfd,(struct sockaddr*)&server,sizeof(server));
	if(ret == -1)
		err("bind")
	ret = listen(sfd,num+1);
	
	pchild pch;
	pch = (pchild)calloc(num,sizeof(child));
	make_child(pch,num);
	
	int epfd = epoll_create(1);
	if(-1 == epfd)
		err("epoll_create")
	struct epoll_event ev,*evs;
	evs = (struct epoll_event *)calloc(num+1,sizeof(struct epoll_event));

	ev.events = EPOLLIN;
	ev.data.fd = sfd;
	ret = epoll_ctl(epfd,EPOLL_CTL_ADD,sfd,&ev);
	if(-1 == ret)
		err("epoll_ctl 1")
	int i;
	for(i=0;i<num;i++)
	{
		ev.data.fd = pch[i].sfd;
		ret = epoll_ctl(epfd,EPOLL_CTL_ADD,pch[i].sfd,&ev);
		if(-1 == ret)
			err("epoll_ctl 2")
	}
	struct sockaddr_in client;
	socklen_t clen = sizeof(client);
	bzero(&client,clen);
	int newfd;
	int j;
	int size;
	int flag;
	while(1)
	{
		bzero(evs,sizeof(child)*num);
		ret = epoll_wait(epfd,evs,num+1,-1);
		if(ret>0)
		{
			for(i=0;i<ret;i++)
			{
			if(evs[i].data.fd == sfd && evs[i].events == EPOLLIN)
				{
					newfd = accept(sfd,(struct sockaddr*)&client,&clen);
					if(-1 == newfd)
						err("accept")
					printf("A client is connect,newfd = %d\n",newfd);
					for(j=0;j<num;j++)
					{
						if(pch[j].busy == 0)
							break;
					}
					if(num != j)
					{
						send_fd(pch[j].sfd,newfd);
						pch[j].busy = 1;
						printf("child process %d is serve now!\n",pch[j].pid);
					}
					else
						printf("no child process is free now\n");
					
				}
				for(j=0;j<num;j++)
				{
					flag = 1;
					if(evs[i].data.fd==pch[j].sfd && evs[i].events==EPOLLIN)
					{
						size = read(pch[j].sfd,&flag,4);
						if(-1 == size)
							err("read")
						if(flag == 0)
						{
							pch[j].busy = 0;
							printf("process %d free again now\n",pch[j].pid);
						}
					}
				}
			}
		}

	}

}

void init(char *ip,char *port,int *pnum)
{
	FILE *pf = fopen("config","rb");
	if(NULL == pf)
		err("open config")
	int ret ;
	ret = fscanf(pf,"%s",ip);
	printf("ip: %s\n",ip);
	ret = fscanf(pf,"%s",port);
	printf("port: %s\n",port);
	ret = fscanf(pf,"%d",pnum);	
	printf("num=%d\n",*pnum);
}
