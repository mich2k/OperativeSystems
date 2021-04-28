#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void){
	printf("UNO\n");
	int pid=fork();
	/*
	*	Non possiamo sapere chi ha eseguito il secondo printf
	* 	Padre e figlio condividono  I/O, PC, video
	*
	*
	*/
	if(pid == -1)
		return -1;
	printf("DUE\n");
	return 0;
}
