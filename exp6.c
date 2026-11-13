/* Process 1 : sends sentences through FIFO1, shows result received on FIFO2 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#define FIFO1 "fifo1"
#define FIFO2 "fifo2"
#define SIZE  4096
int main()
{
    int fd1, fd2;
    char msg[SIZE] = "";
    char line[512];
    char result[SIZE];
    /* both programs create the FIFOs, so start order does not matter */
    if (mkfifo(FIFO1, 0666) == -1 && errno != EEXIST) { perror("mkfifo1"); exit(1); }
    if (mkfifo(FIFO2, 0666) == -1 && errno != EEXIST) { perror("mkfifo2"); exit(1); }
    printf("Enter sentences (empty line to finish):\n");
    while (fgets(line, sizeof(line), stdin) != NULL && line[0] != '\n') {
        if (strlen(msg) + strlen(line) < sizeof(msg))
            strcat(msg, line);
    }
    fd1 = open(FIFO1, O_WRONLY);              /* blocks until Process 2 opens */
    write(fd1, msg, strlen(msg) + 1);
    close(fd1);
    fd2 = open(FIFO2, O_RDONLY);
    ssize_t r = read(fd2, result, sizeof(result) - 1);
    if (r < 0) r = 0;
    result[r] = '\0';
    printf("\nOutput received from Process 2:\n%s\n", result);
    close(fd2);
    return 0;
}
process2.c
/* Process 2 : counts characters, words, lines; saves to file; sends file back */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <sys/stat.h>
#define FIFO1 "fifo1"
#define FIFO2 "fifo2"
#define SIZE  4096
int main()
{
    int fd1, fd2, i;
    char msg[SIZE];
    char result[SIZE];
    int chars = 0, words = 0, lines = 0, inWord = 0;
    if (mkfifo(FIFO1, 0666) == -1 && errno != EEXIST) { perror("mkfifo1"); exit(1); }
    if (mkfifo(FIFO2, 0666) == -1 && errno != EEXIST) { perror("mkfifo2"); exit(1); }
    fd1 = open(FIFO1, O_RDONLY);              /* blocks until Process 1 writes */
    ssize_t r = read(fd1, msg, sizeof(msg) - 1);
    if (r < 0) r = 0;
    msg[r] = '\0';
    close(fd1);
    for (i = 0; msg[i] != '\0'; i++) {
        if (msg[i] == '\n') {
            lines++;                          /* newline is not counted as a character */
        } else {
            chars++;
        }
        if (isspace((unsigned char)msg[i]))
            inWord = 0;
        else if (!inWord) {
            inWord = 1;
            words++;                          /* start of a new word */
        }
    }
    FILE *fp = fopen("result.txt", "w");
    if (fp == NULL) { perror("result.txt"); exit(1); }
    fprintf(fp, "Number of Characters = %d\n", chars);
    fprintf(fp, "Number of Words = %d\n", words);
    fprintf(fp, "Number of Lines = %d\n", lines);
    fclose(fp);
    fp = fopen("result.txt", "r");
    size_t len = fread(result, 1, sizeof(result) - 1, fp);
    result[len] = '\0';                       /* fread does not add '\0' */
    fclose(fp);
    fd2 = open(FIFO2, O_WRONLY);
    write(fd2, result, len + 1);
    close(fd2);
    return 0;
}
