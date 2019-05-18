#include <unistd.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <fcntl.h>
 #include <linux/fb.h>
 #include <sys/mman.h>
 #include <sys/ioctl.h> 
 
 #define PAGE_SIZE 4096  //need to set 4096*n
 
 int main(int argc , char *argv[])
 {
     int fd;
     int i;
     unsigned char *p_map;
     
     //open file
     fd = open("/dev/mymap",O_RDWR);
     if(fd < 0)
     {
         printf("open fail\n");
         exit(1);
     }
 
/*mapping mermory 
	 void * mmap(void *start, size_t length, int prot , int flags, int fd, off_t offset)
     *start is kernel space start adress(set 0 is allocate by kernel),usually set 0
*/
     p_map = (unsigned char *)mmap(0, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED,fd, 0);

     if(p_map == MAP_FAILED)
     {
         printf("mmap fail\n");
         goto end;
     }
 
     for(i=0;i<10;i++)
	 {
        printf("%d\n",p_map[i]);
  		p_map[i]=66;   
 	 }

     for(i=0;i<10;i++)
        printf("%d\n",p_map[i]);


 end:
     munmap(p_map, PAGE_SIZE);
     return 0;
}
