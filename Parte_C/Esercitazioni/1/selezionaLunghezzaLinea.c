#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>

int main( int argc, char** argv ){

    char c;
    int file_desc, comp_rows = 0, file_desc_holder;
    if (argc!=3){
        puts("Errore numero parametri");
        exit(1);
    }

    if ((file_desc = open(argv[1], O_RDONLY)) < 0){
        puts("Errore apertura del file");
        exit(2);
    }

    long n = argv[2][0] - '0';

    if( ! ( n > 0 ) ){
        puts("Errore, n deve essere strettamente positivo");
        exit(3);
    }

    int found_chars =0, rows = 0,ret=-1;
    bool found = false;
    while (ret != 0){       // 0 IS THE LOGIC EOF RETURNED BY EOF
        rows++;
        found_chars=0;
       while ((ret=read(file_desc, &c, 1)) > 0){
            found_chars++;
            if(c == '\n')
                break;
        }
        //printf("la linea %d ha %d caratteri\n", rows, found_chars);
        if(found_chars == n){
            printf("Riga \'%d\' compatibile:\n",rows);
            //file_desc=file_desc_holder;
            lseek(file_desc, -n, SEEK_CUR);
            while(read(file_desc, &c,1) > 0){
                if (c == '\n')
                    break;
                write(1,&c,1);
            }
            puts("");
        }
    }
    if(!found)
        puts("Non e\' stata trovata alcuna linea compatibile");
    return 0;
}
