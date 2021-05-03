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

int main(int argc, char**argv){
    int exit_s,status,pidFiglio;
    int pid = fork();
    if (pid < 0){
        printf("\nErrore nella fork");
        exit(1);
    }
    if (pid==0){
        printf("Il PID del processo figlio è %d\n", getpid());
        printf("Il PID del processo padre è %d\n", getppid());
        sleep(2);
        int rand= my_random(99);
        printf("Random number dal figlio: %d\n",rand);
        exit(rand);
    }

    if (( pidFiglio = wait(&status)) < 0){
        puts("Errore nella wait");
        exit(2);
    }

    if (pid==pidFiglio){
        puts("Figlio terminato");
    }else{
        puts("Pid non corrispondenti");
        exit(3);
    }

    if((status & 0xFF) != 0){
        puts("Figlio terminato anomalmente");
       // exit(4);
    }else{
        exit_s=status >> 8;
        exit_s &= 0xff;
        printf("Il figlio %d è terminato con %d\n",pid,exit_s);
    }
    exit(0);

}
