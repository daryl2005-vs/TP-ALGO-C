#include <stdio.h>

#define N 3 // dimension du vecteur
#define M 3 // nombre de colonnes de la matrice

void vecteur_matrice(int v[], int Mtx[N][M], int result[M]) {
    for(int j=0; j<M; j++) {
        result[j] = 0;
        for(int i=0; i<N; i++) {
            result[j] += v[i] * Mtx[i][j];
        }
    }
}

int main() {
    int v[N] = {1, 2, 3};
    int Mtx[N][M] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    int result[M];

    vecteur_matrice(v, Mtx, result);

    printf("Résultat du produit vecteur × matrice : ");
    for (int j = 0; j < M; j++) {
        printf("%d ", result[j]);
    }
    printf("\n");
    return 0;
}