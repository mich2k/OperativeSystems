#include <ctype.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define PERM 0644 a

typedef int pipe_t[2];

// exit(0) alla fine di son & grandson
// chiudere tutti i lati delle pipe

int main(int argc, char** argv) {
    if (!(argc >= 3)) {
        printf("\nNumero di parametri non adatto\n");
        // exit (1);
    }
    int N = argc - 1, pid_nipote, status, ritorno;
    int pid, pidNipote, file_descriptor, ret;
    long int trasformFiglio = 0, trasformNipote = trasformFiglio;
    char c, char_to_write;
    pipe_t* figlioPadre = malloc(sizeof(pipe_t) * (argc - 1));
    pipe_t* nipotePadre = malloc(sizeof(pipe_t) * (argc - 1));

    if (!figlioPadre) {
        printf("\nErrore malloc");
        exit(2);
    }

    for (int i = 0; i < N; i++) {  // creo pipe
        if (pipe(figlioPadre[i]) < 0) {
            printf("\nErrore generazione pipe di indice %i", i);
            exit(3);
        }
    }

    for (int i = 0; i < N; i++) {  // creo pipe
        if (pipe(nipotePadre[i]) < 0) {
            printf("\nErrore generazione pipe nipote di indice %i", i);
            exit(4);
        }
    }

    for (int i = 0; i < N; i++) {
        if ((pid = fork()) < 0) {
            printf("\nErrore creazione figlio con indice %d\n", i);
            exit(5);
        } else if (pid == 0) {
            // chiudo pipe
            for (int pipe_count = 0; pipe_count < N; pipe_count++) {
                if (pipe_count != i) {
                    close(figlioPadre[pipe_count][0]);
                    close(figlioPadre[pipe_count][1]);
                }
                close(nipotePadre[pipe_count][0]);
                close(nipotePadre[pipe_count][1]);
            }
            close(figlioPadre[i][0]);

            /*printf(
                "Figlio indice %d, con pid %d: ho chiuso le pipe e sono "
                "associato al file: \n\tfilename: %s",
                i, getpid(), argv[i + 1]);*/

            if ((file_descriptor = open(argv[i + 1], O_RDWR)) < 0) {
                printf("Errore in apertura file per figlio con indice %d", i);
                exit(-1);
            }

            if ((pid_nipote = fork()) < 0) {
                printf("\nErrore creazione nipote per figlio con indice %d", i);
                exit(-1);
            } else if (pid_nipote == 0) {  // codice nipote

                // chiudo pipe
                for (int pipe_count = 0; pipe_count < N; pipe_count++) {
                    if (pipe_count != i) {
                        close(nipotePadre[pipe_count][0]);
                        close(nipotePadre[pipe_count][1]);
                    }
                    close(figlioPadre[pipe_count][0]);
                    close(figlioPadre[pipe_count][1]);
                }
                close(figlioPadre[i][0]);

                if ((file_descriptor = open(argv[1], O_RDWR)) < 0) {
                    printf("Errore in apertura file per nipote con pid %d",
                           getpid());
                    exit(-1);
                }

                while ((ret = read(file_descriptor, &c, 1)) > 0) {
                    if (isalpha(c) && islower(c)) {
                        char_to_write = toupper(c);
                        lseek(file_descriptor, -1, SEEK_CUR);
                        write(file_descriptor, &char_to_write, sizeof(char));
                        trasformNipote++;
                    }
                }

                write(nipotePadre[i][1], &trasformNipote, sizeof(long int));
                exit(trasformNipote/256);  // ACHTUNG: METTERE L 'exit()', se non si hanno specifiche, tornare zero
            }
            // codice figlio
            char_to_write = ' ';
            while ((ret = read(file_descriptor, &c, 1)) > 0) {
                if (isdigit(c)) {
                    // un passo indietro col seek?
                    lseek(file_descriptor, -1, SEEK_CUR);
                    write(file_descriptor, &char_to_write, sizeof(char));
                    trasformFiglio++;
                }
            }

            write(figlioPadre[i][1], &trasformFiglio, sizeof(long int));

            // wait nipote
            pidNipote = wait(&status);
            if (pidNipote < 0) {
                printf("Errore in wait\n");
                exit(-7);
            }
            if ((status & 0xFF) != 0)
                printf("Nipote con pid %d terminato in modo anomalo\n",
                       pidNipote);
            else {
                ritorno = (int)((status >> 8) & 0xFF);
                if (ritorno == 255)
                    puts("Errore!");
                else {
                    printf("\nIl nipote con pid=%d ha ritornato %d\n",
                           pidNipote, ritorno);
                    exit(ritorno);
                }
            }
            exit(0);
        }
    }

    long int ritNipote, ritFiglio;

    for (int i = 0; i < N; i++) {
        // chiudo pipe
        for (int pipe_count = 0; pipe_count < N; pipe_count++) {
            if (i != pipe_count) {
                close(nipotePadre[pipe_count][0]);
                close(nipotePadre[pipe_count][1]);
                close(figlioPadre[pipe_count][0]);
                close(figlioPadre[pipe_count][1]);
            } else {
                close(figlioPadre[pipe_count][1]);
                close(nipotePadre[pipe_count][1]);
            }
        }

        while (read(figlioPadre[i][0], &ritFiglio, sizeof(long int)) != 0) {
            read(nipotePadre[i][0], &ritNipote, sizeof(long int));
            printf(
                "\n OUTPUT PADRE (%d):\n\tIl figlio ha effettuato %ld trasformazioni\n\tIl nipote "
                "associato ha effettuato %ld trasformazioni\n\tsul file \'%s\'",
                i, ritFiglio, ritNipote, argv[i + 1]);
        }
    }

    // codice padre
    printf("\n\n\n");
    for (int i = 0; i < (argc - 1); i++) {
        pid = wait(&status);
        if (pid < 0) {
            printf("Errore in wait padre\n");
            exit(6);
        }

        if ((status & 0xFF) != 0)
            printf("Figlio con pid %d terminato in modo anomalo\n", pid);
        else {
            ritorno = (int)((status >> 8) & 0xFF);
            if (ritorno == 255)
                printf(
                    "Il figlio con pid=%d ha ritornato %d e quindi vuole "
                    "dire che il figlio ha avuto dei problemi oppure il "
                    "nipote non è riuscito ad eseguire il wc oppure è "
                    "terminato in modo anormale\n",
                    pid, ritorno);
            else
                printf("Il figlio con pid=%d ha ritornato %d\n", pid, ritorno);
        }
    }
    exit(0);
}