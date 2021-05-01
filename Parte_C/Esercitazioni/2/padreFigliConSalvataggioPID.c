#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>


int my_random(int n){
    int random;
    random=rand()%n;
    return random;
}

int main(int argc, char** argv){
	if(argc!=2){
		puts("Errore, richiesto un solo parametro!");
		exit(1);
	}
	int param = atoi(argv[1]);
	if(param < 0 || param > 154){
		puts("Errore, deve essere strettamente compreso tra 0-255");
		exit(2);
	}
	srand(time(NULL));
	printf("PID Padre: %d\n Parametro N: %d\n", getpid(), param);
	int pidFiglio, exit_s, status, pid;
	for (int i = 0; i < param; i++){
		if((pid=fork()) <  0){
			puts("Errore");
			exit(3);
		}else if(pid==0){
            int val = my_random(100+i);
			exit(val);
		}
		    if (( pidFiglio = wait(&status)) < 0){
        puts("Errore nella wait");
        exit(2);
    }

    if (pid!=pidFiglio){
        puts("Pid non corrispondenti");
        exit(3);
    }

    if((status & 0xFF) != 0){
        puts("Figlio terminato anomalmente");
       // exit(4);
    }else{
        exit_s=status >> 8;
        exit_s &= 0xff;
        printf("Il figlio è terminato:\n PID: %d\n Codice: %d\n Random: %d\n",pid,i,exit_s);
		}
	}
}
