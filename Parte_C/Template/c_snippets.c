// LIBRERIE UTILI
#include <fcntl.h>  // controllo file
#include <stdio.h>
#include <stdlib.h>
#include <string.h>    // strlen per char input, strcat per concatenare, strcpy, strcmp..
#include <sys/wait.h>  // child wait
#include <time.h>      // seed numeri casuali
#include <unistd.h>    // unix cmd

// CONTROLLARE NUMERO DI PARAMETRI

if (argc != 4) {  // 1+3, in questo caso devono essere 3
    puts("Errore nel numero di parametri");
    exit(1);
}

/*
 * Controllo che un file esista e si possa aprire
 * O_RDWR
 * O_RDONLY
 * O_WRONLY
 *
 */

int file_descriptor;

if ((file_descriptor = open(argv[1], O_RDWR)) < 0) {
    puts("Errore in apertura file");
    exit(1);
}


#define PERM 0644

char* filename ="test.txt";
fd_write = creat(filename, PERM);
if (fd_write < 0) {
    printf("Impossibile creare il file %s\n", filename );
    exit(-1);
}
close(fd_write);


/* Controllo che un param i-esimo sia dir assoluta */
if (argv[i][0] != '/') {
    printf("\nErrore con il path di indice \'%d\', non è un path assoluto.");
    exit(2);
}

/*
 * controllo che il secondo parametro sia una stringa
 * di lunghezza 1 (quindi che contenga
 * un singolo carattere); in alternativa si poteva verificare che
 * argv[2][1] fosse o meno != ‘\0’ cioè il carattere nullo!
 */

if (strlen(argv[2]) != 1) {
    puts("Errore non carattere");
    exit(2);
}

// LETTURA CON UN BUFFER

char buffer[BUFSIZ];

while ((nread = read(0, buffer, BUFSIZ)) > 0) {  // leggo da stdin
    /* si legge dallo standard input */
    if (write(outfile, buffer, nread) < nread) {
        close(outfile);
        return 1; /* errore */
    }
}

/*
    LETTURA CHAR-BY-CHAR
    0 STDIN
    1 STDOUT
    2 STDERR
*/
char c;
while ((ret = read(file_desc, &c, 1)) > 0) {
    write(1, &c, 1);
}

int foo(void) {
    int valore;
    if (fork() == 0) {
        printf("
               Funzioni utili:PID
               : % d\n GID
               : % d\n PPID
               : % d ",
                     getpid(),
                 getgid(), getppid());
        // codice figlio
        exit(valore);
    }
    /*  codice padre, non ha bisogno dell else  */
}

/*
 * una volta generato un figlio si può impostare l attesa del padre al figlio
 * nel ritorno in status nel byte alto abbiamo il valore di ritorno del child
 * nel basso dobbiamo avere zero
 * IN CASO invece un child termini con un segnale avremo nell alto zero e nel
 * basso il segnale con cui è terminato
 *
 * ritorna -1 in caso non si abbiano figli da attendere, altrimenti ritorna il
 * pid del child terminato
 *
 */
int status, pid;
pid = wait(&status);

int esempio_child_con_ritorno(void) {
    int pid, status, exit_s;

    if ((pid = fork()) < 0) { /* fork fallita */
        printf(”Errore in fork\n”);
        exit(1);
    }

    if (pid == 0) { /* figlio */
        printf(”Esecuzione del figlio\n”);
        sleep(4);
        exit(5);
    }

    /* padre */
    if (wait(&status) < 0) {
        printf(”Errore in wait\n”);
        exit(1);
    }

    if ((status & 0xFF) != 0)
        printf(”Errore in status\n”);
    else {
        exit_s = status >> 8;
        /* selezione degli 8 bit piu’ significativi */
        exit_s &= 0xFF;
        if(exit_s == 255)
            puts("Errore!");
        else
            printf(”Per il figlio % d lo stato di EXIT e % d\n”, pid, exit_s);
    }
    exit(0);
}

// LE PRIMITIVE EXEC

/*
    Alcune delle primitive della famiglia EXEC sono:
    execv (pathname, argv);
    execl (pathname, arg0, argv1, ..., argvn, (char *) 0);
    execvp (name, argv);
    execlp (name, arg0, argv1, ..., argvn, (char *) 0);


  execv ed execl hanno bisogno del pathname del programma da eseguire

  mentre execlp e execvp Il programma viene ricercato nella ENV VAR $PATH

    execv - argomenti in vettore
    execl - argomenti in lista
    execvp  - argomenti in vettore + path
    execlp  - argomenti in lista + path

    in più abbiamo execve e execle che ci permettono di specificare l ambiente

    v vettore
    l list
    p cerca nel $PATH
    e custom env

*/

// execl base

int execl_base() {
    printf(”Esecuzione di ls\n”);
    execl(”/ bin / ls”, ”ls”, ”- l”, (char*)0);
    printf(”Errore in execl\n”);
    exit(1);
}

// execv

int execv_base() {
    char* av[3];
    av[0] =”ls”;
    av[1] =”- l”;
    av[2] = (char*)0;
    printf(”Esecuzione di ls\n”);
    execv(”/ bin / ls”, av);
    printf(”Errore in execv\n”);
    exit(1);
}

// execvp

/* file callecho.c */
int execvp_base() {
    char* argin[4];
    argin[0] =”myecho”;
    argin[1] =”hello”;
    argin[2] =”world !”;
    argin[3] = (char*)0;
    printf(”Esecuzione di myecho\n”);
    execvp(argin[0], argin);
    printf(”Errore in execvp\n”);
    exit(1);
}
/* file myecho.c */
int main_myecho_c(int argc, char** argv) {
    int i;
    printf(”Sono myecho\n”);
    for (i = 0; i < argc; i++)
        printf(”Argomento argv[% d] = % s\n”, i, argv[i]);
}

// esempio execl + fork
int execl_con_fork() {
    int pid;
    pid = fork();
    if (pid == 0) { /* figlio */
        printf("Esecuzione di ls\n");
        execl("/bin/ls", "ls", "-l", (char*)0);
        printf("Errore in execl\n");
        exit(1);
    }
    if (pid > 0) { /* padre */
        wait((int*)0);
        printf("Terminato ls\n");
        exit(0);
    }
    if (pid < 0) { /* fork fallita */
        printf("Errore in fork\n");
        exit(1);
    }
}

/*  PIPES   */

// definire una pipe
typedef int pipe_t[2];
pipe_t* _pipe = malloc(sizeof(pipe_t) * n_figli);
free(_pipe);

// controllare che la creazione di una pipe abbia avuto successo
    if (pipe(piped) < 0) {
        printf("Errore creazione pipe\n");
        exit(2);
    }

