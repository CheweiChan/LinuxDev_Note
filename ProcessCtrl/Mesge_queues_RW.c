#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
char buf_r[0xff];
char buf_w[0xff];
pid_t pid;

int main() 
{

int msg_id = msgget(777, IPC_CREAT | 0666);

if (msg_id != -1) 
{
   if((pid=fork())>0) //parent process
   {
       waitpid(pid,NULL,0);//wait for child write data to mesge queues
       if (msgrcv(msg_id, buf_r, sizeof(buf_r), 0, IPC_NOWAIT) != -1) //read from queues
       { 
           printf("\nread success: %s", buf_r);       
           if (msgctl(msg_id, IPC_RMID, 0) != -1)//rmove MQ
           printf("\ndelete msg success");
       }
       else 
       {
           perror("\nError_mesg:");
       }
       exit(0);
   }
    else if(pid==0) //child process
    {
        strcpy(buf_w, "hello word.\n");
        if (msgsnd(msg_id,buf_w, sizeof(buf_w), 0) !=-1) //write to queues
            printf("\nsuccess");            
        else
            perror("\nError:");

        exit(0);
    }
} 
else 
{
        perror("\nError:");
}

return 0;
}

