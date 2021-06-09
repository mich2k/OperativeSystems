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

// INPUT TEST: ./a.out 1.inp $(wc -l < 1.inp) a b

int main(int argc, char** argv) {
    if (!(argc >= 5)) {
        puts("Errore numero di parametri, richiesti maggior uguali di 5");
        // exit(1);
    }

    // F, L, C1, ... CN

    // gli ultimi Q sono caratteri
    char ok, c;
    char filename[50];
    int L = atoi(argv[2]), N = argc - 3;
    int pid, status, ritorno, pidFiglio, file_descriptor;

    pipe_t* _pipe = malloc(sizeof(pipe_t) * N);

    if (!_pipe) {
        printf("\nErrore allocazione dinamica\n");
        exit(2);
    }

    strcpy(filename, argv[1]);

    if(L <= 0){
        printf("\nErrore: L deve essere > 0");
        exit(2); // dovrebbe essere 3 eh ma mi scoccio
    }

    // printf("\n filename: %s, lunghezza: %d, N: %d", filename, L, N);

    for (int i = 0; i < N; i++) {
        if (pipe(_pipe[i]) < 0) {
            printf("\nErrore creazione pipe di indice %d\n", i);
            exit(3);
        }
    }

    for (int i = 0; i < N; i++) {
        /* creazione figli */
        if ((pid = fork()) < 0) {
            printf("\nErrore creazione figlio per l'indice %d\n", i);
            exit(4);
        } else if (pid == 0) {
            // chiudo pipe inutilizzate
            for (int k = 0; k < N; k++) {
                close(_pipe[k][1]);
                if (k != i)
                    close(_pipe[k][0]);
            }
            char Ci = *argv[i + 3];
            long int occorrenze = 0;

            // apro il FD

            if ((file_descriptor = open(filename, O_RDONLY)) < 0) {
                puts("Errore in apertura file");
                exit(-1);
            }
            while (read(file_descriptor, &c, sizeof(char)) != 0) {
                if (c == Ci) {
                    occorrenze++;
                } else {
                    if (c == '\n') {
                        if (read(_pipe[i][0], &ok, sizeof(char)) == 1) {
                            printf(
                                "\n\tSegnale ricevuto dal padre per indice %d, "
                                "assegnato %c, trovate %ld\n",
                                i, Ci, occorrenze);
                                occorrenze = 0;
                        }
                    }
                }
            }

            exit(i);
        }
    }
    // codice padre

    // chiudo pipe lettura inutilizzate
    for (int j = 0; j < N; j++) {
        close(_pipe[j][0]);
    }

    for (int l = 0; l < L; l++) {
        printf("\n Linea %d\n", l + 1);

        for (int j = 0; j < N;
             j++) {  // chiudo tutte le scritture inutilizzate ogni ciclo
            // for(int k = 0; k < N; k++){
            //     if(k!=j)
            //         close(_pipe[k][1]);
            // }
            if (write(_pipe[j][1], &ok, sizeof(char)) == -1) {
                printf("\nErrore write padre con indice %d\n", j);
                exit(5);
            }
            sleep((double)1);
        }
    }
    // write(_pipe[0][1], &ok, sizeof(char));
    // write(_pipe[1][1], &ok, sizeof(char));

    // wait padre
    for (int i = 0; i < N; i++) {
        pidFiglio = wait(&status);
        if (pidFiglio < 0) {
            printf("\nErrore in wait\n");
            exit(6);
        }
        if ((status & 0xFF) != 0)
            printf("\nFiglio con pid %d terminato in modo anomalo\n",
                   pidFiglio);
        else {
            ritorno = (int)((status >> 8) & 0xFF);
            printf("\nIl figlio con pid=%d ha ritornato %d (se 255 problemi)\n",
                   pidFiglio, ritorno);
        }
    }
}