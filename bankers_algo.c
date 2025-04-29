#include <stdio.h>
#include <stdbool.h>

#define P 5 // Number of processes
#define R 3 // Number of resources

int main() {
    int alloc[P][R];
    int max[P][R];
    int avail[R];
    int need[P][R];
    bool finish[P] = {false};
    int safeSeq[P];
    int i, j, k;

    // Input Allocation Matrix
    printf("Enter Allocation Matrix (%d x %d):\n", P, R);
    for (i = 0; i < P; i++) {
        printf("Process P%d: ", i);
        for (j = 0; j < R; j++) {
            scanf("%d", &alloc[i][j]);
        }
    }

    // Input Maximum Matrix
    printf("\nEnter Maximum Matrix (%d x %d):\n", P, R);
    for (i = 0; i < P; i++) {
        printf("Process P%d: ", i);
        for (j = 0; j < R; j++) {
            scanf("%d", &max[i][j]);
        }
    }

    // Input Available Resources
    printf("\nEnter Available Resources (size %d): ", R);
    for (j = 0; j < R; j++) {
        scanf("%d", &avail[j]);
    }

    // Calculate Need Matrix
    for (i = 0; i < P; i++)
        for (j = 0; j < R; j++)
            need[i][j] = max[i][j] - alloc[i][j];

    int count = 0;
    while (count < P) {
        bool found = false;
        for (i = 0; i < P; i++) {
            if (!finish[i]) {
                for (j = 0; j < R; j++)
                    if (need[i][j] > avail[j])
                        break;

                if (j == R) {
                    for (k = 0; k < R; k++)
                        avail[k] += alloc[i][k];
                    safeSeq[count++] = i;
                    finish[i] = true;
                    found = true;
                }
            }
        }

        if (!found) {
            printf("\nSystem is NOT in a safe state.\n");
            return 0;
        }
    }

    printf("\nSystem is in a safe state.\nSafe sequence is: ");
    for (i = 0; i < P; i++)
        printf("P%d ", safeSeq[i]);
    printf("\n");

    return 0;
}
