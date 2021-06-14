#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#define PERM 0644

typedef int pipe_t[2];

// secondo i file correnti, test fatto: ./main *.txt

int main(int argc, char** argv) {
    if (!(argc >= 2)) {
        printf("\nNumero di parametri non conforme, richiesto uno, non %d.",
               argc - 1);
        exit(1);
    }

    int H = argc - 1;
    int createdfile, pidSpeciale, pidFiglio, pid = 0, ritorno, status;
    int file_descriptor;  // questo lo uso negli H figli per le open
    pipe_t padreFiglioSpeciale;
    char c;  // char temporaneo, avrei potuto anche fare &buff[h]
    pipe_t* figlioPadre = NULL;
    figlioPadre = malloc(sizeof(pipe_t) * H);

    if (!figlioPadre) { // controllo che il puntatore non sia null
        printf("\nErrore malloc\n");
        exit(2);
    }

    // printf("\nIl parametro e\': %d\n", H); DEBUG

    if (pipe(padreFiglioSpeciale) < 0) {    // controllo il ritorno della pipe per il figlio speciale
        printf("\nErrore creazione pipe padre-figlio speciale\n");
        exit(3);
    }

    if ((createdfile = creat("/tmp/michelegiarletta", PERM)) <
        0) {      // creo il file con i permessi definiti
        exit(4);  // errore nella creazione, esco
    }

    for (int h = 0; h < H; h++) {   // creo tutte le H-pipe necessarie e ne controllo il ritorno
        if (pipe(figlioPadre[h]) < 0) {
            printf("\nErrore creazione H pipe figli \"normali\" con indice %d",
                   h);
            exit(5);
        }
    }

    if ((pidSpeciale = fork()) < 0) {   // genero il figlio speciale e ne controllo il ritorno
        printf("\nErrore creazione figlio speciale");
        exit(6);
    } else if (pidSpeciale == 0) {
        close(0);  // redirigo lo stdint (ovvero 0)
        if (open(argv[1], O_RDONLY) < 0) {
            printf("Errore nella open del file %s\n", argv[1]);
            exit(-1); /* decidiamo di tornare -1 che verra' interpretato dal
                         padre come 255 e quindi un valore unlegit! */
        }
        /* deve chiudere lo standard output e quindi usare la
         * dup sul lato di scrittura della propria pipe */
        close(1);
        dup(padreFiglioSpeciale[1]);
        /* il figlio speciale Ha ambo i lati chiusi, siccome il lato scrittura è
         * lo stdout */
        close(padreFiglioSpeciale[0]);
        close(padreFiglioSpeciale[1]);

        /* Il figlio speciale  diventa il comando wc -l */
        execlp("wc", "wc", "-l", (char*)0); // il primo è il nome per standard, l ultimo è il segnale per far capire che è terminato

        // se la exec arriva qui, signfiica che c\è un errore
        exit(-1); /* torno un valore diverso da zero per indicare insuccesso,
                     questo verrà interpretato come 255, sinonimo di errore */
    }

    // codice padre

    // chiudo il lato scrittura che non serve al padre
    close(padreFiglioSpeciale[1]);
    char rit[10];
    int lunghezza = 0;
    int j = 0;  // conta i caratteri, ho posto lunghezza max 10 nell array
                // siccome per un int è più che sufficiente
    while (read(padreFiglioSpeciale[0], &(rit[j]), 1)) {
        /*
            printf("indice j= %d carattere letto da pipe %c\n", j, rit[j]);

        */
        j++;
    }
    // vado a stampare il ritorno, nonchè la lunghezza del file.
    // siccome è in char[] va convertita in intero
    lunghezza = atoi(rit);

    printf("\nIl figlio speciale ha tornato %d\n", lunghezza);

    // wait padre per figlio speciale

    printf("\n\n\n");
    pidSpeciale = wait(&status);
    if (pidSpeciale < 0) {
        printf("Errore in wait padre\n");
        exit(7);
    }
    if ((status & 0xFF) != 0)
        printf("Figlio speciale con pid %d terminato in modo anomalo\n", pidSpeciale);
    else {
        ritorno = (int)((status >> 8) & 0xFF);
        if (ritorno == 255)
            printf(
                "Il figlio speciale con pid=%d ha ritornato %d e quindi vuole "
                "dire che il figlio ha avuto dei problemi\n",
                pidSpeciale, ritorno);
        else
            printf("Il figlio speciale con pid=%d ha ritornato %d\n", pidSpeciale,
                   ritorno);
    }

    // figli normali

    for (int h = 0; h < H; h++) {
        if ((pid = fork()) < 0) {  // vengono generati
            printf("\nErrore creazione figlio con indice %d\n", h);
            exit(8);
        } else if (pid == 0) {  // codice figlio

            int count = 0, rit = 0;
            char store[240];

            close(padreFiglioSpeciale[0]);
            close(padreFiglioSpeciale[1]);

            for (int k = 0; k < H; k++) {
                close(figlioPadre[k][0]);
                if (h != k) {
                    close(figlioPadre[k][1]);
                }
            }

            if ((file_descriptor = open(argv[h + 1], O_RDONLY)) < 0) {
                puts("Errore in apertura file");
                exit(-1);
            }

            while (read(file_descriptor, &c, 1) != 0) {
                store[count++] = c;
                if (c == '\n') {
                    // avendo il count++ prima dell if sono sicuro che includerà
                    // anche il terminatore nel conteggio. comunico al padre
                    // prima la lunghezza della linea trovata:
                    store[count] = '\0';
                    if (write(figlioPadre[h][1], &count, sizeof(int)) !=
                        sizeof(int)) {
                        printf("\nErrore read indice %d", h);
                        exit(-1);
                    }

                    // converto in stringa
                    store[count] = '\0';

                    if (write(figlioPadre[h][1], &store,
                              sizeof(char) * count) == -1) {
                        printf("\nErrore write su indice %d\n", h);
                        exit(-1);
                    }

                    rit = count; // salvo sempre l ultimo count, così se la linea sarà effettivamente l ultima lo salvo
                    count = 0;
                }
            }
            exit(rit - 1);  // Il ritorno NON deve comprendere il terminatore
        }
    }

    // codice padre

    int ritorno_lun = 0;
    char store[240];

    close(padreFiglioSpeciale[0]);
    close(padreFiglioSpeciale[1]);

    for (int h = 0; h < H; h++) {
        close(figlioPadre[h][1]);
    }

    // con i due cicli innestati, esternamente ho la lunghezza in lineaa
    // in questo modo vado a scrivere prima tutte l-esime linee per ogni figlio
    // e poi procedo con la linea incrementandola

    for (int l = 0; l < lunghezza; l++) {
        for (int h = 0; h < H; h++) {
            if (read(figlioPadre[h][0], &ritorno_lun, sizeof(int)) == -1) {
                printf("\nErrore read padre indice %d", h);
                exit(9);
            }

            // printf("\nFiglio indice %d ha tornato len %d", h, ritorno_lun);

            if (read(figlioPadre[h][0], &store, sizeof(char) * ritorno_lun) ==
                -1) {
                printf("\nErrore read padre indice %d", h);
                exit(10);
            }

            //printf("\nDEBUG: Letto %d-esima linea dall %d-esimo indice: %s",l,h,store);

            if (write(createdfile, &store, sizeof(char) * (ritorno_lun)) ==
                -1) {
                printf("\nErrore write padre indice %d\n", h);
                exit(11);
            }
        }
        // printf(
        //     "\nDEBUG: Sono state scritte tutte le %d-esime linee per tutti i figli in "
        //     "modo ordinato.\n",
        //     l); 
    }

    printf("\nÈ stato tutto scritto sul file: \"/tmp/michelegiarletta\"\n");

    // wait padre per i figli normali
    for (int h = 0; h < H; h++) {
        pidFiglio = wait(&status);
        if (pidFiglio < 0) {
            printf("Errore in wait\n");
            exit(12);
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