#include <stdio.h>

#define N 2
#define M 3
#define P 2

int main() {
    int A[N][M] = {
        {1, 2, 3},
        {4, 5, 6}
    };

    int B[M][P] = {
        {7, 8},
        {9, 10},
        {11, 12}
    };

    int C[N][P];

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < P; j++) {
            C[i][j] = 0;
        }
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < P; j++) {
            for (int k = 0; k < M; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    printf("Le produit des deux matrices est :\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < P; j++) {
            printf("%d\t", C[i][j]);
        }
        printf("\n");
    }

    return 0;
}
