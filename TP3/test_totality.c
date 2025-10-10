#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    int largeur;
    int hauteur;
    int max_val;
    unsigned char *pixels;
} ImagePPM;

int clamp(int x, int min, int max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

static int next_token(FILE *f, char *buf, size_t bufsize) {
    int c;
    size_t i = 0;
    while ((c = fgetc(f)) != EOF) {
        if (isspace(c)) continue;
        if (c == '#') { while ((c = fgetc(f)) != EOF && c != '\n'); continue; }
        ungetc(c, f);
        break;
    }
    if (c == EOF) return 0;
    while ((c = fgetc(f)) != EOF && !isspace(c) && c != '#') {
        if (i + 1 < bufsize) buf[i++] = (char)c;
    }
    buf[i] = '\0';
    if (c == '#') while ((c = fgetc(f)) != EOF && c != '\n');
    return 1;
}

ImagePPM* charger_image(const char *nom_fichier) {
    FILE *f = fopen(nom_fichier, "r");
    if (!f) {
        printf("Erreur : fichier '%s' non trouvé.\n", nom_fichier);
        return NULL;
    }

    char token[256];
    if (!next_token(f, token, sizeof(token)) || strcmp(token, "P3") != 0) {
        printf("Format PPM non supporté (P3 attendu).\n");
        fclose(f);
        return NULL;
    }

    int width, height, maxval;
    if (!next_token(f, token, sizeof(token))) { fclose(f); return NULL; }
    width = atoi(token);
    if (!next_token(f, token, sizeof(token))) { fclose(f); return NULL; }
    height = atoi(token);
    if (!next_token(f, token, sizeof(token))) { fclose(f); return NULL; }
    maxval = atoi(token);

    ImagePPM *image = (ImagePPM *)malloc(sizeof(ImagePPM));
    if (!image) { fclose(f); return NULL; }
    image->largeur = width;
    image->hauteur = height;
    image->max_val = maxval;
    image->pixels = (unsigned char *)malloc(3 * width * height * sizeof(unsigned char));
    if (!image->pixels) { free(image); fclose(f); return NULL; }

    int r, g, b, i = 0;
    while (i < width * height) {
        if (!next_token(f, token, sizeof(token))) break;
        r = atoi(token);
        if (!next_token(f, token, sizeof(token))) break;
        g = atoi(token);
        if (!next_token(f, token, sizeof(token))) break;
        b = atoi(token);
        image->pixels[i * 3 + 0] = (unsigned char)clamp(r, 0, maxval);
        image->pixels[i * 3 + 1] = (unsigned char)clamp(g, 0, maxval);
        image->pixels[i * 3 + 2] = (unsigned char)clamp(b, 0, maxval);
        i++;
    }

    if (i < width * height) {
        printf("Erreur : données de pixels incomplètes.\n");
        free(image->pixels);
        free(image);
        fclose(f);
        return NULL;
    }

    fclose(f);
    return image;
}

void sauver_image(const char *nom_fichier, ImagePPM *image) {
    FILE *f = fopen(nom_fichier, "w");
    if (!f) {
        printf("Erreur : impossible de sauvegarder '%s'.\n", nom_fichier);
        return;
    }
    fprintf(f, "P3\n%d %d\n%d\n", image->largeur, image->hauteur, image->max_val);
    for (int y = 0; y < image->hauteur; y++) {
        for (int x = 0; x < image->largeur; x++) {
            int idx = (y * image->largeur + x) * 3;
            fprintf(f, "%d %d %d ", image->pixels[idx], image->pixels[idx + 1], image->pixels[idx + 2]);
        }
        fprintf(f, "\n");
    }
    fclose(f);
}

void liberer_image(ImagePPM *image) {
    if (image) {
        free(image->pixels);
        free(image);
    }
}

void foncer_eclaircir(ImagePPM *image, char couleur, int val) {
    couleur = toupper(couleur);
    for (int i = 0; i < image->largeur * image->hauteur; i++) {
        unsigned char *p = &image->pixels[i * 3];
        unsigned char r = p[0], g = p[1], b = p[2];
        char dom;
        if (r >= g && r >= b) dom = 'R';
        else if (g >= r && g >= b) dom = 'G';
        else dom = 'B';
        if (dom == couleur) {
            p[0] = (unsigned char)clamp((int)r + val, 0, image->max_val);
            p[1] = (unsigned char)clamp((int)g + val, 0, image->max_val);
            p[2] = (unsigned char)clamp((int)b + val, 0, image->max_val);
        }
    }
}

void passer_en_noir_et_blanc(ImagePPM *image) {
    for (int i = 0; i < image->largeur * image->hauteur; i++) {
        unsigned char *p = &image->pixels[i * 3];
        unsigned char avg = ((int)p[0] + p[1] + p[2]) / 3;
        p[0] = p[1] = p[2] = avg;
    }
}

void creer_negatif(ImagePPM *image) {
    for (int i = 0; i < image->largeur * image->hauteur; i++) {
        unsigned char *p = &image->pixels[i * 3];
        p[0] = (unsigned char)(image->max_val - p[0]);
        p[1] = (unsigned char)(image->max_val - p[1]);
        p[2] = (unsigned char)(image->max_val - p[2]);
    }
}

ImagePPM* decouper_image(ImagePPM *image, int l1, int l2, int c1, int c2) {
    if (l1 < 1 || l2 < l1 || l2 > image->hauteur || c1 < 1 || c2 < c1 || c2 > image->largeur) {
        printf("coordonnées invalides\n");
        return NULL;
    }
    int new_h = l2 - l1 + 1;
    int new_w = c2 - c1 + 1;
    ImagePPM *decoupe = (ImagePPM *)malloc(sizeof(ImagePPM));
    decoupe->largeur = new_w;
    decoupe->hauteur = new_h;
    decoupe->max_val = image->max_val;
    decoupe->pixels = (unsigned char *)malloc(3 * new_w * new_h * sizeof(unsigned char));
    for (int yy = 0; yy < new_h; yy++) {
        for (int xx = 0; xx < new_w; xx++) {
            int old_y = l1 - 1 + yy;
            int old_x = c1 - 1 + xx;
            int old_idx = (old_y * image->largeur + old_x) * 3;
            int new_idx = (yy * new_w + xx) * 3;
            decoupe->pixels[new_idx] = image->pixels[old_idx];
            decoupe->pixels[new_idx + 1] = image->pixels[old_idx + 1];
            decoupe->pixels[new_idx + 2] = image->pixels[old_idx + 2];
        }
    }
    return decoupe;
}

void appliquer_filtre_median(ImagePPM *image) {
    int w = image->largeur;
    int h = image->hauteur;
    unsigned char *temp = (unsigned char *)malloc(3 * w * h * sizeof(unsigned char));
    for (int i = 0; i < w * h; i++) {
        unsigned char voisins[9][3];
        int compteur = 0;
        int y = i / w;
        int x = i % w;
        for (int dy = -1; dy <= 1; dy++) {
            for (int dx = -1; dx <= 1; dx++) {
                int ny = y + dy;
                int nx = x + dx;
                if (ny >= 0 && ny < h && nx >= 0 && nx < w) {
                    int idx = (ny * w + nx) * 3;
                    voisins[compteur][0] = image->pixels[idx];
                    voisins[compteur][1] = image->pixels[idx + 1];
                    voisins[compteur][2] = image->pixels[idx + 2];
                    compteur++;
                }
            }
        }
        for (int c = 0; c < 3; c++) {
            // Tri à bulles
            for (int a = 0; a < compteur - 1; a++) {
                for (int b = a + 1; b < compteur; b++) {
                    if (voisins[a][c] > voisins[b][c]) {
                        unsigned char tmp = voisins[a][c];
                        voisins[a][c] = voisins[b][c];
                        voisins[b][c] = tmp;
                    }
                }
            }
            temp[i * 3 + c] = voisins[compteur / 2][c];
        }
    }
    memcpy(image->pixels, temp, 3 * w * h * sizeof(unsigned char));
    free(temp);
}

void afficher_menu() {
    printf("Commandes disponibles :\n");
    printf("dom <R/G/B> <valeur> <fichier.ppm> - Foncer ou éclaircir.\n");
    printf("gris <fichier.ppm> - Passer en noir et blanc.\n");
    printf("neg <fichier_entree.ppm> <fichier_sortie.ppm> - Créer le négatif.\n");
    printf("size <fichier.ppm> - Afficher la taille.\n");
    printf("cut <fichier_entree.ppm> <l1> <l2> <c1> <c2> <fichier_sortie.ppm> - Découper.\n");
    printf("fil <fichier_entree.ppm> <fichier_sortie.ppm> - Appliquer filtre médian.\n");
    printf("quit - Quitter.\n");
}

int main() {
    char commande[256];
    printf("Application de traitement d'images PPM.\n");
    afficher_menu();

    while (1) {
        printf("ppmviewer > ");
        if (!fgets(commande, sizeof(commande), stdin)) break;
        commande[strcspn(commande, "\n")] = 0;

        if (strcmp(commande, "quit") == 0) break;

        else if (strncmp(commande, "size ", 5) == 0) {
            char nom[100];
            sscanf(commande + 5, "%s", nom);
            ImagePPM *img = charger_image(nom);
            if (img) {
                printf("%d x %d\n", img->largeur, img->hauteur);
                liberer_image(img);
            }
        }

        else if (strncmp(commande, "gris ", 5) == 0) {
            char nom[100];
            sscanf(commande + 5, "%s", nom);
            ImagePPM *img = charger_image(nom);
            if (img) {
                passer_en_noir_et_blanc(img);
                char output[200];
                strcpy(output, nom);
                char *dot = strrchr(output, '.');
                if (dot) *dot = '\0';
                strcat(output, "_gris.ppm");
                sauver_image(output, img);
                printf("opération effectuée ; %s créé\n", output);
                liberer_image(img);
            }
        }

        else if (strncmp(commande, "dom ", 4) == 0) {
            char c;
            int val;
            char nom[100];
            sscanf(commande + 4, "%c %d %s", &c, &val, nom);
            ImagePPM *img = charger_image(nom);
            if (img) {
                foncer_eclaircir(img, c, val);
                char output[200];
                strcpy(output, nom);
                char *dot = strrchr(output, '.');
                if (dot) *dot = '\0';
                strcat(output, "_dom.ppm");
                sauver_image(output, img);
                printf("opération effectuée ; %s créé\n", output);
                liberer_image(img);
            } else {
                printf("fichier non trouvé\n");
            }
        }

        else if (strncmp(commande, "neg ", 4) == 0) {
            char input[100], output[100];
            sscanf(commande + 4, "%s %s", input, output);
            ImagePPM *img = charger_image(input);
            if (img) {
                creer_negatif(img);
                sauver_image(output, img);
                printf("opération effectuée\n");
                liberer_image(img);
            }
        }

        else if (strncmp(commande, "cut ", 4) == 0) {
            char input[100], output[100];
            int l1, l2, c1, c2;
            sscanf(commande + 4, "%s %d %d %d %d %s", input, &l1, &l2, &c1, &c2, output);
            ImagePPM *img = charger_image(input);
            if (img) {
                ImagePPM *decoupe = decouper_image(img, l1, l2, c1, c2);
                if (decoupe) {
                    sauver_image(output, decoupe);
                    printf("opération effectuée\n");
                    liberer_image(decoupe);
                }
                liberer_image(img);
            }
        }

        else if (strncmp(commande, "fil ", 4) == 0) {
            char input[100], output[100];
            sscanf(commande + 4, "%s %s", input, output);
            ImagePPM *img = charger_image(input);
            if (img) {
                appliquer_filtre_median(img);
                sauver_image(output, img);
                printf("opération effectuée\n");
                liberer_image(img);
            }
        }

        else {
            printf("Commande non reconnue.\n");
        }
    }
    return 0;
}