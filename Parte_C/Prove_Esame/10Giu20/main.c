#include <ctype.h>
#include <fcntl.h>  // controllo file
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>    // strlen per char input
#include <sys/wait.h>  // child wait
#include <time.h>      // seed numeri casuali
#include <unistd.h>    // unix cmd

#define PERM 0644

void _free(int** piped, int* file_desc_arr, int argc){
    for(int k = 1; k < argc - 1; k++)
        free(piped[k]);
    free(file_desc_arr);
    free(piped);
}

int main(int argc, char** argv) {
    if (!(argc >= 3)) {
        puts("Errore numero di parametri, richiesti >= 2");
        exit(-1);
    }

    int pid, pidFiglio, status, nread, fd_write, ritorno, n_righe = 0;
    int* file_desc_arr = malloc(sizeof(int) * (argc - 1));

    int son_len = 0, father_message_number = 0, father_len = 0;
    char son_mess[512], father_buffer[512];
    char* filename;

    // VERSIONE DINAMICA:

    int** piped = malloc(sizeof(int) * (argc-1));
    for(int k = 1; k <= argc-1; k++)
        piped[k] = malloc(2*sizeof(int));
    
    // VERSIONE AUTOMATICA
    //int piped[512][2];



    for (unsigned int i = 1; i <= (argc - 1); i++) {
        // printf("%s\n", argv[i]);  DEBUG
        if (argv[i][0] != '/') {
            printf(
                "\nErrore con il path di indice \'%d\', non è un path "
                "assoluto.\n",
                i);
            exit(-2);
        } else {
            if ((file_desc_arr[i] = open(argv[i], O_RDONLY)) < 0) {
                printf("Errore in apertura del %d file, ovvero\n%s\n", i,
                       argv[i]);
                _free(piped,file_desc_arr,argc);
                exit(-3);
            }
        }
    }

    filename = "Camilla.out";
    fd_write = creat(filename, PERM);
    if (fd_write < 0) {
        printf("Impossibile creare il file %s\n", filename);
        // chiudi file
        _free(piped,file_desc_arr,argc);
        exit(-4);
    }
    close(fd_write);

    /* creazione figli */
    for (int i = 1; i <= (argc - 1); i++) {
        if (pipe(piped[i]) != 0) {  // CICLO PIPE DEDICATO !
            printf("Errore creazione pipe con indice %d", i);
            _free(piped,file_desc_arr,argc);
            exit(-5);
        }

        if ((pid = fork()) < 0) {
            printf("Errore creazione figlio per l'indice %d\n", i);
            // chiudi file
            _free(piped,file_desc_arr,argc);
            exit(-6);
        } else if (pid == 0) {
            /* codice figlio */
            close(piped[i][0]);
            for(int k = 0; k < (argc-1); k++){
                if(k!=i)
                    close(piped[k][1]);
            }
            printf("Sono il figlio %d di indice %d ed ho associato %d\n",
                   getpid(), i, file_desc_arr[i]);
            while ((nread = read(file_desc_arr[i], &(son_mess[son_len]), 1)) !=
                   0) {
                if (son_mess[son_len] != '\n') {
                    son_len++;

                } else {
                    //printf("\nprimo char: %c\n lunghezza: %d", son_mess[0], son_len + 1); DEBUG
                    if (isalnum(son_mess[0]) && (son_len + 1) < 10) {
                        son_mess[son_len++] = '\0';
                        // printf("sono %d e ho letto %s e mando %d", getpid(),
                        // son_mess, son_len);  // DEBUG

                        write(piped[i][1], &son_len, sizeof(son_len));
                        write(piped[i][1], son_mess, son_len);
                        n_righe++:
                    }
                    son_len = 0;  //  va resettato così che si possa
                                  //          calcolare la lunghezza delle
                                  //          successive stringhe
                }
            }
            close(file_desc_arr[i]);
            exit(n_righe);
        }
    }

    // padre

    for (int i = 1; i <= (argc - 1); i++) {  // ACHTUNG
        close(piped[i][1]);

        printf("\n\nPadre %d sta per iniziare a leggere i messaggi dalla pipe\n\n",
               getpid());

        while (read(piped[i][0], &father_len, sizeof(father_len)) != 0) {
            /* ricevuta la lunghezza, il padre puo' andare a leggere la
           linea/stringa */

            read(piped[i][0], father_buffer, father_len);
            // magari stampare anche il file descriptor da cui lo riceve, o il PID del figlio o il filename da cui deriva
            printf("\nSTRINGA RICEVUTA: (dettagli)\n\tLUNGHEZZA: %d\n\tMESS: %s\n", father_len, father_buffer);

            /* dato che il figlio gli ha inviato delle stringhe, il padre le
           puo' scrivere direttamente con una printf */
            // printf("%d: %s\n", father_message_number, father_buffer);
            father_message_number++;
        }
    }

    printf("\n\n\n");
    /* padre aspetta il figlio */
    // la wait deve stare fuori dai for
    pidFiglio = wait(&status);
    if (pidFiglio < 0) {
        printf("Errore in wait\n");
        // chiudi file
        _free(piped,file_desc_arr,argc);
        exit(-7);
    }
    if ((status & 0xFF) != 0)
        printf("Figlio con pid %d terminato in modo anomalo\n", pidFiglio);
    else {
        ritorno = (int)((status >> 8) & 0xFF);
        if (ritorno == 255)
            puts("Errore!");
        else {
            printf("Il figlio con pid=%d ha ritornato %d linee inviate\n", pidFiglio,
                   ritorno);
        }
    }
    _free(piped,file_desc_arr,argc);
}