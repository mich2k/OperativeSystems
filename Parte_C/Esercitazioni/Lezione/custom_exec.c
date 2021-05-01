#include <stdio.h>
main(int argc, char** argv) {
    int stato, atteso, pid;
    char st[80];
    for (;;)
    /* ciclo infinito */
    {
        if ((pid = fork()) < 0) {
            perror("fork");
            exit(1);
        }
        if (pid == 0) { /* FIGLIO: esegue i comandi */
            printf("inserire il comando da eseguire:\n");
            scanf("%s", st);
            execlp(st, st, (char*)0);
            perror("errore");
            exit(0);
        } else {
            /* PADRE */
            atteso = wait(&stato);
            /*attesa figlio: sincronizzazione */
            printf("eseguire altro comando? (si/no) \n");
            scanf("%s", st);
            if (strcmp(st, "si"))
                exit(0);
        }
    }
}