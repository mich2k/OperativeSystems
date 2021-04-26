#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void){
	int pid;
	printf("UNO\n");

	if ((pid=fork()) < 0){
		// fork fallita
		printf("errore in fork\n");
		exit(1);
		}
	if (pid == 0){
		// qui si esegue il figlio
		printf("DUE\n");
	}else{
		/*
		* per il padre non sarà vero pid pari a zero, quindi eseguirà questo codice
		*/
		printf("Ho creato il figlio con PID: %d\n", pid);
		}
	exit(0); // questa parte invece è eseguita da entrambi


}
