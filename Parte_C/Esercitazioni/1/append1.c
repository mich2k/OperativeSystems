#include <stdio.h>

#include <stdlib.h>

#include <unistd.h>

#include <fcntl.h>

#define PERM 0644

// ESERCIZIO PRESO DALLE SOLUZIONI, mi scocciavo


/* in UNIX */
int appendfile(char * f1) {
  int outfile, nread;
  /* oufile per file descriptor file su cui
appendere, nread numero caratteri letti */
  char buffer[BUFSIZ];
  /* buffer per la lettura/scrittura */
)
if ((outfile = open(f1, O_WRONLY | O_APPEND | O_CREAT, PERM)) < 0
  /* apertura in append o creazione */
  return 1;
  while ((nread = read(0, buffer, BUFSIZ)) > 0)
  /* si legge dallo standard input */
  {
    Sistemi Operativi e Lab.(Prof.ssa Letizia Leonardi)
  }
  if (write(outfile, buffer, nread) < nread) {
    close(outfile);
    return 2; /* errore */
  }
} /* fine del file di input */
close(outfile);
return (0);
int main(int argc, char ** argv) {
  int integi;
  /* variabile per valore di ritorno della funzione
appenfile */
  if (argc != 2) /* controllo sul numero di argomenti */ {
    printf("ERRORE: ci vuole un argomento \n");
    exit(3);
  }
  integi = appendfile(argv[1]);
  exit(integi);
}
