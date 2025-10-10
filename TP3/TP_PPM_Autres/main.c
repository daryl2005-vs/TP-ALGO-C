#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* ---------- Structures ---------- */
typedef struct {
    int r, g, b;
} Pixel;

typedef struct {
    int width, height;
    int maxval;
    Pixel *data;
} Image;

/* ---------- Fonctions utilitaires ---------- */
int clamp(int x, int min, int max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

static int next_token(FILE *f, char *buf, size_t bufsize) {
    int c; size_t i = 0;
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

/* ---------- Chargement et sauvegarde ---------- */
Image *load_ppm(const char *filename) {
    FILE *f = fopen(filename, "rb");
    if (!f) { perror("open"); return NULL; }

    char token[256];
    if (!next_token(f, token, sizeof(token)) || strcmp(token, "P3") != 0) {
        fprintf(stderr, "Format PPM non supporté (P3 attendu)\n");
        fclose(f); return NULL;
    }

    int width, height, maxval;
    if (!next_token(f, token, sizeof(token))) return NULL; width = atoi(token);
    if (!next_token(f, token, sizeof(token))) return NULL; height = atoi(token);
    if (!next_token(f, token, sizeof(token))) return NULL; maxval = atoi(token);

    Image *img = malloc(sizeof(Image));
    img->width = width; img->height = height; img->maxval = maxval;
    img->data = malloc(sizeof(Pixel) * width * height);

    int r, g, b, i = 0;
    while (i < width * height && next_token(f, token, sizeof(token))) {
        r = atoi(token);
        if (!next_token(f, token, sizeof(token))) break;
        g = atoi(token);
        if (!next_token(f, token, sizeof(token))) break;
        b = atoi(token);
        img->data[i].r = clamp(r, 0, maxval);
        img->data[i].g = clamp(g, 0, maxval);
        img->data[i].b = clamp(b, 0, maxval);
        i++;
    }

    fclose(f);
    return img;
}

int save_ppm(const char *filename, const Image *img) {
    FILE *f = fopen(filename, "w");
    if (!f) { perror("save"); return 0; }
    fprintf(f, "P3\n%d %d\n%d\n", img->width, img->height, img->maxval);
    int n = img->width * img->height;
    for (int i = 0; i < n; i++) {
        fprintf(f, "%d %d %d ", img->data[i].r, img->data[i].g, img->data[i].b);
        if ((i + 1) % img->width == 0) fprintf(f, "\n");
    }
    fclose(f);
    return 1;
}

void free_image(Image *img) {
    if (img) { free(img->data); free(img); }
}

/* ---------- Opérations sur l’image ---------- */
void foncer_ou_eclaircir(Image *img, char color, int val) {
    int n = img->width * img->height;
    for (int i = 0; i < n; i++) {
        Pixel *p = &img->data[i];
        int dom = (p->r >= p->g && p->r >= p->b) ? 'R' :
                  (p->g >= p->r && p->g >= p->b) ? 'G' : 'B';
        if (toupper(color) == dom) {
            p->r = clamp(p->r + val, 0, img->maxval);
            p->g = clamp(p->g + val, 0, img->maxval);
            p->b = clamp(p->b + val, 0, img->maxval);
        }
    }
}

void gris(Image *img) {
    int n = img->width * img->height;
    for (int i = 0; i < n; i++) {
        int avg = (img->data[i].r + img->data[i].g + img->data[i].b) / 3;
        img->data[i].r = img->data[i].g = img->data[i].b = avg;
    }
}

void negatif(Image *img) {
    int n = img->width * img->height;
    for (int i = 0; i < n; i++) {
        img->data[i].r = img->maxval - img->data[i].r;
        img->data[i].g = img->maxval - img->data[i].g;
        img->data[i].b = img->maxval - img->data[i].b;
    }
}

Image *decouper(const Image *img, int l1, int l2, int c1, int c2) {
    int newh = l2 - l1 + 1, neww = c2 - c1 + 1;
    Image *out = malloc(sizeof(Image));
    out->width = neww; out->height = newh; out->maxval = img->maxval;
    out->data = malloc(sizeof(Pixel) * newh * neww);
    for (int r = 0; r < newh; r++)
        for (int c = 0; c < neww; c++)
            out->data[r*neww + c] = img->data[(l1 + r)*img->width + (c1 + c)];
    return out;
}

/* ---------- Interface ppmviewer ---------- */
void run_shell(void) {
    char cmd[256];
    printf("ppmviewer> (tapez 'quit' pour sortir)\n");
    while (1) {
        printf("ppmviewer> ");
        if (!fgets(cmd, sizeof(cmd), stdin)) break;

        char op[16], arg1[64], arg2[64], arg3[64], arg4[64], arg5[64], arg6[64];
        int n = sscanf(cmd, "%15s %63s %63s %63s %63s %63s %63s",
                       op, arg1, arg2, arg3, arg4, arg5, arg6);

        if (n == 0) continue;
        if (strcmp(op, "quit") == 0) break;
        else if (strcmp(op, "size") == 0 && n >= 2) {
            Image *img = load_ppm(arg1);
            if (img) {
                printf("%dx%d pixels\n", img->width, img->height);
                free_image(img);
            }
        } else if (strcmp(op, "gris") == 0 && n >= 2) {
            Image *img = load_ppm(arg1);
            gris(img);
            save_ppm(arg1, img);
            free_image(img);
        } else if (strcmp(op, "dom") == 0 && n >= 4) {
            char c = arg1[0];
            int val = atoi(arg2);
            Image *img = load_ppm(arg3);
            foncer_ou_eclaircir(img, c, val);
            save_ppm(arg3, img);
            free_image(img);
        } else if (strcmp(op, "neg") == 0 && n >= 3) {
            Image *img = load_ppm(arg1);
            negatif(img);
            save_ppm(arg2, img);
            free_image(img);
        } else if (strcmp(op, "cut") == 0 && n >= 7) {
            int l1 = atoi(arg2), l2 = atoi(arg3), c1 = atoi(arg4), c2 = atoi(arg5);
            Image *img = load_ppm(arg1);
            Image *part = decouper(img, l1, l2, c1, c2);
            save_ppm(arg6, part);
            free_image(part); free_image(img);
        } else {
            printf("Commande inconnue.\n");
        }
    }
}

/* ---------- Fonction principale ---------- */
int main(void) {
    run_shell();
    return 0;
}
