#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "image.h"

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
