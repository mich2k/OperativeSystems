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

typedef struct {
    int c1;   // pid
    char c2;  // 2 carattere
    char c3;  // penultimo carattere
} s;

// INPUT TEST: ./a.out 1.inp $(wc -l < 1.inp) a b

int main(int argc, char** argv) {
    if (argc != 3) {
        puts("Errore numero di parametri, richiesti esattamente due parametri");
        // exit(1);
    }

    int L = atoi(argv[2]);
    char c;
    int status, ritorno, pidFiglio, pid, file_descriptor;
    pipe_t* figlioPadre = NULL;
    // F è argv[1];

    if (L <= 0 || L > 255) {
        printf("\nIl parametro L è fuori range: 1-255");
        exit(2);
    }

    figlioPadre = calloc(L, sizeof(pipe_t));

    for (int q = 0; q < L; q++) {
        if (pipe(figlioPadre[q]) < 0) {
            printf("\nErrore creazione pipe con indice %d", q);
            exit(3);
        }
    }

    for (int q = 0; q < L; q++) {
        /* creazione figli */
        if ((pid = fork()) < 0) {
            printf("Errore creazione figlio per l'indice %d\n", q);
            exit(4);
        }
        if (pid == 0) {
            /* codice figlio */

            // chiudo pipe non necessarie

            for (int k = 0; k < L; k++) {
                close(figlioPadre[k][0]);
                if (k != q)
                    close(figlioPadre[k][1]);
            }

            char buff[250];
            int count = 0;
            int l_ass = -1;
            s compat;

            if ((file_descriptor = open(argv[1], O_RDONLY)) <
                0) {  // ATTENZIONE!! 'i+1' non '1'
                puts("Errore in apertura file");
                exit(-1);
            }

            while (read(file_descriptor, &c, 1) != 0) {
                if (count == q)
                    break;
                if (c == '\n')
                    count++;
            }
            printf("\n Figlio %d associa la linea %d:", q, count);

            l_ass = count;
            count = 0;

            lseek(file_descriptor, -1L, SEEK_CUR);

            while (read(file_descriptor, &c, 1) != 0) {
                buff[count++] = c;
                if (c == '\n') {
                    // converto la stringa in stringa di char
                    buff[count] = '\0';
                    printf("\nFiglio con indice %d e pid %d linea: %s", q,
                           getpid(), buff);

                    // int nw = write(_pipe[(i + 1) % N][1], &ok, sizeof(char));
                    count = 0;
                    break;
                }
            }
            compat.c1 = getpid();
            compat.c2 = buff[0];
            compat.c3 = count;
            if(write(figlioPadre[q][1], &compat, sizeof(compat)) == -1){
                printf("\nErrore write indice %d\n",q);
                exit(-1);
            }
            exit(l_ass);
        }
    }

    // codice padre

    s s_padre;
    for (int k = 0; k < L; k++) {
        close(figlioPadre[k][1]);
    }

    for (int q = 0; q < L; q++) {

        while (read(figlioPadre[q][0], &s_padre, sizeof(s_padre)) != 0) {
            printf("\n%c e %d yee ", s_padre.c2, s_padre.c3);

            if (s_padre.c2 == s_padre.c3) {
                printf(
                    "\nLa struct tornata dal figlio con pid %d ha %c e %c "
                    "uguali",
                    s_padre.c1, s_padre.c2, s_padre.c3);
            }
            break;
        }
    }

    printf("\n\n\n");

    // wait padre
    for (int i = 0; i < L; i++) {
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