#include <ctype.h>
#include <fcntl.h>  // controllo file
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>    // strlen per char input
#include <sys/wait.h>  // child wait
#include <time.h>      // seed numeri casuali
#include <unistd.h>    // unix cmd
typedef int pipe_t[2];

int main(int argc, char** argv) {
    if (!(argc >= 3)) {
        puts("Errore numero di parametri, richiesti >= 2");
        // exit(-1);
    }
    int pidFiglio, status, nread, fd_write, ritorno,
        n_righe = 0, pid_s, pidNipote, grandson_len = 0;
    long int int_ret, int_father_ret, int_father_sum = 0;
    int pid;
    char grandson_mess[512];
    pipe_t* _pipe = malloc(sizeof(pipe_t) * (argc - 1));
    pipe_t _pipe_son;

    if (!_pipe) {
        printf("Errore malloc\n");
        exit(-1);
    }

    for (int i = 0; i < (argc - 1); i++) {
        if (argv[i + 1][0] != '/') {
            printf("Il file con indice %d non e\' in forma assoluta: %s", i,
                   argv[i + 1]);
            exit(-2);
        }
    }

    for (int i = 0; i < (argc - 1); i++) {  // IMPORTANTE: ciclo pipe DEDICATO
        if (pipe(_pipe[i]) < 0) {
            printf("Errore creazione pipe con indice %d", i);
            exit(-4);
        }
    }

    for (int i = 0; i < (argc - 1); i++) {
        /* creazione figli */
        if ((pid = fork()) < 0) {
            printf("Errore creazione figlio per l'indice %d\n", i);
            exit(-5);
        }
        if (pid == 0) {
            /* codice figlio */
            for (int k = 0; k < (argc - 1); k++) {
                close(_pipe[k][0]);
                if (k != i)
                    close(_pipe[k][1]);
            }

            // creo pipe figlio-nipote
            if (pipe(_pipe_son) != 0) {
                printf("Errore creazione pipe figlio-nipote con indice %d", i);
                exit(-6);
            }

            if ((pid = fork()) < 0) {
                printf(
                    "Errore creazione nipote per l'indice %d con figlio %d\n",
                    i, getpid());
                exit(-7);
            }
            if (pid == 0) { /* CODICE NIPOTE */
                printf(
                    "Sono il nipote del figlio di indice %d con pid %d "
                    "associato al file %s\n",
                    i, getpid(), argv[i + 1]);
                close(_pipe[i][1]);
                close(0);
                if (open(argv[i + 1], O_RDONLY) < 0) {
                    printf("Errore nella open del file %s\n", argv[i + 1]);
                    exit(-1);
                }
                close(1);
                dup(_pipe_son[1]);
                close(_pipe_son[0]);
                close(_pipe_son[1]);
                execlp("/bin/wc", "wc", "-l", (char*)0);
                fprintf(stderr, "errore exec");
                exit(-1);
            }
            close(_pipe_son[1]);  // chiudo lettura pipe figlio-nipote
            printf("Sono il figlio %d di indice %d\n", getpid(), i);

            grandson_len = 0;
            while (read(_pipe_son[0], &(grandson_mess[grandson_len++]),
                        sizeof(char)))
                ;

            if (grandson_len == 0) {
                printf("Il nipote di %d non ha tornato nessun messaggio.",
                       getpid());
            } else {
                grandson_mess[grandson_len - 1] = '\0';
                int_ret = atoi(grandson_mess);
            }
            write(_pipe[i][1], &int_ret, sizeof(int_ret));

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
                    printf("Il nipote con pid=%d ha ritornato %d\n", pidNipote,
                           ritorno);
                    exit(ritorno);
                }
            }
        }
    }

    // attesa terminazione

    for (int k = 0; k < (argc - 1); k++)
        close(_pipe[k][1]);

    // vedo cosa hanno ritornato i figli
    for (int i = 0; i < (argc - 1); i++) {
        /* il padre recupera tutti i valori interi dai figli */
        read(_pipe[i][0], &int_father_ret, sizeof(int_father_ret));
        /* stampa non richiesta, ma che male non fa */
        printf(
            "Il figlio di indice %d ha convertito il valore %ld "
            "per il file \'%s\'\n",
            i, int_father_ret, argv[i + 1]);
        int_father_sum += int_father_ret;
    }
    printf("La somma finale e\' %ld\n", int_father_sum);

    printf("\n\n\n");
    for (int i = 0; i < (argc - 1); i++) {
        pid = wait(&status);
        if (pid < 0) {
            printf("Errore in wait padre\n");
            exit(5);
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