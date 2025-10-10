#include <stdio.h>
#include <stdlib.h>
#include "image.h"

// Fonctions pour charger et sauvegarder une image
Image* load_image(const char *filename) {
    // Implémentation pour charger une image PPM
}

void save_image(const char *filename, Image *image) {
    // Implémentation pour sauvegarder une image PPM
}

// Fonctions pour manipuler l'image
void darken_lighten(Image *image, char color, int value) {
    // Implémentation pour foncer ou éclaircir les pixels
}

void convert_to_grayscale(Image *image) {
    // Implémentation pour passer en noir et blanc
}

void create_negative(Image *image) {
    // Implémentation pour créer le négatif d'une image
}

void get_image_size(Image *image) {
    printf("%d x %d\n", image->width, image->height);
}

void crop_image(Image *image, int l1, int l2, int c1, int c2, const char *output_filename) {
    // Implémentation pour découper l'image
}

void apply_median_filter(Image *image, const char *output_filename) {
    // Implémentation pour appliquer le filtre médian
}
