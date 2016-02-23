#include"head.h"

void send_fd(int sfd,int fd)
{
	char buf1[10] = {0};
	char buf2[10] = {0};
	struct iovec iov[2];
	iov[0].iov_base = buf1;
	iov[0].iov_len = 6;
	iov[1].iov_base = buf2;
	iov[1].iov_len = 5;
	
	struct cmsghdr *cmsg;
	int len = CMSG_LEN(sizeof(int));
	cmsg = (struct cmsghdr*)calloc(1,len);
	cmsg->cmsg_len = len;
	cmsg->cmsg_level = SOL_SOCKET;
	cmsg->cmsg_type = SCM_RIGHTS;
	int *fdptr = (int *)CMSG_DATA(cmsg);
	*fdptr = fd;
	 
	struct msghdr msg; 
	bzero(&msg,sizeof(msg));
	msg.msg_iov = iov;
	msg.msg_iovlen = 2;
	msg.msg_control	= cmsg; 
	msg.msg_controllen = len;
	
	int ret = sendmsg(sfd,&msg,0);
	if(-1 == ret)
		err("sendmsg")
}
void recv_fd(int sfd,int *fd)
{
	char buf1[10] = {0};
	char buf2[10] = {0};
	struct iovec iov[2];
	iov[0].iov_base = buf1;
	iov[0].iov_len = 10;
	iov[1].iov_base = buf2;
	iov[1].iov_len = 10;
	
	struct cmsghdr *cmsg;
	int len = CMSG_LEN(sizeof(int));
	cmsg = (struct cmsghdr*)calloc(1,len);
	cmsg->cmsg_len = len;
	cmsg->cmsg_level = SOL_SOCKET;
	cmsg->cmsg_type = SCM_RIGHTS;
	 
	struct msghdr msg; 
	bzero(&msg,sizeof(msg));
	msg.msg_iov = iov;
	msg.msg_iovlen = 2;
	msg.msg_control	= cmsg; 
	msg.msg_controllen = len;
	
	int ret = recvmsg(sfd,&msg,0);
	if(-1 == ret)
		err("sendmsg")

	*fd = *(int *)CMSG_DATA(cmsg);
}

