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

int my_random(int n) {
    return (rand() % n);
}

int main(int argc, char** argv) {
    if (!(argc >= 5)) {
        puts("Errore numero di parametri, richiesti maggior uguali di 5");
        // exit(-1);
    }
    srand(time(NULL));

    int fd_write, pid, file_desc, pidFiglio, status, ritorno;
    int received_len, sent_len;
    char* filename;
    char son_buffer[512];

    int H = atoi(argv[argc - 1]);
    int N = argc - 2;
    pipe_t* piped = (pipe_t*)malloc(sizeof(pipe_t) * N);
    printf("H: %d\n", H);  // gli altri parametri vanno da 1 ad argc-2

    if (H < 0 || H > 255) {
        puts("Valore numerico non valido 0<H<255");
        exit(1);
    }

    // creo il file indicato
    filename = "/tmp/creato";
    fd_write = creat(filename, PERM);
    if (fd_write < 0) {
        printf("Impossibile creare il file %s\n", filename);
        exit(2);
    }
    close(fd_write);

    // genero pipe
    for (int i = 0; i < N; i++) {
        if (pipe(piped[i]) != 0) {  
            printf("Errore creazione pipe con indice %d", i);
            exit(3);
        }
    }

    /* creazione figli */
    for (int i = 0; i < N; i++) {
        if ((pid = fork()) < 0) {
            printf("Errore creazione figlio per l'indice %d\n", i);
            exit(4);
        } else if (pid == 0) {
            /* codice figlio */
            for (int k = 0; k < N; k++) {
                close(piped[k][0]);
                if (k != i){
                    close(piped[k][1]);
                }
            }
            printf("Sono il figlio %d di indice %d\n", getpid(), i);

            if ((file_desc = open(argv[i + 1], O_RDONLY)) < 0) {
                puts("Errore in apertura file");
                exit(5);
            }

            while (read(file_desc, &(son_buffer[i]),
                        sizeof(char))) {  // legge fino alla fine

                if (son_buffer[i] == '\n') {
                    son_buffer[i] = '\0';  
                    printf("Trovato: %s\n", son_buffer);
                    sent_len = 10;
                    if (write(piped[i][1], &sent_len, sizeof(int)) == -1) { // write(piped[i][1], &i....)
                        printf("Errore nella write del figlio %d\n", getpid());
                    }
                    i = 0;  // azzero il contatore per non mettere tutto in
                            // append
                } else
                    i++; 
            }

            exit(0);
        }
    }

    for (int k = 0; k < N; k++){
        close(piped[k][1]);
    }

    // padre
    for (int i = 0; i < N; i++) {
        for(int k = 0; k < N; k++){
            if (k!=i)
                close(piped[k][0]);
        }


        printf(
            "\n\nPadre %d sta per iniziare a leggere i messaggi dalla pipe\n\n",
            getpid());
    
        while (read(piped[i][0], &received_len, sizeof(received_len)) != 0) {   // if(read(piped[i][0], &received_len, sizeof(received_len)) != -1)

            printf("ricevuto: %d", received_len);

        }
    }

    printf("\n\n\n");

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
                printf(
                    "Il figlio con pid=%d ha ritornato %d, anomalo\n",
                    pidFiglio, ritorno);
            else
                printf("Il figlio con pid=%d ha ritornato %d\n", pidFiglio, ritorno);
        }
    }
    exit(0);
}