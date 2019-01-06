#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main()
{
	int pipe_fd[2]; //[1]read  [2]write
	pid_t pid;
	char buf_r[100];
	char* p_wbuf;
	int r_num;
	
	memset(buf_r,0,sizeof(buf_r));
	
	/*create pipe*/
	if(pipe(pipe_fd)<0)
	{
		printf("pipe create error\n");
		return -1;
	}
	
	/*create fork*/
	if((pid=fork())==0)  //Child process
	{
		printf("\n");
		close(pipe_fd[1]);
		sleep(2); //wait for parent write data to pipe
		if((r_num=read(pipe_fd[0],buf_r,100))>0)
		{
			printf(   "%d numbers read from the pipe is %s\n",r_num,buf_r);
		}	
		close(pipe_fd[0]);
		exit(0);
  	}
	else if(pid>0)//Parent process
	{
		close(pipe_fd[0]);
		if(write(pipe_fd[1],"Hello",5)!=-1)
			printf("parent write1 Hello!\n");
		if(write(pipe_fd[1]," Pipe",5)!=-1)
			printf("parent write2 Pipe!\n");
		close(pipe_fd[1]);
		sleep(3);
		waitpid(pid,NULL,0); /*wait for child process*/
		exit(0);
	}
	return 0;
}

