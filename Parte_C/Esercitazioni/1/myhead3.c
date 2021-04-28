#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdbool.h>

// working test: ./out -numero < elem

int main(int argc, char**argv){

    int nread, param, file_desc = 0; // standard sarà stdin
    char c;
	printf("--> %d\n", argc);
    if (argc > 3){
        puts("Errore numero parametri");
        exit(1);
    }

	if (argc==2){	// ./out 2 < test
		puts("Modalità filtro con parametro -numero unico");
		param = argv[1][0];
		param = param - '0';
	}else{
		if (argc==1){
				puts("Modalità filtro senza parametri");
				param=10;
		}else{
			if ((file_desc = open(argv[2], O_RDONLY)) < 0){	// O_RDONLY, O_WRONLY, O_RDWR
				puts("Errore in apertura file");
				exit(2);
			}
			param = argv[1][0];
			param = param - '0';

		}

		}
	printf("\nConsidero quindi le prime %d linee\n", param);
	for(int j = 0; j < param;j++){
		//printf("\nlettura %d\n", j+1);
		while ((nread = read(file_desc, &c, 1)) > 0){	// imposto un buffer di un carattere, così da identificare lo \n
			if(c=='\n'){
                write(1,"\n",1);
				break;
            }
			write(1,&c,1);
		}
	}
	return 0;
}
