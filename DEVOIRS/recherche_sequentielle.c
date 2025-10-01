#include <stdio.h>

int main() {
    int n, valeur, trouve = 0;

    printf("Entrez la taille du tableau : ");
    scanf("%d", &n);

    int tab[n];

    printf("Entrez les elements du tableau :\n");
    for (int i = 0; i < n; i++) {
        scanf("%d", &tab[i]);
    }

    printf("Entrez la valeur a rechercher : ");
    scanf("%d", &valeur);

    for (int i = 0; i < n; i++) {
        if (tab[i] == valeur) {
            printf("Valeur %d trouvee a la position %d (indice %d).\n", valeur, i + 1, i);
            trouve = 1;
            break;
        }
    }

    if (!trouve) {
        printf("Valeur %d non trouvee dans le tableau.\n", valeur);
    }

    return 0;
}
