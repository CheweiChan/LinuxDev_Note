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
	int  nread,result;
	fd_set set,temp; 
    struct timeval timeout;
	/* create fifo*/
    unlink(FIFO); //remove FIFO file
	if((mkfifo(FIFO,O_CREAT|O_EXCL)<0)&&(errno!=EEXIST))
		printf("cannot create fifoserver\n");

	
	printf("Preparing for reading bytes...\n");
	
	memset(buf_r,0,sizeof(buf_r));
	
	/* open fifo */
    /*Must setting RW,if setting read only will always return EOF(0),select() will always return have data*/
	fd=open(FIFO,O_RDWR|O_NONBLOCK,0);
    FD_ZERO(&set); /*将set清零使集合中不含任何fd*/ 
    FD_SET(fd, &set); /*将fd加入set集合*/

if(fd==-1)
{
	perror("open");
    exit(1);	
}

while(1)
{
    temp=set;
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;
    result=select(fd+1, &temp, NULL, NULL, &timeout);//blocked until timeout
    //result=select(fd+1, &temp, NULL, NULL, NULL/*NULL will blocked until data*/);
    if(result == 0)
    {
           printf("\ntimeout\n");
    }
    else if(result == -1)
    {
           printf("\nfail\n");
    }
    else
    {
		if((nread=read(fd,buf_r,100))==-1)
		{
			if(errno==EAGAIN)
				printf("no data yet\n");
		}
		printf("read %s from FIFO\n",buf_r);
	    memset(buf_r,0,sizeof(buf_r));
    }
}	
unlink(FIFO); //remove FIFO file
}

