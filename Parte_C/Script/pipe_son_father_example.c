

/*

    PER COMUNICARE TRA PADRE - FIGLIO:
        - Il padre crea la pipe prima di creare il figlio
        - Il padre crea il figlio
        - Per stabilire una comunicazione uni-direzionale è
             necessario che il padre chiuda la lettura e il figlio chiuda la scrittura
            
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fstab.h>

typedef int piped;

int main(int argc, char** argv){
    int pid;
    piped pipe_[2];
    //int M_SIZE = 50;
    char msg[6];
    // creazione pipe
    pipe(pipe_);

    if((pid==fork()) == 0){
        close(pipe_[0]); // chiudo il figlio in lettura
        write(pipe_[1], "piazza", 6);   // se commento questa write andrà in read == 0
        exit(0);        
    }
    // padre
    close(pipe_[1]);    // chiude scrittura

    if(read(pipe_[0], msg, 6) != 0){
        printf("msg: \"%s\"\n", msg);
    }
    else
        puts("Non c'è scrittore!");
    return EXIT_SUCCESS;

}