#include <stdio.h>
#include <stdlib.h>

// Structure to hold grayscale pixel data
typedef struct {
    unsigned char value;
} PixelGray;

// Function to read a PGM image into a 2D array
PixelGray** readPGM(const char* filename, int* width, int* height) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Error: Unable to open file %s\n", filename);
        exit(1);
    }

    // Read PGM header
    char header[3];
    fscanf(file, "%s", header);
    if (header[0] != 'P' || header[1] != '5') {
        fprintf(stderr, "Error: Not a valid PGM file\n");
        exit(1);
    }

    // Skip comments
    char c = getc(file);
    while (c == '#') {
        while (getc(file) != '\n');
        c = getc(file);
    }
    ungetc(c, file);

    // Read width, height, and max grayscale value
    fscanf(file, "%d %d", width, height);
    int maxGrayValue;
    fscanf(file, "%d", &maxGrayValue);
    fgetc(file); // Read newline character

    // Allocate memory for pixel array
    PixelGray** image = (PixelGray**)malloc(*height * sizeof(PixelGray*));
    for (int i = 0; i < *height; ++i) {
        image[i] = (PixelGray*)malloc(*width * sizeof(PixelGray));
    }

    // Read pixel data
    for (int i = 0; i < *height; ++i) {
        fread(image[i], sizeof(PixelGray), *width, file);
    }

    fclose(file);
    return image;
}

// Function to write a 2D matrix as a PGM image
void writePGM(const char* filename, PixelGray** matrix, int* width, int* height) {
    FILE* file = fopen(filename, "wb");
    if (!file) {
        fprintf(stderr, "Error: Unable to open file %s\n", filename);
        exit(1);
    }

    // Write PGM header
    fprintf(file, "P5\n%d %d\n255\n", *width, *height);

    // Write pixel data
    for (int i = 0; i < *height; ++i) {
        fwrite(matrix[i], sizeof(PixelGray), *width, file);
    }

    fclose(file);
}

// Function to threshold the image matrix
PixelGray** threshold(PixelGray** matrix, int* width, int* height) {
    // Allocate memory for thresholded image
    PixelGray** thresholded = (PixelGray**)malloc(*height * sizeof(PixelGray*));
    for (int i = 0; i < *height; ++i) {
        thresholded[i] = (PixelGray*)malloc(*width * sizeof(PixelGray));
    }

    // Thresholding: Convert pixel values above 127 to 255, else 0
    for (int i = 0; i < *height; ++i) {
        for (int j = 0; j < *width; ++j) {
            thresholded[i][j].value = (matrix[i][j].value > 127) ? 255 : 0;
        }
    }

    return thresholded;
}

// Function to rotate the image matrix left
PixelGray** rotateLeft(PixelGray** matrix, int* width, int* height) {
    // Allocate memory for rotated image
    PixelGray** rotated = (PixelGray**)malloc(*width * sizeof(PixelGray*));
    for (int i = 0; i < *width; ++i) {
        rotated[i] = (PixelGray*)malloc(*height * sizeof(PixelGray));
    }

    // Perform rotation: swapping rows and columns
    for (int i = 0; i < *width; ++i) {
        for (int j = 0; j < *height; ++j) {
            rotated[*width - i - 1][j].value = matrix[j][i].value;
        }
    }

    // Update width and height
    int temp = *width;
    *width = *height;
    *height = temp;

    return rotated;
}

// Function to convert binary image to grayscale
PixelGray** binaryToGrayscale(PixelGray** binary, int width, int height) {
    // Allocate memory for grayscale image
    PixelGray** grayscale = (PixelGray**)malloc(height * sizeof(PixelGray*));
    for (int i = 0; i < height; ++i) {
        grayscale[i] = (PixelGray*)malloc(width * sizeof(PixelGray));
    }

    // Convert binary values (0 or 255) to grayscale (0 to 255)
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            grayscale[i][j].value = binary[i][j].value;
        }
    }

    return grayscale;
}

//main function - DO NOT MODIFY
int main() {
    int width, height;  // variable to hold width and height. Use reference in other functions

    PixelGray** image_original = readPGM("lenna.pgm", &width, &height);
    // Now you have the grayscale image data in the 'image_original' 2D array

    // Create a new 2D array 'image_thresh' to store the threshold image
    PixelGray** image_thresh = threshold(image_original, &width, &height);
    // Convert thresholded image to grayscale
    PixelGray** grayscale_thresh = binaryToGrayscale(image_thresh, width, height);
    // Write the grayscale thresholded image
    writePGM("threshold_grayscale.pgm", grayscale_thresh, &width, &height);

    // Create a new 2D array 'image_rotate' to store the rotated image
    PixelGray** image_rotate = rotateLeft(image_original, &width, &height);
    // Convert rotated image to grayscale
    PixelGray** grayscale_rotate = binaryToGrayscale(image_rotate, width, height);
    // Write the grayscale rotated image
    writePGM("rotate_left_grayscale.pgm", grayscale_rotate, &width, &height);

    // Free the allocated memory when you're done
    for (int i = 0; i < height; ++i) {
        free(image_original[i]);
        free(image_thresh[i]);
        free(grayscale_thresh[i]);
        free(image_rotate[i]);
        free(grayscale_rotate[i]);
    }
    free(image_original);
    free(image_thresh);
    free(grayscale_thresh);
    free(image_rotate);
    free(grayscale_rotate);
    return 0;
}
