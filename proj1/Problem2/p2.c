#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int n = atoi(argv[1]);
    char cmd[256] = "";
    int pids[n];
    double max_t = 0.0, min_t = 1e9, total_t = 0.0;
    
    int pipes[n][2];
    for (int i = 0; i < n; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("Pipe failed");
            return 1;
        }
    }

    for (int j = 2; j < argc; j++) {
        strcat(cmd, argv[j]);
        strcat(cmd, " ");
    }
    strcat(cmd, "> /dev/null 2>&1");
    
    for (int i = 0; i < n; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("Fork failed");
            return 1;
        }

        if (pid == 0) {
            pid_t g_pid = fork();
            if (g_pid < 0) {
                perror("Fork failed");
                return 1;
            }
            close(pipes[i][0]);
            struct timeval start, end;
            gettimeofday(&start, NULL);
            if (g_pid == 0) {
                execlp("/bin/sh", "/bin/sh", "-c", cmd, NULL);
                perror("Could not execve");
                exit(1);
            } else {
                wait(NULL);  
                gettimeofday(&end, NULL);
                double exec_time = (end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_usec - start.tv_usec) / 1000.0;
                write(pipes[i][1], &exec_time, sizeof(exec_time));
                close(pipes[i][1]);
            }
            exit(0);
        } else {
            pids[i] = pid;
        }
    }            

    int status;
    pid_t child_pid;
    while ((child_pid = wait(&status)) > 0) {
        for (int i = 0; i < n; i++) {
            if (pids[i] == child_pid) {
                double elapsed;
                read(pipes[i][0], &elapsed, sizeof(elapsed));
                if (elapsed >= max_t) max_t = elapsed;
                if (elapsed <= min_t) min_t = elapsed;
                total_t += elapsed;
                printf("Child %d Executed in %.2f millis\n", i + 1, elapsed);
            }
        }
    }

    printf("\nMax: %.2f millis\n", max_t);
    printf("Min: %.2f millis\n", min_t);
    printf("Average: %.2f millis\n", total_t / n);

    return 0;
}
