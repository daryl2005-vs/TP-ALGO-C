#include <stdio.h>
#include <stdlib.h>
#include "image.h"
#include "commands.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: ppmviewer <filename>\n");
        return 1;
    }

    Image *image = load_image(argv[1]);
    if (!image) {
        printf("Erreur lors du chargement de l'image.\n");
        return 1;
    }

    char command[100];
    
    printf("ppmviewer > ");
    while (fgets(command, sizeof(command), stdin)) {
        command[strcspn(command, "\n")] = 0; // Enlève le saut de ligne
        execute_command(command, image);
        printf("ppmviewer > ");
    }

    free(image->data);
    free(image);
    return 0;
}
