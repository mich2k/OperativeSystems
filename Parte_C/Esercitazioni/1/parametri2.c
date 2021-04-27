

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#define PERM 0644


#include <string.h>

int main( int argc, char*argv[]){
	int outfile;
	if ( argc != 4 ){
		printf("inserire 3 parametri, non %d\n",argc-1);
		return -1;
		}
	// provo ad aprire il file in scrittura, se non riesce stampo errore sul file
	if ((outfile = open( argv[1], O_WRONLY)) < 0){
		printf("\nerrore sul file\n");
		return -2;
	}
	//else
	//	printf("file %s rilevato correttamente", argv[1]);
	if ( !(argv[2] > 0) ){
		printf("il secondo parametro deve essere un numero strettamente positivo");
		return -3;
	}
	if ( strlen(argv[3]) != 1){
		printf("il terzo parametro deve essere un carattere singolo");
		return -4;
	}
	puts("");
	printf("controlli passati con successo\n filename: %s\n numero parametri: %d", argv[0], argc);
	for(unsigned int i = 0; i < argc; i++)
		printf("\nparametro %d: %s", i, argv[i]);

	return 0;
}