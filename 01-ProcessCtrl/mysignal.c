#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

void my_func(int sign_no)
{
	if(sign_no==SIGINT)
		printf("I have get SIGINT\n");
}
int main()
{
	printf("Waiting for signal SIGINT(ctrl+c) \n ");
	

	signal(SIGINT, my_func);//ctrl+c
	
	pause();
	exit(0);
}

