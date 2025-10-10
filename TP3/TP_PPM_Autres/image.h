#ifndef IMAGE_H
#define IMAGE_H

typedef struct {
    int r, g, b;
} Pixel;

typedef struct {
    int width, height;
    int maxval;
    Pixel *data;
} Image;

Image *load_ppm(const char *filename);
int save_ppm(const char *filename, const Image *img);
void free_image(Image *img);
int clamp(int x, int min, int max);

#endif
