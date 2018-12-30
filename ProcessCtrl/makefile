CFLAGS:=-lpthread -o

all:
	@echo "create all process"
	gcc fifo_read.c -o fifo_read
	gcc fifo_write.c -o fifo_write
	gcc pthread.c $(CFLAGS) pthread
	gcc fifo_read_select.c -o fifo_read_select
	gcc pipe_rw.c -o pipe_rw
	gcc mysignal.c -o mysignal
::/*Execute fifo read before fifo write*/
fifo:
	@echo "create a fifo process"
	gcc fifo_read.c -o fifo_read
	gcc fifo_write.c -o fifo_write

::/*creat a new process and pass the local variable to process*/
pthread:
	@echo "create a pthread process"
	gcc pthread.c $(CFLAGS) pthread
clear:
	@rm -f *.o
