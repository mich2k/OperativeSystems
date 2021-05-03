#include <stdio.h>
#include <unistd.h>

int main(void) {

	printf("UNO: PID = %d, UID = %d, GID = %d\n", getpid(), getuid(), getgid());
	fork();
	printf("DUE: PID = %d, UID = %d, GID = %d\n", getpid(), getuid(), getgid());
	exit(0);
}
