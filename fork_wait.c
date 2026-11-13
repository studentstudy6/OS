#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
/* Parent uses Bubble Sort */
void bubbleSort(int arr[], int n)
{
    int i, j, temp;
    for (i = 0; i < n - 1; i++)
        for (j = 0; j < n - i - 1; j++)
            if (arr[j] > arr[j + 1]) {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
}
/* Child uses Insertion Sort */
void insertionSort(int arr[], int n)
{
    int i, j, key;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}
void printArray(int arr[], int n)
{
    int i;
    for (i = 0; i < n; i++)
        printf("%d ", arr[i]);
Operating Systems Lab - Complete Practical Guide (Programs, Commands, Viva) Page 6
    printf("\n");
}
int main()
{
    int n, i;
    printf("Enter number of elements: ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Invalid input\n");
        return 1;
    }
    int arr[n];
    printf("Enter elements:\n");
    for (i = 0; i < n; i++)
        scanf("%d", &arr[i]);
    fflush(stdout);              /* avoid duplicated buffered output after fork() */
    pid_t pid = fork();
    if (pid < 0) {
        printf("Fork Failed\n");
        exit(1);
    }
    else if (pid == 0) {         /* CHILD */
        printf("\nCHILD PROCESS (Insertion Sort)\n");
        insertionSort(arr, n);
        printf("Sorted Array by Child : ");
        printArray(arr, n);
        printf("Child PID  = %d\n", getpid());
        printf("Parent PID = %d\n", getppid());
        exit(0);
    }
    else {                       /* PARENT */
        bubbleSort(arr, n);      /* parent sorts ...             */
        wait(NULL);              /* ... and waits for the child  */
        printf("\nPARENT PROCESS (Bubble Sort)\n");
        printf("Sorted Array by Parent: ");
        printArray(arr, n);
        printf("Parent PID = %d\n", getpid());
    }
    return 0;
}

zombie.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main()
{
    pid_t pid = fork();
    if (pid < 0) {
        printf("Fork Failed\n");
        exit(1);
    }
    else if (pid == 0) {                 /* CHILD: exits immediately */
        printf("Child Process Terminated (PID = %d)\n", getpid());
        exit(0);
    }
    else {                               /* PARENT: never calls wait() */
        printf("Parent Sleeping... (PID = %d, Child PID = %d)\n", getpid(), pid);
        fflush(stdout);
        sleep(20);                       /* child is a ZOMBIE during this time */
        printf("Parent Wakes Up\n");
    }
    return 0;
}

orphan.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main()
{
    pid_t pid = fork();
    if (pid < 0) {
        printf("Fork Failed\n");
        exit(1);
    }
    else if (pid > 0) {                  /* PARENT: exits first */
        printf("Parent Process Exiting (PID = %d)\n", getpid());
        exit(0);
    }
    else {                               /* CHILD: becomes an orphan */
        sleep(5);
        printf("Child Process Running\n");
        printf("Child PID      = %d\n", getpid());
        printf("New Parent PID = %d\n", getppid());   /* 1 (init/systemd) */
    }
    return 0;
}
