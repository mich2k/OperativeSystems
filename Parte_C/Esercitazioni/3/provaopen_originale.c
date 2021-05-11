#include <fcntl.h>  //<== deve essere incluso per poter usare le costanti per la open (O_RDONLY, O_WRONLY e O_RDWR)
#include <stdio.h>  //<== deve essere incluso per poter usare printf e puts
#include <stdlib.h>  //<== deve essere incluso per poter usare la primitiva exit
#include <unistd.h>  //<== deve essere incluso per poter usare la primitiva close
int main(int argc, char** argv) {
    int fd1, fd2, fd3;
    if (argc != 2) {
        puts("Errore nel numero di parametri");
        /* <= = per prima cosa controlliamo il numero di parametri
             : vogliamo avere un numero uguale a 1(
                   il nome del file che vogliamo aprire)
                   e quindi il numero dei parametri deve essere 1+1(nome del
           comando stesso) = 2*/
        exit(1);
    }
    /*<= = in caso di errore,
    usiamo la primitiva exit con un numero diverso ogni volta(
        in modo assolutamente analogo a quanto veniva fatto nella
            shell) !*/
    if ((fd1 = open(argv[1], O_RDONLY)) < 0) {
        /* <= = invochiamo la open e controlliamo se il valore di ritorno è
     minore di 0; FARE MOLTA ATTENZIONE ALLE PARENTESI INDICATE !Unix
     controllerà i diritti sul file del processo in esecuzione controllando UID
     e GID effettivi(che si trovano nel descrittore di processo) nei confronti
     di UID e GID del file(che si trovano nell’inode)*/
        puts("Errore in apertura file");
        exit(2);
    } else
        printf("Valore di fd1 = %d\n",
               fd1);  //<== stampiamo il valore ritornato dalla

    if ((fd2 = open(argv[1], O_RDONLY)) < 0) { /*<= = NOTA BENE
     : usiamo lo stesso nome di file;
 verrà occupato un ulteriore elemento libero della TFA dei processo e un
     ulteriore elemento della TFA di sistema;
 sia questo elemento che il precedente(derivante dalla precedente open)
         farà riferimento all’unico elemento della Tabella degli I -
     NODE attivi che contiene la copia dell’I -
     NODE del file il cui nome è argv[1]*/
        puts("Errore in apertura file");
        exit(2);
    } else
        printf("Valore di fd2 = %d\n", fd2);
    close(fd1);  //<= = chiudiamo il primo file aperto
    if ((fd3 = open(argv[1], O_RDONLY)) <
        0) {  //<== NOTA BENE: stessa nota di cui sopra !{
        puts("Errore in apertura file");
        exit(2);
    } else
        printf("Valore di fd3 = %d\n",
               fd3);  //<= = verifichiamo il valore ritornato dalla open !
    return 0;
    /* <= = ritorniamo 0 che è il valore di successo in UNIX ! <= = NOTA BENE:
     * Equivalente a exit(0);*/
}