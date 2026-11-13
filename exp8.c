//sstf.c
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
int main()
{
    int n, head, i, j;
    int req[100];
    int visited[100] = {0};
    int totalSeek = 0;
    printf("Enter number of requests: ");
    scanf("%d", &n);
    printf("Enter request queue:\n");
    for (i = 0; i < n; i++)
        scanf("%d", &req[i]);
    printf("Enter initial head position: ");
    scanf("%d", &head);
    printf("\nSeek Sequence: %d", head);
    for (i = 0; i < n; i++) {
        int min = INT_MAX;
        int index = -1;
        for (j = 0; j < n; j++) {
            if (!visited[j]) {
                int dist = abs(head - req[j]);
                if (dist < min) {
                    min = dist;
                    index = j;
                }
            }
        }
        visited[index] = 1;
        totalSeek += min;
        head = req[index];
        printf(" -> %d", head);
    }
    printf("\nTotal Seek Time = %d\n", totalSeek);
    return 0;
}


//scan.c
#include <stdio.h>
#include <stdlib.h>
void sort(int arr[], int n)
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
int main()
{
    int req[100], n, head, diskSize;
    int i, totalSeek = 0;
    printf("Enter number of requests: ");
    scanf("%d", &n);
    printf("Enter requests:\n");
    for (i = 0; i < n; i++)
        scanf("%d", &req[i]);
    printf("Enter initial head position: ");
    scanf("%d", &head);
    printf("Enter disk size: ");
    scanf("%d", &diskSize);
    sort(req, n);
    printf("\nSeek Sequence: %d", head);
    int pos = 0;                       /* first request >= head */
    while (pos < n && req[pos] < head)
        pos++;
    /* 1. move towards higher cylinders (away from spindle) */
    for (i = pos; i < n; i++) {
        totalSeek += abs(head - req[i]);
        head = req[i];
        printf(" -> %d", head);
    }
    /* 2. go to the last cylinder of the disk */
    totalSeek += abs(head - (diskSize - 1));
    head = diskSize - 1;
    printf(" -> %d", head);
    /* 3. reverse direction and service the remaining requests */
    for (i = pos - 1; i >= 0; i--) {
        totalSeek += abs(head - req[i]);
        head = req[i];
        printf(" -> %d", head);
    }
    printf("\nTotal Seek Time = %d\n", totalSeek);
    return 0;
}

//clook.c
#include <stdio.h>
#include <stdlib.h>
void sort(int arr[], int n)
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
int main()
{
    int req[100], n, head;
    int i, totalSeek = 0;
    printf("Enter number of requests: ");
    scanf("%d", &n);
    printf("Enter requests:\n");
    for (i = 0; i < n; i++)
        scanf("%d", &req[i]);
    printf("Enter initial head position: ");
    scanf("%d", &head);
    sort(req, n);
    int pos = 0;                       /* first request >= head */
    while (pos < n && req[pos] < head)
        pos++;
    printf("\nSeek Sequence: %d", head);
    /* 1. service requests towards higher cylinders */
    for (i = pos; i < n; i++) {
        totalSeek += abs(head - req[i]);
        head = req[i];
        printf(" -> %d", head);
    }
    /* 2. jump to the lowest pending request (not to cylinder 0) */
    if (pos > 0) {
        totalSeek += abs(head - req[0]);
        head = req[0];
        printf(" -> %d", head);
    }
    /* 3. continue upward through the remaining requests */
    for (i = 1; i < pos; i++) {
        totalSeek += abs(head - req[i]);
        head = req[i];
        printf(" -> %d", head);
    }
    printf("\nTotal Seek Time = %d\n", totalSeek);
    return 0;
}

//Step 3 - Compile and execute
gcc sstf.c -o sstf
gcc scan.c -o scan
gcc clook.c -o clook
# same input for all three:
#   8
#   98 183 37 122 14 124 65 67
#   53
# SCAN additionally asks disk size: 200
./sstf
./scan
./cloo
