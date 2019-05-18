void * mmap(void *start, size_t length, int prot , int flags, int fd, off_t offset)

Start: The starting address of the memory area to be mapped to, usually NULL (NULL is 0). NULL means that the memory address is specified by the kernel.
Length: the size of the memory area to be mapped.

Prot: The expected memory protection flag cannot conflict with the open mode of the file. Is one of the following values, which can be reasonably combined by the or operation

PROT_EXEC //page content can be executed
PROT_READ // page content can be read
PROT_WRITE // page can be written
PROT_NONE // page is not accessible

Flags: Specifies the type of mapping object, whether mapping options and mapping pages can be shared. Its value can be a combination of one or more of the following bits.
MAP_SHARED : The write data to the mapped area is copied back into the file and allowed to be shared by other processes that map the file.
MAP_PRIVATE : Create a private map that is copied at write time. The writing of the memory area does not affect the original file. This flag and the above flags are mutually exclusive and only one of them can be used.