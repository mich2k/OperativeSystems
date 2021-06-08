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
        // exit(1);
    }

    int N = argc -
            1;  //! mi rifiuto di chiamarlo Q
    int pid, status, ritorno, pidFiglio, file_descriptor;
    char c, ok;
    pipe_t* _pipe = NULL;
    _pipe = malloc(sizeof(pipe_t) * N);
    if (!_pipe) {
        printf("\nErrore allocazione dinamica");
        exit(2);
    }

    for(int i = 0; i < N; i++){ // NON DIMENTICARE DI CREARE LE PIPE!!!!!
        if(pipe(_pipe[i]) < 0){
            printf("\nErrore creaizione pipe indice %d",i);
            exit(3);
        }
    }

    for (int i = 0; i < N; i++) {
        if ((pid = fork()) < 0) {
            printf("\nErrore creazione figlio con indice %d\n", i);
            exit(4);
        } else if (pid == 0) {  // codice figlio

            int countdigit = 0, count = 0, rit = 0;
            char buff[512];

            // chiudo pipe
            for (int j = 0; j < N; j++) {
                if (j != i)  // CHIUSURA PIPE PER RING
                    close(_pipe[j][0]);
                if (j != (i + 1) % N)
                    close(_pipe[j][1]);
            }

            if ((file_descriptor = open(argv[i + 1], O_RDONLY)) <
                0) {  // ATTENZIONE!! 'i+1' non '1'
                puts("Errore in apertura file");
                exit(-1);
            }

            while (read(file_descriptor, &c, 1) != 0) {
                buff[count++] = c;
                if (c == '\n') {
                    // buff[count] = '\n';
                    if (read(_pipe[i][0], &ok, sizeof(char)) != sizeof(char)) {
                        printf("\nErrore read indice %d", i);
                        exit(-1);
                    }

                    // converto la stringa in stringa di char
                    buff[count] = '\0';
                    printf(
                        "\nFiglio con indice %d e pid %d ha letto %d "
                        "caratteri "
                        "numerici nella linea \"%s\"",
                        i, getpid(), countdigit, buff);

                    
                    //int nw = write(_pipe[(i + 1) % N][1], &ok, sizeof(char));

                    if(write(_pipe[(i + 1) % N][1], &ok, sizeof(char)) == -1){
                        printf("\nErrore write su indice %d\n", i);
                        exit(-1);
                    }

                    count = 0;
                    rit=countdigit;
                    countdigit=0;

                } else {
                    if (isdigit(c)) {
                        countdigit++;
                    }
                }
            }

            exit(rit);
        }
    }
    // codice padre
    for (int j = 1; j < N;
         j++) { /* l'indice lo facciamo partire quindi da 1! */
        close(_pipe[j][0]);
        close(_pipe[j][1]);
    }

    if (write(_pipe[0][1], &ok, sizeof(char)) == -1) {
        printf("\nErrore write padre\n");
    }

    // ora posso chiudere la sua lettura, dell indice zero (che ho evitato
    // prima partendo da j=1)

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