#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h> 
#include <sys/time.h>
#define FIFO "./myfifo"

void main(int argc,char** argv)
{
	char buf_r[100];
	int  fd,a;
	int  nread;
	fd_set set; 
	struct timeval timeout;
    FD_ZERO(&set); /*将set清零使集合中不含任何fd*/ 
     // FD_SET(fd, &set); /*将fd加入set集合*/
     // FD_CLR(fd, &set); /*将fd从set集合中清�?/ 
     // FD_ISSET(fd, &set); /*测试fd是否在set集合�?/
	while(1)
	{
        timeout.tv_sec = 2;
        timeout.tv_usec = 0;
        memset(buf_r,0,sizeof(buf_r));
    	if((mkfifo(FIFO,0666)<0)&&(errno!=EEXIST))
    	printf("cannot create fifoserver\n");
    	fd=open(FIFO,O_RDWR|O_NONBLOCK,0);
    	if(fd==-1)
    	{
    		perror("open");
    		exit(1);	
    	}

	    printf("Preparing for reading bytes...\n");
		FD_SET(fd, &set);
                a=select(fd+1, &set, 0, 0, NULL);
	
    	if(FD_ISSET(fd, &set))
    	{
    	printf("\n isset\n");
    	}
    	else printf("\n noset\n");

    	if(a==0)
    	{
    	printf("\n timeout\n");
    	}
    	else if(a==-1)
    	{
    	printf("\n fail\n");
    	}
        else
        {
            printf("\ntrue\n");
    	    if((nread=read(fd,buf_r,100))==-1)
    	    {
    		    if(errno==EAGAIN)
    		    printf("no data yet\n");
    	    }
    	printf("read %s from FIFO,%d,%d\n",buf_r,a,fd);
        unlink(FIFO);
        }      
        FD_CLR(fd, &set); 
    	sleep(1);
	}	
	pause(); 
	unlink(FIFO);
}

