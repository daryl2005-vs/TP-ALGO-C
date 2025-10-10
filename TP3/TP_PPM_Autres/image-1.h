#ifndef IMAGE_H
#define IMAGE_H

typedef struct {
    int width;
    int height;
    int max_color_value;
    unsigned char *data; // tableau des pixels
} Image;

Image* load_image(const char *filename);
void save_image(const char *filename, Image *image);
void darken_lighten(Image *image, char color, int value);
void convert_to_grayscale(Image *image);
void create_negative(Image *image);
void get_image_size(Image *image);
void crop_image(Image *image, int l1, int l2, int c1, int c2, const char *output_filename);
void apply_median_filter(Image *image, const char *output_filename);

#endif // IMAGE_H
