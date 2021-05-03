#include <fcntl.h>  // controllo file
#include <stdio.h>
#include <stdlib.h>
#include <string.h>    // strlen per char input
#include <sys/wait.h>  // child wait
#include <time.h>      // seed numeri casuali
#include <unistd.h>    // unix cmd
#include <ctype.h>
#include <stdbool.h>

#define PERM 0644

int main(int argc, char** argv) {
    
    if (!(argc >= 3)) {
        puts("Errore numero di parametri, richiesti >= 2");
        exit(-1);
    }

    int pid, file_descriptor, pidFiglio, status, nread, fd_write, cont = 0, ritorno;
    int* file_desc_arr = malloc(sizeof(int) * (argc - 1));
    char* filename;
    char c;
    char** str = malloc(sizeof(int) * (argc-1));
    bool legit = true;

    for (unsigned int i = 1; i <= (argc - 1); i++) {
        printf("%s\n", argv[i]);
        if (argv[i][0] != '/') {
            printf(
                "\nErrore con il path di indice \'%d\', non è un path "
                "assoluto.\n",
                i);
            exit(-2);
        } else {
            if ((file_desc_arr[i] = open(argv[i], O_RDONLY)) < 0) {
                printf("Errore in apertura dell %d file, ovvero\n%s", i,
                       argv[i]);
                // dovrei chiudere tutti fi files
                for (int j = 0; j < i; j++)
                    close(file_desc_arr[j]);
                free(file_desc_arr);
                exit(-3);
            }
        }
    }


    filename = "Camilla.out";
    fd_write = creat(filename, PERM);
    if (fd_write < 0) {
        printf("Impossibile creare il file %s\n", filename);
        // chiudi file
        free(file_desc_arr);
        exit(-4);
    }
    close(fd_write);

    /* creazione figli */
    for (int i = 1; i <= (argc - 1); i++) {
        if ((pid = fork()) < 0) {
            printf("Errore creazione figlio per l'indice %d\n", i);
            // chiudi file
            free(file_desc_arr);
            exit(-5);
        } else if (pid == 0) { /* codice figlio */

            printf("Sono il figlio %d di indice %d ed ho associato %d\n", getpid(), i, file_desc_arr[i]);
            while ((nread = read(file_desc_arr[i], &c, 1)) > 0) {
                if(c == '\n' && !legit){
                    legit = true;
                    //free(str);  //?
                    }
                if (cont==0 && !isdigit(c)){
                    legit = false;
                }
                if(!legit)
                    continue;
                str[i] = realloc(str[i], sizeof(char) * (cont+1));
                str[i][cont++] = c;
                //printf(" %d -> %c -> %d\n",getpid(), c, cont++);
                /*if (write(file_descriptor, buffer, nread) < nread) {
                    close(file_descriptor);
                    return 1;
                    }
                */
            }
            close(file_desc_arr[i]);
            if(cont <= 10){
                //free(str[i]);
                //printf("Lunghezza non rispettata: %d\n",cont);
                exit(-6);
            }
        str[i][cont] = '\0';
            printf("\nPID %d con \nlunghezza %d con indice %d e stringa  %s \n ", getpid(),cont, i,str[i]);
            exit(i);
        }
    } // fine for

    for (int i = 0; i < (argc - 1); i++) {
        pidFiglio = wait(&status);
        if (pidFiglio < 0) {
            printf("Errore in wait\n");
            // chiudi file
            free(file_desc_arr);
            exit(-6);
        }
        if ((status & 0xFF) != 0)
            printf("Figlio con pid %d terminato in modo anomalo\n", pidFiglio);
        else {
            ritorno = (int)((status >> 8) & 0xFF);
            if (ritorno == 255)
                puts("Errore!");
            else{
                if(ritorno == EXIT_SUCCESS){
                    printf("Indice Figlio Compatibile: %d\n\t File Desc. Corrispondente: %d ", ritorno, file_desc_arr[i]);
                }
                printf("Il figlio con pid=%d ha ritornato %d\n", pidFiglio,
                   ritorno);
            }
        }
    }
    free(file_desc_arr);
}