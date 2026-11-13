pipe_file.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
int main()
{
    int pipe1[2], pipe2[2];      /* pipe1: parent -> child, pipe2: child -> parent */
    pid_t pid;
    char filename[256];
    char buffer[1024];
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("pipe");
        exit(1);
    }
    pid = fork();
    if (pid < 0) {
        printf("Fork Failed\n");
        exit(1);
    }
    if (pid > 0) {                                   /* PARENT */
        close(pipe1[0]);                             /* unused read end of pipe1  */
        close(pipe2[1]);                             /* unused write end of pipe2 */
        printf("Enter file name: ");
        fflush(stdout);
        scanf("%255s", filename);
        write(pipe1[1], filename, strlen(filename) + 1);
        close(pipe1[1]);
        printf("\nContents of File:\n");
        fflush(stdout);                              /* printf is buffered, write() is not */
        ssize_t n;
Operating Systems Lab - Complete Practical Guide (Programs, Commands, Viva) Page 25
        while ((n = read(pipe2[0], buffer, sizeof(buffer))) > 0)
            write(STDOUT_FILENO, buffer, n);         /* read until child closes pipe2 */
        close(pipe2[0]);
        wait(NULL);
    }
    else {                                           /* CHILD */
        close(pipe1[1]);
        close(pipe2[0]);
        read(pipe1[0], filename, sizeof(filename));
        close(pipe1[0]);
        FILE *fp = fopen(filename, "r");
        if (fp == NULL) {
            const char *err = "Error: Cannot open file.\n";
            write(pipe2[1], err, strlen(err));
        } else {
            size_t n;
            while ((n = fread(buffer, 1, sizeof(buffer), fp)) > 0)
                write(pipe2[1], buffer, n);          /* any file size works */
            fclose(fp);
        }
        close(pipe2[1]);
        exit(0);
    }
    return 0;
}
