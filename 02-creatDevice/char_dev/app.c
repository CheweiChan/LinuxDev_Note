#include <stdio.h>
#include <fcntl.h>              /* O_RDWR */
#include <linux/ioctl.h>        /* ioctl */

#define MAGIC_NUMBER 'C'
#define _EXECUTE _IO(MAGIC_NUMBER, 1)
#define _WRITE   _IOW(MAGIC_NUMBER, 6, int)
#define _READ    _IOR(MAGIC_NUMBER, 2, int)
#define _RW           _IOWR(MAGIC_NUMBER, 7, int)
#define _IOCTL_MAXNR  8

int main(int argc, char**argv)
{
        int fd;
        int buf[10]={0};
        printf("charDevice test");

        fd = open("/dev/myCharDevice", O_RDWR);
        if(fd < 0)
        {
                printf("open fail\n");
                return -1;
        }


        write(fd, buf, 1);
        read(fd, buf, 1);

        ioctl(fd, _EXECUTE);
        ioctl(fd, _WRITE, 0);
        ioctl(fd, _READ, 0);
        ioctl(fd, _RW, 0);

        close(fd);
   

        return 0;
}
