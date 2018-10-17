#include <stdio.h>
#include <pthread.h>
#include <unistd.h> /*getpid()*/
#include <stdlib.h>

struct menber
{
    int a;
    char *s;
};

void *create(void *arg)
{
    struct menber *temp;
    temp=(struct menber *)arg;
    printf("This thread's id is %u  \n", (unsigned int)pthread_self());
    printf("The process pid is %d  \n",getpid());
    printf("menber->a = %d  \n",temp->a);
    printf("menber->s = %s  \n",temp->s);
    return (void *)0;
}

int main(int argc,char *argv[])
{
    pthread_t tidp;
    int error;
    struct menber *b;
    b=(struct menber *)malloc( sizeof(struct menber) );
    b->a = 4;
    b->s = "zieckey";

    error = pthread_create(&tidp, NULL, create, (void *)b);

    if( error )
    {
        printf("phread is not created...\n");
        return -1;
    }
    printf("The main process's pid is %d  \n",getpid());
    sleep(1);
    return 0;
}
