#include <stdio.h>
#include <string.h>
#include "commands.h"
#include "image.h"

void execute_command(const char *command, Image *image) {
    char cmd[100];
    sscanf(command, "%s", cmd);

    if (strcmp(cmd, "size") == 0) {
        get_image_size(image);
    } 
    else if (strcmp(cmd, "gris") == 0) {
        convert_to_grayscale(image);
    } 
    else if (strcmp(cmd, "neg") == 0) {
        // Implémentation pour créer le négatif
    } 
    else if (strncmp(cmd, "dom", 3) == 0) {
        // Implémentation pour foncer ou éclaircir
    } 
    else if (strncmp(cmd, "cut", 3) == 0) {
        // Implémentation pour découper l'image
    } 
    else if (strncmp(cmd, "fil", 3) == 0) {
        // Implémentation pour appliquer le filtre médian
    } 
    else {
        printf("Commande inconnue.\n");
    }
}
