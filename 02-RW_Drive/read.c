#include <sys/types.h>  
#include <sys/stat.h>  
#include <stdio.h>  
#include <fcntl.h>  
void main(void)  
{  
    int fd;//, num; 
    char num[10];
    int len;
    fd=open("/dev/IOTest", O_RDWR, S_IRUSR|S_IWUSR);     //可读写方式打开设备文件  
    if(fd!=-1)  
    {  
        read(fd, &num, 5*sizeof(char));                        //读取设备变量  
        printf("The variable is %s\n", num);  
        close(fd);   //关闭设备文件   
    }  
    else  
    {  
        perror("ERROR:");  
        printf("Device open failure\n");  
    }  
}
