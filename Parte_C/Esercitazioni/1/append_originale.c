#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#define PERM 0644
/* in UNIX */

int appendfile(char * f1) {


  int outfile, nread;
  char buffer[BUFSIZ];
  if ((outfile = open(f1, O_WRONLY)) < 0){
  /* apertura in scrittura */
    if ((outfile = creat(f1, PERM)) < 0)	 /* se il file non esiste, viene creato */
		return 1;	// errore nella creazione
  } else
	  lseek(outfile, 0L, SEEK_END); //<== Nota bene: si deve usare il valore 0 indicato come LONG INTEGER e quindi 0 L!

    /* se il file esiste, ci si posiziona alla fine */

    while ((nread = read(0, buffer, BUFSIZ)) > 0){	// leggo da stdin
	/* si legge dallo standard input */
    if (write(outfile, buffer, nread) < nread) {
      close(outfile);
      return 2; /* errore */
    }
  } /* fine del file di input */
  close(outfile);
  return 0;
}

int main(int argc, char ** argv) {
  int integi;
  if (argc != 2) /* controllo sul numero di argomenti */ {
    printf("ERRORE: ci vuole un argomento \n");
    exit(3);
  }
  integi = appendfile(argv[1]);
  exit(integi);
}
