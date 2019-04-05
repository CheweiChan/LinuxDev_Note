#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main() 
{
    char str1[20] = { 0 }; 
    void* shm;
   // 1. create SHM
    int shm_id = shmget(15, 128, IPC_CREAT | 0666);
    if (shm_id != -1)
    {
        // 2. mapping shm
        shm = shmat(shm_id, NULL, 0);
        if (shm != (void*)-1) 
        {
            // 3. write shm
            char str2[] = "share memory test";
            memcpy(shm, str2, strlen(str2) + 1);
            // 4. read shm
            memcpy(str1, shm, strlen("share memory test"));
            printf("read shm = %s\n", (char *)str1); 
        } 
        else 
        {
            perror("shmat:");
        }
    } 
    else
    {
        perror("shmget:");
    }

    // close && delete shm
    shmdt(shm);
    if (0 == shmctl(shm_id, IPC_RMID,NULL))
        printf("delete shm success.\n");



    return 0;
}

