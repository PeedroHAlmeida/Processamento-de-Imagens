#ifndef IMAGELIB_H
#define IMAGELIB_H

// Represents a grayscale image
typedef struct {
    int width;
    int height;
    int max_val; // Maximum gray value
    int *data;   // Pointer to pixel data
} image;

/**
 * @brief Creates a new image structure and allocates memory for its pixels.
 * * @param width The width of the image.
 * @param height The height of the image.
 * @param max_val The maximum gray value for the image.
 * @return A pointer to the newly created image structure.
 */
image* img_create(int width, int height, int max_val);

/**
 * @brief Frees the memory associated with an image structure.
 * * @param img A pointer to the image to be freed.
 */
void img_free(image* img);

/**
 * @brief Saves an image structure to a PGM file.
 * * @param img The image to save.
 * @param filename The name of the output PGM file.
 */
void save_pgm(image* img, const char* filename);

#endif // IMAGELIB_H