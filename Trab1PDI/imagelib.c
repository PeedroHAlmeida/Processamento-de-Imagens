#include <stdio.h>
#include <stdlib.h>
#include "imagelib.h"

// Creates and allocates memory for a new image
image* img_create(int width, int height, int max_val) {
    image *img = (image*)malloc(sizeof(image));
    if (!img) {
        perror("Failed to allocate memory for image struct");
        exit(EXIT_FAILURE);
    }
    img->width = width;
    img->height = height;
    img->max_val = max_val;
    img->data = (int*)malloc(width * height * sizeof(int));
    if (!img->data) {
        perror("Failed to allocate memory for image data");
        free(img);
        exit(EXIT_FAILURE);
    }
    return img;
}

// Frees memory used by an image
void img_free(image* img) {
    if (img) {
        free(img->data);
        free(img);
    }
}

// Saves an image to a PGM file
void save_pgm(image* img, const char* filename) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        perror("Failed to open file for writing");
        return;
    }

    // Write PGM header
    fprintf(fp, "P2\n");
    fprintf(fp, "# Created by PGH decoder\n");
    fprintf(fp, "%d %d\n", img->width, img->height);
    fprintf(fp, "%d\n", img->max_val);

    // Write pixel data
    for (int i = 0; i < img->height; i++) {
        for (int j = 0; j < img->width; j++) {
            fprintf(fp, "%d ", img->data[i * img->width + j]);
        }
        fprintf(fp, "\n");
    }

    fclose(fp);
    printf("Image saved as %s\n", filename);
}