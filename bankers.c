#include <stdio.h>
int main()
{
    int n, m, i, j, k;
    printf("Enter number of processes: ");
    scanf("%d", &n);
    printf("Enter number of resource types: ");
    scanf("%d", &m);
    int alloc[n][m], max[n][m], need[n][m];
    int avail[m];
    printf("\nEnter Allocation Matrix:\n");
    for (i = 0; i < n; i++)
        for (j = 0; j < m; j++)
            scanf("%d", &alloc[i][j]);
    printf("\nEnter Maximum Matrix:\n");
    for (i = 0; i < n; i++)
        for (j = 0; j < m; j++)
            scanf("%d", &max[i][j]);
    printf("\nEnter Available Resources:\n");
    for (j = 0; j < m; j++)
        scanf("%d", &avail[j]);
    /* Need = Max - Allocation */
    for (i = 0; i < n; i++)
        for (j = 0; j < m; j++)
            need[i][j] = max[i][j] - alloc[i][j];
    printf("\nNeed Matrix:\n");
    for (i = 0; i < n; i++) {
        printf("P%d: ", i);
        for (j = 0; j < m; j++)
            printf("%d ", need[i][j]);
Operating Systems Lab - Complete Practical Guide (Programs, Commands, Viva) Page 17
        printf("\n");
    }
    int finish[n], safeSeq[n], work[m];
    for (i = 0; i < n; i++) finish[i] = 0;
    for (j = 0; j < m; j++) work[j] = avail[j];
    int count = 0;
    while (count < n) {
        int found = 0;
        for (i = 0; i < n; i++) {
            if (finish[i] == 0) {
                for (j = 0; j < m; j++)
                    if (need[i][j] > work[j])
                        break;
                if (j == m) {                 /* Need <= Work */
                    for (k = 0; k < m; k++)
                        work[k] += alloc[i][k];   /* process releases resources */
                    safeSeq[count++] = i;
                    finish[i] = 1;
                    found = 1;
                }
            }
        }
        if (found == 0) {
            printf("\nSystem is NOT in Safe State (deadlock possible).\n");
            return 0;
        }
    }
    printf("\nSystem is in Safe State.\n");
    printf("Safe Sequence: ");
    for (i = 0; i < n; i++) {
        printf("P%d", safeSeq[i]);
        if (i != n - 1)
            printf(" -> ");
    }
    printf("\n");
    return 0;
}
