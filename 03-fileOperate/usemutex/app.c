#include <stdio.h>
#include <fcntl.h>      /* O_RDWR */
#include <pthread.h>    /* pthread_create */
#include <stdlib.h>
#include <unistd.h>
#define RW_BUFF_SIZE (5)
#define NUM_THREADS 3

/* thread function*/
void *wirite_share_mem(void *threadid)
{
        long tid;
        tid = (long)threadid;

        unsigned char *wrbuf;
        unsigned char *rdbuf;
        int row_count = 32;
        int i;
        int fd;

        wrbuf = malloc(RW_BUFF_SIZE);
        rdbuf = malloc(RW_BUFF_SIZE);

        fd = open("/dev/mutexTest", O_RDWR);
        if(fd < 0)
        {
                printf("[app] share mem node open fail\n");
                pthread_exit(NULL);
        }

        /* thread 0,1 -->write / thread 2 -->read */
        switch(tid)
        {
                case 0:
                case 1:
                        for(i = 0; i < RW_BUFF_SIZE; i++)
                                *(wrbuf + i) = tid + 'A';
                        lseek(fd, 0, 0);
                        write(fd, wrbuf, RW_BUFF_SIZE);
                        break;
                case 2:
                        lseek(fd, 0, 0);
                        read(fd, rdbuf, RW_BUFF_SIZE);
                        for(i = 0; i < RW_BUFF_SIZE; i++)
                        {
                                if(i == 0)
                                        printf("%*d:", 2, 1);
                                printf("%c  ", *(rdbuf + i));
                                if(i % row_count == (row_count - 1))
                                        printf("\n%*d:", 2, i / row_count + 2);
                        }
                        printf("\n");
        }

        close(fd);
	pthread_detach(pthread_self()); 
        pthread_exit(NULL);
}

int main (int argc, char *argv[])
{
        pthread_t threads[NUM_THREADS];
        int rc,threadid;

        /* creat thread */
        for(threadid = 0 ; threadid < NUM_THREADS; threadid++)
        {
                printf("[app] creating thread %d\n", threadid);
                rc = pthread_create(&threads[threadid], NULL, wirite_share_mem, (void *)threadid);
                if (rc)
                {
                        printf("[app] ERROR; return code from pthread_create() is %d\n", rc);
                        exit(-1);
                }
                usleep(100);
        }
        printf("[app] main: exit!\n");
        pthread_exit(NULL);
        return 0;
}
