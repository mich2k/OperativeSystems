

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#define PERM 0644


#include <string.h>
// BUFSIZ è standard 512

int main( int argc, char*argv[]){
    int file;
	if ( argc != 3 ){
		printf("inserire 2 parametri, non %d\n",argc-1);
		return -1;
		}
	// provo ad aprire il file in scrittura, se non riesce stampo errore sul file
	if ((file = open( argv[1], O_RDWR)) < 0){
		printf("\nerrore sul file\n");
		return -2;
	}

	if ( strlen(argv[2]) != 1){
		printf("il secondo parametro deve essere un carattere singolo");
		return -3;
	}
	
    int nread;
    char buffer [BUFSIZ];
    
    unsigned int i = 0, cont_occ=0;
    while ( (nread = read(file,buffer,BUFSIZ))>0);
    
    close (file);
    
    while(buffer[i] != 0){
       if( !(isprint(buffer[i])))
       {   i++;
           continue;
           
        }
        printf("\n\nesamino \'%c\'", buffer[i]);
        if( buffer[i]=='a')
            cont_occ++;
        i++;
    }
	puts("");
    printf("\nSono state trovate \'%u\' occorrenze di \'%s\' in \'%s\'",cont_occ,argv[2],argv[1]);
/*	printf("controlli passati con successo\n filename: %s\n numero parametri: %d", argv[0], argc);
	for(unsigned int i = 0; i < argc; i++)
		printf("\nparametro %d: %s", i, argv[i]);
*/
	return 0;
}
