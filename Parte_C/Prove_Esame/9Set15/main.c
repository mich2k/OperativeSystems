#include <ctype.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include <signal.h>

#define PERM 0644

typedef int pipe_t[2];

int main(int argc, char** argv) {
    if (!(argc >= 4)) {
        printf("Numero di parametri non compatibile");
        // exit(1);
    }

    int N = argc - 1;
    int status, ritorno, pidFiglio, son_file_descriptor, file_descriptor;
    pipe_t* padreFiglio = (pipe_t*)malloc(sizeof(pipe_t) * (N - 1));
    pipe_t* figlioPadre = (pipe_t*)malloc(sizeof(pipe_t) * (N - 1));
    int* pid = malloc(sizeof(int) * N), *confronto = NULL;
    char received, c;
    printf("Il padre sarà associato a %s\n", argv[argc - 1]);

    for (int i = 1; i < N; i++) {
        printf("figlio %d a %s\n", i, argv[i]);
    }

    if ((confronto = (int*)malloc(N * sizeof(int))) == NULL) {
        printf("Errore allocazione confronto\n");
        exit(3);
    }

    /* inizializzazione di confronto: tutti i valori a 1 perche' all'inizio si
     * deve mandare l'indicazione al figlio di leggere! */
    for (int i = 0; i < N; i++)
        confronto[i] = 1;

    // genero pipe pf
    for (int i = 0; i < N; i++) {
        if (pipe(padreFiglio[i]) != 0) {
            printf("Errore creazione pipe con indice %d", i);
            exit(3);
        }
    }

    // genero pipe fp
    for (int i = 0; i < N; i++) {
        if (pipe(figlioPadre[i]) != 0) {
            printf("Errore creazione pipe con indice %d", i);
            exit(3);
        }
    }

    // apro file del padre, ultimo parametro
    if ((file_descriptor = open(argv[N], O_RDONLY)) < 0) {
        printf("Impossibile aprire il file %s\n", argv[argc - 1]);
        exit(8);
    } else {
        printf("debug: riuscito ad aprire il file %s", argv[N]);
    }

    // creazione figli

    for (int i = 0; i < N; i++) {
        if ((pid[i] = fork()) < 0) {
            printf("errore creazione figlio di indice %d", i);
            exit(9);
        }
        if (pid[i] == 0) {  // codice figlio
            // chiudo le pipes inutilizzate
            // + non uso lo stesso indice (i) del ciclo for principale
            for (int j = 0; j < N; j++) {
                close(padreFiglio[j][1]);
                close(figlioPadre[j][0]);
                if (j != i) {
                    // in caso non fossero le sue i-esime pipe vado anche a
                    // chiudere i lati che a me servono, ma che non uso perchè l
                    // indice del figlio non corrisponde
                    close(padreFiglio[j][0]);
                    close(figlioPadre[j][1]);
                }
            }

            // ogni figlio è associato ad un file corrispondente al suo indice
            // vado quindi ad aprire dentro al figlio il file associato

            // ACHTUNG:
            //  non voglio un array di FD
            //  non voglio un unico file descriptor con una dup() (anche se in
            //  questo caso non ha senso kekkk (cringe zio))

            if ((son_file_descriptor = open(argv[i + 1], O_RDONLY)) < 0) {
                printf("Impossibile aprire il file %s\n", argv[i + 1]);
                exit(-1);  // -1 corrisponde a 255
            }

            while (read(padreFiglio[i][0]), &received, sizeof(char)) {
                // nella traccia i figli vanno stoppati quando il padre ha
                // finito di leggere il suo file, concordo di mandare un
                // carattere d (done)
                if (received == 'd')
                    break;
                else {
                    // il figlio scrive su fP il carattere autorizzato letto
                    read(son_file_descriptor, &c, sizeof(char));
                    printf("HO LETTO IL TOKEN per il carattere %c\n", c);
                    write(figlioPadre[i][1], &c, 1);
                }
            }
            printf(
                "\nIl figlio %d e\' riuscito a leggere il file %s prima che il "
                "padre finisse",
                getpid(), argv[i + 1]);
            exit(EXIT_SUCCESS);
        }
    }

    printf("\n\n\n");

    // codice padre
    for (int i = 0; i < N; i++) {  // lati inutilizzati
        close(figlioPadre[i][1]);
        close(padreFiglio[i][0]);
    }

    // leggo dal file del padre AF
    while (read(file_descriptor, &c, sizeof(char))) {
        // ...
    }

    for (i = 0; i < N; i++)
        if (!confronto[i]) {
            /* terminiamo forzatamente tutti i figli che hanno fallito il
             * confronto e che sono bloccati sulla read dalla pipe con il padre
             */

            // DEFINITO IN <signal.h> !

            if (kill(pid[i], SIGKILL) == -1) /* controlliamo che la kill non fallisca a causa della
                       terminazione di uno dei figli, anche se in questo caso
                       non dovremmo avere mai questo caso */
                printf(
                    "Figlio con pid %d non esiste e quindi e' gia' terminaro\n",
                    pid[i]);
            ;
        } else {
            received = 't';
            write(padreFiglio[i][1], &received, 1);
        }

    // stato di attesa
    for (int i = 0; i < N; i++) {
        pidFiglio = wait(&status);
        if (pidFiglio < 0) {
            printf("Errore in wait padre\n");
            exit(5);
        }
        if ((status & 0xFF) != 0)
            printf("Figlio con pid %d terminato in modo anomalo\n", pidFiglio);
        else {
            ritorno = (int)((status >> 8) & 0xFF);
            if (ritorno == 255)
                printf("Il figlio con pid=%d ha ritornato %d, anomalo\n",
                       pidFiglio, ritorno);
            else
                printf("Il figlio con pid=%d ha ritornato %d\n", pidFiglio,
                       ritorno);
        }
    }
    exit(0);
}