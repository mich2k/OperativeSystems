#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define PERM 0644

typedef int pipe_t[2];

int main(int argc, char** argv) {
    int pid, j, piped[2];
    int L;                          
    int Lun;
    char inpbuf[512];             
    int pidFiglio, status, ritorno; 

    // passo n stringhe per argv, creo un figlio e il figlio manda la lunghezza da leggere e la stringa stessa da stampare

    // controllo n parametri
    if(argc < 1){
        printf("Richiesto almeno un parametro stringa");
        exit(1);
    }

    // creo pipe
    if (pipe(piped) < 0) {
        printf("Errore creazione pipe\n");
        exit(2);
    }

    // creo figlio
    if ((pid = fork()) < 0) {
        printf("Errore creazione figlio\n");
        exit(3);
    }

    /* figlio */
    if (pid == 0) {
        int fd;
        close(piped[0]); // chiudo lettura sul figlio

        printf("Figlio %d sta per iniziare a scrivere una serie di messaggi\n",getpid());

        /* con un ciclo leggiamo tutte le linee e ne calcoliamo la lunghezza
         */
        L = 0; /* valore iniziale dell'indice */
        j = 0; /* il figlio inizializza la sua variabile j per contare i
         messaggi che ha mandato al padre */
        while (j < argc -1) {

                L= strlen(argv[j+1]);
                argv[j+1][L]='\0';
                L++;
                write(piped[1], &L, sizeof(L));
                /* comunichiamo la stringa corrispondente alla
                linea al processo padre */
                write(piped[1], argv[j+1], L);
                j++;
            }
        printf("Figlio %d scritto %d messaggi sulla pipe\n", getpid(), j);
        exit(0);
    }


    /* padre */
    close(piped[1]); // chiudo scrittura sul padre
    printf("Padre %d sta per iniziare a leggere i messaggi dalla pipe\n", getpid());

    j = 0; // con j conto quanti messaggi ho ricevuto man mano
      
    while (read(piped[0], &Lun, sizeof(Lun)) != 0) {
        /* ricevuta la lunghezza, il padre puo' andare a leggere la
       linea/stringa */
        read(piped[0], inpbuf, Lun);
        /* dato che il figlio gli ha inviato delle stringhe, il padre le
       puo' scrivere direttamente con una printf */
        printf("%d: %s\n", j, inpbuf);
        j++;
    }
    printf("Padre %d letto %d messaggi dalla pipe\n", getpid(), j);
    /* padre aspetta il figlio */
    pidFiglio = wait(&status);
    if (pidFiglio < 0) {
        printf("Errore wait\n");
        exit(5);
    }
    if ((status & 0xFF) != 0)
        printf("Figlio con pid %d terminato in modo anomalo\n", pidFiglio);
    else {
        ritorno = (int)((status >> 8) & 0xFF);
        printf("Il figlio con pid=%d ha ritornato %d (se 255 problemi!)\n",
               pidFiglio, ritorno);
    }
    exit(0);
}