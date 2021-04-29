#include <stdio.h>


int main(int argc, char *argv[]){
	//printf("%d", argc);
	if ( argc < 2 ){
		printf("\ninserire almeno un parametro, non %d\n", argc - 1);
		return 1;
		}

	printf("\nNome programma: %s", argv[0]);
	for (unsigned int i = 0; i < argc; i++)
		printf("\nNella posizione %d: %s",i,argv[i]);
	puts("");
	return 0;
}