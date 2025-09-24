#include <stdio.h>

int somme(int n) {
    int s = 0;
    for(int i =0; i < n; i++) {
        s += 1;
    }
    return s;
}

int multiply(int a, int b) {
    int result = 0;
    for (int i = 0; i < b; i++) {
        result = result + a; // ajout de a, a fois
    }
    return result;
}

int main() {
    int a = 3, b = 4;
    printf("Resultat de %d * %d est : %d\n", a, b, multiply(a,b));
    return 0;
}