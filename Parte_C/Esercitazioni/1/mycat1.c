

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#define PERM 0644



/*
 *  PRINTF  Per stampe dell utente, commenti a mo di echo, riguardo cosa accade nel programma
 *  WRITE   Si utilizza nei file
 *  SPRINTF Scrive su una stringa
 * 
 * 
 * 
 * 
*/

int main( int argc, char*argv[]){
    int file, nread;
    char buffer [BUFSIZ];
	
    
    if ( argc < 2 ){
		printf("inserire almeno un parametro, non %d\n",argc-1);
		return -1;
		}
    for (unsigned int i = 1; i < argc; i++){
    	if ((file = open( argv[i], O_RDONLY)) < 0){
            printf("\nerrore sul file %s\n", argv[i]);
            close(file);
            return -2;
        }
        while ( (nread = read (file, buffer, BUFSIZ) )> 0 );
        printf("\ncat del file %s: ", argv[i]);
        
        printf("\n%s", buffer);
        
        close (file);
        puts("");
    }
    
	return 0;
}
