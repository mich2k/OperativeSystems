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

typedef struct {
    int c1;        // pid nipote
    int c2;        // dimensione linea + term
    char c3[250];  // linea corrente trasformata in stringa dal padre
} s;

int main(int argc, char** argv) {
    int N = argc - 1;
    if (!(N >= 3)) {
        printf("Numero di parametri (%d) non >= 3\n", N);
        // exit(1);
    }

    pipe_t* figlioPadre = NULL;
    pipe_t figlioNipote;
    int i, pid, pidN;
    int pidFiglio, pidNipote, status, ritorno;
    char msg[512];

    // controllo assoluti

    if ((figlioPadre = (pipe_t*)malloc(N * sizeof(pipe_t))) == NULL) {
        printf("Errore allocazione pipe padre\n");
        exit(4);
    }

    /* creazione pipe */
    for (i = 0; i < N; i++)
        if (pipe(figlioPadre[i]) < 0) {
            printf("\nErrore creazione pipe (%d)\n", i);
            exit(6);
        }

    for (i = 0; i < N; i++) {
        if ((pid = fork()) < 0) {
            printf("Errore creazione figli\n");
            exit(9);
        } else if (pid == 0) {
            for (int j = 0; j < N; j++) {
                close(figlioPadre[i][0]);
                if (j != i)
                    close(figlioPadre[i][1]);
            }
            printf(
                "\nSono il figlio %d di indice %d\n\tsono associato al file "
                "%s\n",
                getpid(), i, argv[i + 1]);

            // creo pipe fN
            if (pipe(figlioNipote) < 0) {
                printf("Errore creazione pipe figlio-nipote con indice %d", i);
                exit(-1);  // ?
            }

            // close(figlioNipote[1]);

            if ((pidN = fork()) < 0) {
                printf("Errore creazione nipote indice %d", i);
                exit(-1);
            } else if (pidN == 0) {
                printf("Sono il nipote con indice %d", i);
                close(figlioPadre[i][1]);
                close(0);
                // close(figlioPadre[i][0]); già chiusa nel figlio, andrà
                // richiusa? non penso
                if (open(argv[i + 1], O_RDONLY) < 0) {
                    printf("Errore nella open del file %s\n", argv[i + 1]);
                    exit(-1);
                }
                printf("%s aperto", argv[i + 1]);
                close(1);
                dup(figlioNipote[1]);
                // dup(figlioNipote[0]);
                close(figlioNipote[0]);
                close(figlioNipote[1]);
                execlp("/bin/wc", "wc", "-c", (char*)0);
                printf("Errore exec nipote %d", i);
                exit(-1);
            }
            close(figlioNipote[1]);
            int j = 0;
            char c;
            while (read(figlioNipote[0], &msg[j], sizeof(char))) {
                printf("\ntornato %c", msg[j++]);
                if (j == 30)
                    break;
            }
            // wait figlio-nipote

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
                    exit(ritorno);  // !
                }
            }
        }
        exit(EXIT_SUCCESS);
    }

    // codice padre

    // wait padre-figlio

    printf("\n\n\n");
    for (int i = 0; i < (argc - 1); i++) {
        pidFiglio = wait(&status);
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
                    pidFiglio, ritorno);
            else
                printf("Il figlio con pid=%d ha ritornato %d\n", pidFiglio,
                       ritorno);
        }
    }
    exit(EXIT_SUCCESS);
}
