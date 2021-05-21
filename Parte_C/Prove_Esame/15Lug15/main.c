
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#define PERM 0644

typedef int pipe_t[2];

// PROVA ESAME DA LEZIONE

int main(int argc, char** argv) {
    int N;
    int* pid;
    pipe_t* pipeFiglioPadre;
    pipe_t* pipePadreFiglio;
    int i, j;
    int fdw, fd;
    int pidFiglio, status, ritorno;
    char c;
    int primoindice;
    char token = 'x';
    int nr;

    if (argc < 3) {
        printf("Errore numero di parametri %d\n", argc);
        exit(1);
    }

    N = argc - 1;
    printf("Numero di processi da creare %d\n", N);

    if ((pid = (int*)malloc(N * sizeof(int))) == NULL) {
        printf("Errore allocazione pid\n");
        exit(2);
    }

    if ((pipeFiglioPadre = (pipe_t*)malloc(N * sizeof(pipe_t))) == NULL) {
        printf("Errore allocazione pipe padre\n");
        exit(3);
    }

    if ((pipePadreFiglio = (pipe_t*)malloc(N * sizeof(pipe_t))) == NULL) {
        printf("Errore allocazione pipe padre\n");
        exit(4);
    }

    for (i = 0; i < N; i++)  // creo pipe (queste due si potevano fare in un
                             // unico for senza segnali)
        if (pipe(pipeFiglioPadre[i]) < 0) {
            printf("Errore creazione pipe\n");
            exit(5);
        }

    for (i = 0; i < N; i++)  // creo pipe (questa non serviva con i segnali)
        if (pipe(pipePadreFiglio[i]) < 0) {
            printf("Errore creazione pipe\n");
            exit(6);
        }

    if ((fdw = creat("Merge", PERM)) <
        0) {  // si poteva usare la creat 'avanzata'
        printf("Impossibile creare il file Merge\n");
        exit(7);
    }

    for (i = 0; i < N;
         i++) {  // mi serve l array dinamico perchè il SIGKILL chiede il pid
        if ((pid[i] = fork()) < 0) {
            printf("Errore creazione figli\n");
            exit(8);
        } else if (pid[i] == 0) {
            printf("Sono il figlio %d di indice %d\n", getpid(), i);

            for (j = 0; j < N; j++) {  // il figlio chiude i lati non usati
                close(pipeFiglioPadre[j][0]);
                close(pipePadreFiglio[j][1]);
                if (j != i) {  // ci serve solo la sua scrittura figlio-padre e
                               // la lettura padre-figlio
                    close(pipeFiglioPadre[j][1]);
                    close(pipePadreFiglio[j][0]);
                }
            }

            if ((fd = open(argv[i + 1], O_RDONLY)) < 0) {
                printf("Impossibile aprire il file %s\n", argv[i + 1]);
                exit(-1);  // dentro al figlio si torna sempre -1, fuori da
                           // questo si tornano valori positivi crescenti
            }

            do {
                read(pipePadreFiglio[i][0], &token, 1); // attendo che il padre mi dica che 'tocca a me' sul figlio
                nr = read(fd, &c, 1);
				//printf("Letto token per carattere %c\n",c);
                if (nr) {	// se ha letto qualcosa lo va a scrivere nella pipe per mandarlo al padre
                    write(pipeFiglioPadre[i][1], &c, 1);
                }
            } while (nr);

            exit(c);
        }
    }

    for (i = 0; i < N; i++) {
        close(pipePadreFiglio[i][0]);
        close(pipeFiglioPadre[i][1]);
    }

    nr = 1;
    while (nr)
        for (i = 0; i < N; i++) {
            write(pipePadreFiglio[i][1], &token, 1);
            nr = read(pipeFiglioPadre[i][0], &c, 1);
            if (nr) {	// quando riesce a leggere il carattere dal figlio
                write(fdw, &c, 1);	// scrivo in Merge
            } else {
                primoindice = i;
                break;
            }
        }
    printf("Valore di primoindice %d\n", primoindice);

    for (i = 0; i < N; i++)
        if (i != primoindice)
            if (kill(pid[i], SIGKILL) == -1)
                printf(
                    "Figlio con pid %d non esiste e quindi e' gia' terminato\n",
                    pid[i]);
    ;

    for (i = 0; i < N; i++) {
        pidFiglio = wait(&status);
        if (pidFiglio < 0) {
            printf("Errore in wait\n");
            exit(9);
        }

        if ((status & 0xFF) != 0)
            printf("Figlio con pid %d terminato in modo anomalo\n", pidFiglio);
        else {
            ritorno = (int)((status >> 8) & 0xFF);
            printf("Il figlio con pid=%d ha ritornato il carattere %c\n",
                   pidFiglio, ritorno);
        }
    }
    exit(0);
}