#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

// working test: ./out -numero < elem

int main(int argc, char**argv){

    if (argc != 2){
        puts("Errore numero parametri");
        exit(1);
    }

	int nread, param = argv[1][0];
	char c;
	param = param - '0';	/*	CONVERSIONE CHAR -> INT */
    /*
     *
     *   0 è STDIN
     *
     *   1 è STDOUT
	 *
     */
	printf("INSERITO %d\n", param);
	for(int j = 0; j < param;j++){
		//printf("\nlettura %d\n", j+1);
		while ((nread = read(0, &c, 1)) > 0){	// imposto un buffer di un carattere, così da identificare lo \n
			if(c=='\n'){
                write(1,"\n",1);
				break;
            }
			write(1,&c,1);
		}
	}
	return 0;
}
