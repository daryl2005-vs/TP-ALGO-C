 #include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int largeur;
    int hauteur;
    int max_val;
    unsigned char *pixels; // tableau pour stocker les pixels
} ImagePPM;

// Prototypes de fonctions
ImagePPM* charger_image(const char *nom_fichier);
void sauver_image(const char *nom_fichier, ImagePPM *image);
void foncer_eclaircir(ImagePPM *image, char couleur, int val);
void passer_en_noir_et_blanc(ImagePPM *image);
void creer_negatif(ImagePPM *image);
void afficher_taille(ImagePPM *image);
ImagePPM* decouper_image(ImagePPM *image, int l1, int l2, int c1, int c2);
void appliquer_filtre_median(ImagePPM *image);
void liberer_image(ImagePPM *image);
void afficher_menu();

int main() {
    char commande[100];
    ImagePPM *image = NULL;

    printf("Application de traitement d'images PPM.\n");
    afficher_menu();

    while (1) {
        printf("ppmviewer > ");
        fgets(commande, sizeof(commande), stdin);
        commande[strcspn(commande, "\n")] = 0; // enlever le saut de ligne

        if (strncmp(commande, "quit", 4) == 0) {
            break;
        } else if (strncmp(commande, "size ", 5) == 0) {
            image = charger_image(commande + 5);
            if (image) {
                afficher_taille(image);
                liberer_image(image);
            }
        } else if (strncmp(commande, "gris ", 5) == 0) {
            image = charger_image(commande + 5);
            if (image) {
                passer_en_noir_et_blanc(image);
                sauver_image("image_gris.ppm", image);
                printf("Image en niveaux de gris sauvegardée sous 'image_gris.ppm'.\n");
                liberer_image(image);
            }
        } else if (strncmp(commande, "neg ", 4) == 0) {
            image = charger_image(commande + 4);
            if (image) {
                creer_negatif(image);
                sauver_image("image_neg.ppm", image);
                printf("Négatif de l'image sauvegardé sous 'image_neg.ppm'.\n");
                liberer_image(image);
            }
        } else if (strncmp(commande, "cut ", 4) == 0) {
            int l1, l2, c1, c2;
            char nom_fichier[100];
            sscanf(commande + 4, "%d %d %d %d %s", &l1, &l2, &c1, &c2, nom_fichier);
            image = charger_image(nom_fichier);
            if (image) {
                ImagePPM *decoupe = decouper_image(image, l1, l2, c1, c2);
                if (decoupe) {
                    sauver_image("image_decoupe.ppm", decoupe);
                    printf("Image découpée sauvegardée sous 'image_decoupe.ppm'.\n");
                    liberer_image(decoupe);
                }
                liberer_image(image);
            }
        } else if (strncmp(commande, "dom ", 4) == 0) {
            char couleur;
            int val;
            char nom_fichier[100];
            sscanf(commande + 4, "%c %d %s", &couleur, &val, nom_fichier);
            image = charger_image(nom_fichier);
            if (image) {
                foncer_eclaircir(image, couleur, val);
                sauver_image("image_modifiee.ppm", image);
                printf("Image modifiée sauvegardée sous 'image_modifiee.ppm'.\n");
                liberer_image(image);
            }
        } else if (strncmp(commande, "fil ", 4) == 0) {
            char nom_fichier[100];
            sscanf(commande + 4, "%s", nom_fichier);
            image = charger_image(nom_fichier);
            if (image) {
                appliquer_filtre_median(image);
                sauver_image("image_filtre.ppm", image);
                printf("Filtre médian appliqué, sauvegardé sous 'image_filtre.ppm'.\n");
                liberer_image(image);
            }
        } else {
            printf("Commande non reconnue. Tapez 'help' pour voir les commandes disponibles.\n");
        }
    }

    liberer_image(image);
    return 0;
}

// Fonction pour charger une image PPM
ImagePPM* charger_image(const char *nom_fichier) {
    FILE *fichier = fopen(nom_fichier, "r");
    if (!fichier) {
        printf("Erreur : fichier '%s' non trouvé.\n", nom_fichier);
        return NULL;
    }

    ImagePPM *image = malloc(sizeof(ImagePPM));
    if (!image) {
        fclose(fichier);
        printf("Erreur : échec de l'allocation de mémoire pour l'image.\n");
        return NULL;
    }

    if (fscanf(fichier, "P3\n%d %d\n%d\n", &image->largeur, &image->hauteur, &image->max_val) != 3) {
        fclose(fichier);
        free(image);
        printf("Erreur : format du fichier incorrect.\n");
        return NULL;
    }

    image->pixels = malloc(image->largeur * image->hauteur * 3);
    if (!image->pixels) {
        fclose(fichier);
        free(image);
        printf("Erreur : échec de l'allocation de mémoire pour les pixels.\n");
        return NULL;
    }

    for (int i = 0; i < image->largeur * image->hauteur * 3; i++) {
        if (fscanf(fichier, "%hhu", &image->pixels[i]) != 1) {
            fclose(fichier);
            free(image->pixels);
            free(image);
            printf("Erreur : échec de la lecture des pixels.\n");
            return NULL;
        }
    }
    fclose(fichier);
    printf("Image chargée : %d x %d\n", image->largeur, image->hauteur);
    return image;
}

// Fonction pour sauvegarder une image PPM
void sauver_image(const char *nom_fichier, ImagePPM *image) {
    FILE *fichier = fopen(nom_fichier, "w");
    if (!fichier) {
        printf("Erreur : échec de l'ouverture du fichier pour sauvegarde.\n");
        return;
    }
    
    fprintf(fichier, "P3\n%d %d\n%d\n", image->largeur, image->hauteur, image->max_val);
    for (int i = 0; i < image->largeur * image->hauteur * 3; i++) {
        fprintf(fichier, "%d ", image->pixels[i]);
    }
    fclose(fichier);
}

// Fonctions de manipulation d'images
void foncer_eclaircir(ImagePPM *image, char couleur, int val) {
    for (int i = 0; i < image->largeur * image->hauteur; i++) {
        unsigned char *composantes = &image->pixels[i * 3]; // R, G, B
        int index = (couleur == 'R') ? 0 : (couleur == 'G') ? 1 : 2;
        int new_value = composantes[index] + val;

        // Limiter les valeurs entre 0 et 255
        if (new_value < 0) new_value = 0;
        if (new_value > 255) new_value = 255;

        composantes[index] = new_value;
    }
}

void passer_en_noir_et_blanc(ImagePPM *image) {
    for (int i = 0; i < image->largeur * image->hauteur; i++) {
        unsigned char *composantes = &image->pixels[i * 3];
        unsigned char gris = (composantes[0] + composantes[1] + composantes[2]) / 3;
        composantes[0] = composantes[1] = composantes[2] = gris;
    }
}

void creer_negatif(ImagePPM *image) {
    for (int i = 0; i < image->largeur * image->hauteur; i++) {
        unsigned char *composantes = &image->pixels[i * 3];
        composantes[0] = 255 - composantes[0];
        composantes[1] = 255 - composantes[1];
        composantes[2] = 255 - composantes[2];
    }
}

void afficher_taille(ImagePPM *image) {
    printf("Taille de l'image : %d x %d\n", image->largeur, image->hauteur);
}

ImagePPM* decouper_image(ImagePPM *image, int l1, int l2, int c1, int c2) {
    if (l1 >= l2 || c1 >= c2 || l2 > image->hauteur || c2 > image->largeur) {
        printf("Erreur : coordonnées invalides.\n");
        return NULL;
    }

    ImagePPM *decoupe = malloc(sizeof(ImagePPM));
    if (!decoupe) {
        printf("Erreur : échec de l'allocation de mémoire pour l'image découpée.\n");
        return NULL;
    }

    decoupe->largeur = c2 - c1;
    decoupe->hauteur = l2 - l1;
    decoupe->max_val = image->max_val;
    decoupe->pixels = malloc(decoupe->largeur * decoupe->hauteur * 3);
    if (!decoupe->pixels) {
        free(decoupe);
        printf("Erreur : échec de l'allocation de mémoire pour les pixels découpés.\n");
        return NULL;
    }

    for (int l = l1; l < l2; l++) {
        for (int c = c1; c < c2; c++) {
            int idx_src = (l * image->largeur + c) * 3;
            int idx_dst = ((l - l1) * decoupe->largeur + (c - c1)) * 3;
            decoupe->pixels[idx_dst] = image->pixels[idx_src];
            decoupe->pixels[idx_dst + 1] = image->pixels[idx_src + 1];
            decoupe->pixels[idx_dst + 2] = image->pixels[idx_src + 2];
        }
    }

    return decoupe;
}

void appliquer_filtre_median(ImagePPM *image) {
    unsigned char *pixels_temp = malloc(image->largeur * image->hauteur * 3);
    for (int i = 0; i < image->largeur * image->hauteur; i++) {
        int voisins[9][3] = {0}; // 3 couleurs pour 8 voisins + soi-même
        int compteur = 0;
        for (int di = -1; di <= 1; di++) {
            for (int dj = -1; dj <= 1; dj++) {
                int ni = (i / image->largeur) + di;
                int nj = (i % image->largeur) + dj;
                if (ni >= 0 && ni < image->hauteur && nj >= 0 && nj < image->largeur) {
                    int idx = (ni * image->largeur + nj) * 3;
                    voisins[compteur][0] = image->pixels[idx];
                    voisins[compteur][1] = image->pixels[idx + 1];
                    voisins[compteur][2] = image->pixels[idx + 2];
                    compteur++;
                }
            }
        }

        for (int c = 0; c < 3; c++) {
            // Tri pour obtenir la valeur médiane
            for (int a = 0; a < compteur - 1; a++) {
                for (int b = a + 1; b < compteur; b++) {
                    if (voisins[a][c] > voisins[b][c]) {
                        int temp = voisins[a][c];
                        voisins[a][c] = voisins[b][c];
                        voisins[b][c] = temp;
                    }
                }
            }
            pixels_temp[i * 3 + c] = voisins[compteur / 2][c]; // valeur médiane
        }
    }

    memcpy(image->pixels, pixels_temp, image->largeur * image->hauteur * 3);
    free(pixels_temp);
}

void liberer_image(ImagePPM *image) {
    if (image) {
        free(image->pixels);
        free(image);
    }
}

void afficher_menu() {
    printf("Commandes disponibles :\n");
    printf("1. size <nom_fichier> - Afficher la taille de l'image.\n");
    printf("2. gris <nom_fichier> - Convertir l'image en niveaux de gris.\n");
    printf("3. neg <nom_fichier> - Créer le négatif de l'image.\n");
    printf("4. cut <l1> <l2> <c1> <c2> <nom_fichier> - Découper l'image.\n");
    printf("5. dom <R/G/B> <valeur> <nom_fichier> - Foncer ou éclaircir.\n");
    printf("6. fil <nom_fichier> - Appliquer un filtre médian.\n");
    printf("7. quit - Quitter l'application.\n");
}
