#include <ctype.h>
#include <fcntl.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#define PERM 0644

typedef int pipe_t[2];

int main(int argc, char** argv) {
    if (!(argc >= 3)) {
        printf("\nNumero di parametri non conforme.");
        exit(1);
    }

    unsigned int N = argc - 1;
    int pid, status, ritorno, pidFiglio, file_descriptor;
    char c;
    pipe_t* _pipe;
    bool ok = false;

    /* allocazione pipe */
    if ((_pipe = (pipe_t*)malloc(N * sizeof(pipe_t))) == NULL) {
        printf("Errore allocazione pipe\n");
        exit(2);
    }

    /* creazione pipe */
    for (int j = 0; j < N; j++)
        if (pipe(_pipe[j]) < 0) {
            printf("Errore creazione pipe\n");
            exit(3);
        }

    for (int i = 0; i < N; i++) {
        if ((pid = fork()) < 0) {
            printf("\nErrore creazione figlio con indice %d\n", i);
            exit(5);
        } else if (pid == 0) {  // codice figlio

            // chiudo pipe
            for (int j = 0; j < N; j++) {
                if (j != i)  // CHIUSURA PIPE PER RING
                    close(_pipe[j][0]);
                if (j != (i + 1) % N)
                    close(_pipe[j][1]);
            }

            int k = 0;

            long int cur = 0L;  // LONG !

            if ((file_descriptor = open(argv[i+1], O_RDWR)) < 0) {  // ATTENZIONE!! 'i+1' non '1'
                puts("Errore in apertura file");
                exit(-1);
            }

            /*
                - leggo il carattere
                - verifico se è compatibile
                - verifico se ho l ok
                - ... codice
                - scrivo l ok sulla pipe successiva
            */

            while (read(file_descriptor, &c, 1) != 0) {
                if ((lseek(file_descriptor, 0, SEEK_CUR)-1) == (i + k * N)) {   // invece di cur preferisco lseek che ritorna long int
                    read(_pipe[i][0], &ok, sizeof(bool));

                    printf("\noccorrenza: \'%c\' nel figlio di indice %d\n", c,
                           i);

                    if (write(_pipe[(i + 1) % N][1], &ok, sizeof(bool)) == -1) {
                        printf("\nErrore write indice %d", i);
                        exit(-1);
                    }

                    k++;
                }
                cur++;
            }
            exit(0);
        }
    }

    // codice padre
    for (int j = 1; j < N;
         j++) { /* l'indice lo facciamo partire quindi da 1! */
        close(_pipe[j][0]);
        close(_pipe[j][1]);
    }

    write(_pipe[0][1], &ok, sizeof(bool));

    // ora posso chiudere la sua lettura, dell indice zero (che ho evitato
    // primia partendo da j=1)

    close(_pipe[0][1]);

    // wait padre
    for (int i = 0; i < N; i++) {
        pidFiglio = wait(&status);
        if (pidFiglio < 0) {
            printf("Errore in wait\n");
            exit(6);
        }
        if ((status & 0xFF) != 0)
            printf("Figlio con pid %d terminato in modo anomalo\n", pidFiglio);
        else {
            ritorno = (int)((status >> 8) & 0xFF);
            printf("Il figlio con pid=%d ha ritornato %d (se 255 problemi)\n",
                   pidFiglio, ritorno);
        }
    }
    exit(0);
}