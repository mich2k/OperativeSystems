#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>

int main(int argc, char** argv){

    if(argc!=3){
     puts("Errore numero parametri");
     exit(1);
    }

    int file_desc, cont = 0,n, nread;
    char c;
    if ((file_desc = open(argv[1], O_RDONLY)) < 0){
        puts("Errore apertura del file");
        exit(2);
    }
    n = *argv[2] -'0';
    if (n <= 0){
        printf("Errore: %d deve essere un numero strettamente positivo", *argv[2]);
        exit(3);
    }
    while ((nread = read(file_desc, &c, 1)) > 0){	// imposto un buffer di un carattere, così da identificare lo \n
        if(cont % n == 0 && isprint(c)){
            printf("\nIl carattere multiplo in posizione %d, ovvero %c\n", cont, c);
        }
        cont++;
    }
}
