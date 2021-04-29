#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
int main( int argc, char** argv){
    bool found = false;
    char c;
    int file_desc;
    if (argc!=3){
        puts("Errore numero parametri");
        exit(1);
    }

    if ((file_desc = open(argv[1], O_RDONLY)) < 0){
        puts("Errore apertura del file");
        exit(2);
    }

    // TODO: aggiungere controllo che confronta 'n' con $(wc -l)

    long n = argv[2][0] - '0';

    if( ! ( n > 0 ) ){
        puts("Errore, n deve essere strettamente positivo");
        exit(3);
    }

    for ( unsigned long rows = 1; found != true; ++rows){
        while ((read(file_desc, &c, 1)) > 0){
            if(c=='\n')
                break;
            if(rows==n){
                printf("%c",c);
                found = true;
            }

        }
    }
    puts("");
    return 0;
}
