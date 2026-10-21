#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int pid;
    pid = fork();

    if (pid < 0) {
        printf("Fork failed\n");
    }
    else if (pid == 0) {
        printf("Parent Process ID: %d\n", getppid());
        printf("Child Process ID: %d\n", getpid());
    }
    else {
        wait(NULL);
    }
    return 0;
}
