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

typedef struct {
    char v1;      // carattere del figlio Ci
    long int v2;  // numero di occorrenze di Ci
} s;

#define N 26

// exit(0) alla fine di son & grandson
// chiudere tutti i lati delle pipe

// ESERCIZIO CON STRUCT:
//  nell if va posto '== -1', non '!= 0', siccome torna la sizeof di quanto
//  letto/scritto

// se non si chiudono le pipe nel padre si potrebbe stare in handling sulla wait


// sorting di struct:
void bubbleSort(s* v, int dim) {
    int i;
    s tmp;
    int ordinato = 0;
    while (dim > 0 && !ordinato) {
        ordinato = 1;
        for (i = 0; i < dim - 1; i++) {
            if (v[i].v2 < v[i + 1].v2) {
                tmp = v[i];
                v[i] = v[i + 1];
                v[i + 1] = tmp;
                ordinato = 0;
            }
        }
        dim--;
    }
}

int main(int argc, char** argv) {
    if (!(argc == 2)) {
        printf("\nNumero di parametri non adatto\n");
        exit(1);
    }

    int status, ritorno, file_descriptor, ret;

    char c;

    s s_arr[N];

    pipe_t* figlioFiglio = malloc(sizeof(pipe_t) * (N));
    int* pid = malloc(sizeof(int) * N);

    if (!figlioFiglio || !pid) {
        printf("Errore allocazione dinamica\n");
        exit(2);
    }

    // creo pipe
    for (int i = 0; i < N; i++) {
        if (pipe(figlioFiglio[i]) < 0) {
            printf("\nErrore creazione pipe di indice %d", i);
            exit(3);
        }
    }

    for (int i = 0; i < N; i++) {
        if ((pid[i] = fork()) < 0) {
            printf("\nErrore creazione figlio con indice %d\n", i);
            exit(5);
        } else if (pid[i] == 0) {
            // codice figlio

            char alpha = 'a' + i;

            // printf("\n%c", s_arr[i].v1);

            // chiudo pipe
            for (int j = 0; j < N; j++) {
                if (j != i)
                    close(figlioFiglio[j][1]);  // resto aperta la corrente in
                                                // scrittura
                if ((i == 0) || (j != i - 1))
                    close(figlioFiglio[j][0]);  // resto aperta la precedente in
                                                // lettura
            }

            if (i > 0) {
                ret = read(figlioFiglio[i - 1][0], s_arr, sizeof(s_arr));
                if (ret == -1 || ret != sizeof(s_arr)) {
                    printf("\nErrore read o non coincidenza sull\' indice %d\n",
                           i);
                }
            }

            s_arr[i].v1 = alpha;
            s_arr[i].v2 = 0;

            // memcpy(&s_arr[i].v1, &alpha[i], sizeof(char));

            if ((file_descriptor = open(argv[1], O_RDONLY)) < 0) {
                printf(
                    "Errore in apertura file per nipote con pid %d e indice %d",
                    getpid(), i);
                exit(-1);
            }

            while (read(file_descriptor, &c, 1) != 0) {
                if (c == s_arr[i].v1) {
                    printf(
                        "Figlio indice %d, associato a \'%c\', trovata "
                        "occorrenza.\n",
                        i, s_arr[i].v1);
                    s_arr[i].v2++;
                }
            }
            ret = write(figlioFiglio[i][1], s_arr, sizeof(s_arr));
            if (ret == -1 || ret != sizeof(s_arr)) {
                printf("\nErrore write o non coincidenza sull\' indice %d\n",
                       i);
            }

            // printf("Figlio %d, ho scritto:\n\t%c e %ld\n\n", i, s_arr[i].v1,
            //        s_arr[i].v2);
            exit((int) c);
        }
    }

    // codice padre

    for (int j = 0; j < N - 2;
         j++) {  // se non si chiudono le pipe nel padre si potrebbe stare in
                 // handling sulla wait
        close(figlioFiglio[j][1]);
        close(figlioFiglio[j][0]);
    }
    close(figlioFiglio[25][1]);

    while ((ret = read(figlioFiglio[N - 1][0], s_arr, sizeof(s_arr))) != 0) {
        printf("Il padre ha letto la pipe dell ultimo figlio.\n");
    }

    bubbleSort(s_arr, N);
    puts("Stampo array ricevuto sorted:");

    for (int k = 0; k < N; k++) {
        printf("%d: pid: %d, %c, %ld\n",k, pid[s_arr[k].v1 - 97], s_arr[k].v1, s_arr[k].v2);
    }

    // wait padre
    printf("\n\n\n");
    for (int i = 0; i < N; i++) {
        pid[i] = wait(&status);
        if (pid[i] < 0) {
            printf("Errore in wait padre\n");
            exit(6);
        }

        if ((status & 0xFF) != 0)
            printf("Figlio con pid %d terminato in modo anomalo\n", pid[i]);
        else {
            ritorno = (int)((status >> 8) & 0xFF);
            if (ritorno == 255)
                printf(
                    "Il figlio con pid=%d ha ritornato %c e quindi vuole "
                    "dire che il figlio ha avuto dei problemi\n",
                    pid[i], ritorno);
            else
                printf("Il figlio con pid=%d ha ritornato %c, ASCII: %d\n", pid[i],
                       ritorno, ritorno);
        }
    }
    exit(0);
}