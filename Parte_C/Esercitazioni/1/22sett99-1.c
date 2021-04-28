/*
Si progetti, utilizzando il linguaggio C e le primitive di basso livello che operano sui file, un
filtro che accetta due parametri: il primo parametro deve essere il nome di un file (F), mentre il
secondo deve essere considerato un singolo carattere (C). Il filtro deve operare una modifica del
contenuto del file F: in particolare, tutte le occorrenze del carattere C nel file F devono essere
sostituite con il carattere spazio.
*/
#include <stdio.h>

#include <stdlib.h>

#include <unistd.h>

#include <fcntl.h>

#include <string.h>

int main(int argc, char ** argv) {
  int fd;
  char c;
  /* <== usiamo un singolo carattere dato che faremo una lettura carattere per carattere, dato che dobbiamo
  verificare se abbiamo trovato il carattere cercato!*/
  if (argc != 4) {
    puts("Errore nel numero di parametri");
    exit(1);
  }
  if ((fd = open(argv[1], O_RDWR)) < 0)
  /*<== NOTA BENE: l’apertura la dobbiamo fare in LETTURA E SCRITTURA,
dato che dobbiamo leggere per cercare il carattere e quindi dobbiamo poi scrivere!*/
  {
    puts("Errore in apertura file");
    exit(2);
  }
  if (strlen(argv[2]) != 1){
  /*<== controllo che il secondo parametro sia una stringa di lunghezza 1 (quindi che contenga
un singolo carattere); in alternativa si poteva verificare che argv[2][1] fosse o meno != ‘\0’ cioè il carattere nullo!*/
    puts("Errore non carattere");
    exit(3);
  }
    if (strlen(argv[3]) != 1){
    puts("Errore non carattere");
    exit(4);
  }
  while (read(fd, &c, 1)){
    /* <== lettura di un singolo carattere alla volta; N.B. il controllo sul fatto se siamo arrivati
    all’end-of-file logico del file viene effettuato da UNIX sulla base del valore del file pointer (che si trova nell’elemento della Tabella dei File
    Aperti di Sistema, riferito dall’elemento di posto fd della Tabella dei File Aperti del processo) rispetto alla lunghezza del file (che si trova nella
    copia dell’i-node caricato nella tabella degli INODE Attivi di Sistema)*/
    if (c == argv[2][0]) {//<== ATTENZIONE: argv[2][0] è il carattere che dobbiamo cercare {
       lseek(fd, -1L, SEEK_CUR);
        /* SI DEVE RIPORTARE INDIETRO IL FILE POINTER <== NOTA BENE: quando verifichiamo che abbiamo
        trovato il carattere il file pointer è già avanzato ed è sul carattere successivo e quindi dobbiamo tornare indietro di 1 (-1L) rispetto alla
        posizione corrente (1 o SEEK_CUR)!*/
        write(fd, argv[3], 1);

        /*
         *  Per il blank basta fare ./out c \
         *
         *
        */

    }

}
  return 0;
}
