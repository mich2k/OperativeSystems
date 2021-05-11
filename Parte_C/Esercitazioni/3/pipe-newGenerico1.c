/* FILE: pipe-newGenerico1.c */
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
int main(int argc, char** argv) {
    int pid, j, piped[2];
    char mess[512];                 /* array che serve per salvare la linea
                   letta dal figlio, supposta non piu' lunga di 512 caratteri compreso il
                   terminatore di linea */
    int L;                          /* indice per la lettura di un singolo
                            carattere da file */
    char inpbuf[512];               /* array usato dal padre; NOTA BENE: poteva
                essere usato sempre l'array usato dal figlio cioe' mess */
    int Lun;                        /* lunghezza letta dal padre dalla pipe;
                          NOTA BENE: poteva essere usata sempre la variabile usata dal figlio cioe'
                          L */
    int pidFiglio, status, ritorno; /* per wait padre */
    if (argc != 2) {
        printf(
            "Numero dei parametri errato %d: ci vuole un singolo parametro\n",
            argc);
        exit(1);
    }
    /* si crea una pipe */
    if (pipe(piped) < 0) {
        printf("Errore creazione pipe\n");
        exit(2);
    }
    if ((pid = fork()) < 0) {
        printf("Errore creazione figlio\n");
        exit(3);
    }

    /* figlio */
    if (pid == 0) {
        int fd;
        close(piped[0]); /* il figlio CHIUDE il lato di lettura */
        if ((fd = open(argv[1], O_RDONLY)) < 0) {
            printf("Errore in apertura file %s\n", argv[1]);
            exit(-1); /* torniamo al padre un -1 che sara' interpretato
           come 255 e quindi identificato come errore */
        }
        printf(
            "Figlio %d sta per iniziare a scrivere una serie di messaggi, di "
            "lunghezza non nota, sulla pipe dopo averli letti dal file passato "
            "come parametro\n",
            getpid());
        /* con un ciclo leggiamo tutte le linee e ne calcoliamo la lunghezza
         */
        L = 0; /* valore iniziale dell'indice */
        j = 0; /* il figlio inizializza la sua variabile j per contare i
         messaggi che ha mandato al padre */
        while (read(fd, &(mess[L]), 1) != 0) {
            if (mess[L] == '\n') /* siamo arrivati alla fine di una linea */
            {
                /* il padre ha concordato con il figlio che gli
               mandera' solo stringhe e quindi dobbiamo sostituire il
               terminatore di linea con il terminatore di stringa */
                mess[L] = '\0';
                L++; /* incrementiamo L per tenere conto anche del
                terminatore di linea */
                     /* comunichiamo L al processo padre */
                write(piped[1], &L, sizeof(L));
                /* comunichiamo la stringa corrispondente alla
                linea al processo padre */
                write(piped[1], mess, L);
                L = 0; /* azzeriamo l'indice per la prossima linea
                        */
                j++;   /* se troviamo un terminatore di linea
                 incrementiamo il conteggio */
            } else
                L++;
        }
        printf("Figlio %d scritto %d messaggi sulla pipe\n", getpid(), j);
        exit(0);
    }


    /* padre */
    close(piped[1]); /* il padre CHIUDE il lato di scrittura */
    printf("Padre %d sta per iniziare a leggere i messaggi dalla pipe\n", getpid());

    j = 0; /* il padre inizializza la sua variabile j per verificare quanti
      messaggi ha mandato il figlio */
      
    while (read(piped[0], &Lun, sizeof(Lun))) {
        /* ricevuta la lunghezza, il padre puo' andare a leggere la
       linea/stringa */
        read(piped[0], inpbuf, Lun);
        /* dato che il figlio gli ha inviato delle stringhe, il padre le
       puo' scrivere direttamente con una printf */
        printf("%d: %s con Lun: %d \n", j, inpbuf, Lun);
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