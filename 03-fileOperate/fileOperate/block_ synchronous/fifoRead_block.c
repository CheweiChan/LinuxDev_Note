#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define FIFO "./myfifo"
void main(int argc,char** argv)
{
	char buf_r[100];
	int  fd;
	int  nread;
	
	/* create fifo*/
	if((mkfifo(FIFO,O_CREAT|O_EXCL)<0)&&(errno!=EEXIST))
		printf("cannot create fifoserver\n");
	
	printf("Preparing for reading bytes...\n");
	
	memset(buf_r,0,sizeof(buf_r));
	
	/* open fifo */
	fd=open(FIFO,O_RDONLY,0);
	if(fd==-1)
	{
		perror("open");
		exit(1);	
	}
	while(1)
	{
		memset(buf_r,0,sizeof(buf_r));
		
		if((nread=read(fd,buf_r,100))==-1)
		{
			if(errno==EAGAIN)
				printf("no data yet\n");
		}
		printf("read %s from FIFO\n",buf_r);
		sleep(1);
	}	
	unlink(FIFO); //remove FIFO file
}

