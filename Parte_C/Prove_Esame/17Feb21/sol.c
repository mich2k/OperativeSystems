
#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

typedef int pipe_t[2];

int main(int argc, char** argv) {
    int Q;

    int pid;
    pipe_t* pipes;
    int q, j;

    int fd;
    int pidFiglio, status, ritorno;
    char linea[250];

    int nrnum;
    char ok;
    int nr, nw;

    if (argc < 3) {
        printf("Errore numero di parametri\n");
        exit(1);
    }

    Q = argc - 1;
    printf("Numero di processi da creare %d\n", Q);

    if ((pipes = (pipe_t*)malloc(Q * sizeof(pipe_t))) == NULL) {
        printf("Errore allocazione pipe\n");
        exit(2);
    }

    for (q = 0; q < Q; q++)
        if (pipe(pipes[q]) < 0) {
            printf("Errore creazione pipe\n");
            exit(3);
        }

    for (q = 0; q < Q; q++) {
        if ((pid = fork()) < 0) {
            printf("Errore creazione figlio\n");
            exit(4);
        } else if (pid == 0) {
            printf("Sono il figlio %d e sono associato al file %s\n", getpid(),
                   argv[q + 1]);

            for (j = 0; j < Q; j++) {
                if (j != q)
                    close(pipes[j][0]);
                if (j != (q + 1) % Q)
                    close(pipes[j][1]);
            }

            if ((fd = open(argv[q + 1], O_RDONLY)) < 0) {
                printf("Impossibile aprire il file %s\n", argv[q + 1]);
                exit(-1);
            }

            j = 0;

            nrnum = 0;

            while (read(fd, &(linea[j]), 1) != 0) {
                if (linea[j] == '\n') {
                    nr = read(pipes[q][0], &ok, sizeof(char));

                    if (nr != sizeof(char)) {
                        printf(
                            "Figlio %d ha letto un numero di byte sbagliati "
                            "%d\n",
                            q, nr);
                        exit(-1);
                    }

                    linea[j] = '\0';
                    printf(
                        "Figlio con indice %d e pid %d ha letto %d caratteri "
                        "numerici nella linea %s\n",
                        q, getpid(), nrnum, linea);

                    nw = write(pipes[(q + 1) % Q][1], &ok, sizeof(char));

                    if (nw != sizeof(char)) {
                        printf(
                            "Figlio %d ha scritto un numero di byte sbagliati "
                            "%d\n",
                            q, nw);
                        exit(-1);
                    }

                    j = 0;
                    ritorno = nrnum;
                    nrnum = 0;
                } else {
                    if (isdigit(linea[j]))
                        nrnum++;
                    j++;
                }
            }

            exit(ritorno);
        }
    }

    for (q = 1; q < Q; q++) {
        close(pipes[q][0]);
        close(pipes[q][1]);
    }

    nw = write(pipes[0][1], &ok, sizeof(char));

    if (nw != sizeof(char)) {
        printf("Padre ha scritto un numero di byte sbagliati %d\n", nw);
        exit(5);
    }

    close(pipes[0][1]);

    for (q = 0; q < Q; q++) {
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