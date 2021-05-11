#include <ctype.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define PERM 0644

typedef int pipe_t[2];
/*
    //  Invece di fare una open per ogni singolo figlio, avendo un unico file è smarter usare una dup
    //  con una open sullo 0-esimo e una dup sugli (B-1) esimi

    ERRORI:
        - aver usato argv[1][0] per parametri a più di un carattere invece di atoi
        - concatenazione della stringa con un puntatore + strcat invece che con char array + strcpy + strcat 
        -chiudere i lati in scrittura nel figlio per tutti i figli se non il corrente i-esimo 
        - array di PID per i figli, per stampare l indice corrispettivo nel padre (con if tra pid[i] e pidFiglio nella wait)
        - diverso tipo di allocazione per le pipe (typedef+malloc)

*/
void _free(pipe_t* p, int argc, int* pid, int* fd_arr) {
    // qui andrebbero messe tutte le free,
    // per poi chiamare questa funzione prima di ogni exit e a fine codice
}

int main(int argc, char** argv) {
    if (argc != 4) {
        puts("Errore numero di parametri, richiesti 3");
        exit(-1);
    }

    int pidFiglio, status, nread, fd_write, ritorno;

    int son_len = 0, father_message_number = 0, father_len = 0;
    char son_mess[512], father_buffer;
    char filename[512];
    /*
        quando si deve creare un file con un estensione partendo dall argv
        usare array o malloc, non char* filename = argv[1] o si modificheranno
        entrambe le stringhe. (obv ggez)

        strcat ->   concatena
    */
    int B = atoi(argv[2]);
    int L = atoi(argv[3]);
    int* pid = malloc(sizeof(int) *
                      B);  // array per salvare i figli e portarli al padre

    strcpy(filename, argv[1]);
    strcat(filename, ".Chiara");

    pipe_t* _pipe = malloc(sizeof(pipe_t) *
                           B);  // una pipe arr di due elementi per ogni figlio
    int* file_desc = calloc(B, sizeof(int));

    if (!_pipe || !pid) {
        puts("Errore allocazione dinamica");
        exit(-2);
    }

    printf("\n File originario: %s\n File creato: %s\n", argv[1], filename);
    fd_write = creat(filename, PERM);
    if (fd_write < 0) {
        printf("Impossibile creare il file %s\n", filename);
        exit(-3);
    }

    // apro F (argv[1])

    if ((file_desc[0] = open(argv[1], O_CREAT | O_RDONLY)) < 0) {
        puts("Errore in apertura file");
        exit(-4);
    }

    // creo le pipe in un for dedicato
    for (int q = 0; q < B; q++) {
        if (pipe(_pipe[q]) != 0) {
            printf("Errore creazione delle pipe con indice %d\n", q);
            exit(-5);
        }
        if (q != 0)
            file_desc[q] = dup(file_desc[0]);
    }

    for (int i = 0; i < B; i++) {
        if ((pid[i] = fork()) < 0) {  // salvo i pid nell indice i
            printf("Errore creazione figlio per l'indice %d\n", i);
            exit(-6);
        }
        if (pid[i] == 0) {
            /* Quando ho n figli e sto trattando l i-esimo figlio devo:
                      -   chiudere la lettura del figlio i esimo con
                              ->  close(_pipe[i][0])
                        -   chiudere la scrittura di tutti gli altri n-1

                        for(int k = 0; k < n_totale_figli; k++){
                            if(k!=i)
                                close(_pipe[k][1]);
                        }*/

            close(_pipe[i][0]);  // chiudo quindi la lettura dell attuale
            for (int k = 0; k < B; k++) {
                if (k != i)
                    close(_pipe[k][1]);  // chiudo le altre scritture
            }
            if ((file_desc[i] = open(argv[1], O_RDONLY)) < 0) {
                printf("Errore apertura file %s", argv[1]);
                exit(0); /* decidiamo di tornare 0 che verra' interpretato dal
                            padre come valore non ammissibile!*/
            }

            printf("Sono il figlio %d ed ho associato %d con %d blocco\n",
                   getpid(), file_desc[i], i);

            lseek(file_desc[i], (long)i * L / B, SEEK_SET);
            read(file_desc[i], son_mess, L / B);
            printf("figlio %d ho letto --> %s\n ", getpid(), son_mess);

            write(_pipe[i][1], &son_mess[L / B - 1], sizeof(char));
            exit(L / B);
        }
    }

    // padre
    for (int i = 0; i < B; i++)
        close(_pipe[i][1]);

    for (int i = 0; i < B; i++) {
        printf(
            "\n\nPadre %d sta per iniziare a leggere i messaggi dalla pipe\n\n",
            getpid());

        nread = read(_pipe[i][0], &father_buffer, sizeof(char));

        // debug
        printf("\nCARATTERE RICEVUTO: (dettagli)\n\n\tVALORE: %c\n",
               father_buffer);

        write(fd_write, &father_buffer, sizeof(char));
    }

    // attesa terminazione
    printf("\n\n\n");
    for (int i = 0; i < B; i++) {
        pidFiglio = wait(&status);
        if (pidFiglio < 0) {
            printf("Errore in wait\n");

            exit(-7);
        }
        if (pid[i] == pidFiglio) {
            if ((status & 0xFF) != 0)
                printf("Figlio con pid %d terminato in modo anomalo\n",
                       pidFiglio);
            else {
                ritorno = (int)((status >> 8) & 0xFF);
                if (ritorno == 255)
                    puts("Errore!");
                else {
                    printf("Il figlio con pid=%d ha ritornato %d con indice %d\n", pidFiglio,
                           ritorno, i);
                }
            }
        }
    }

    free(_pipe);
    free(pid);
    close(fd_write);
    for (int i = 0; i < B; i++)
        close(file_desc[i]);
    free(file_desc);
    exit(0);
}