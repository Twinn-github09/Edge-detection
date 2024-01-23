#include <stdio.h>
#include <math.h>
#include <string.h>

#define MAX_WIDTH 1024
#define MAX_HEIGHT 1024

unsigned char image[MAX_HEIGHT][MAX_WIDTH];

int main() {
    FILE *inputFile, *outputFile;
    char type[3];
    int width, height, maxVal;

    // To Open the PGM file
    inputFile = fopen("input.pgm", "rb");
    if (inputFile == NULL) {
        printf("Error: Unable to open the input file.\n");
        return -1;
    }
    else 
    printf("Opened file sucessfully.....\n");

    // To Read the header of the PGM file
    fscanf(inputFile, "%2s %d %d %d", type, &width, &height, &maxVal);
    if (strcmp(type, "P5") != 0 || maxVal > 255 || width > MAX_WIDTH || height > MAX_HEIGHT) {
        printf("Error!!: Invalid or unsupported PGM file format :( ).\n");
        fclose(inputFile);
        return -1;
    }
    else
    printf("\n_______________IMAGE INFO__________________\n\n");
	printf("* Version: %s \n* Width: %d \n* Height: %d \n* Maximum Gray Level: %d \n", type, width, height,maxVal);
    printf("\nFile processing ......\n");

    // To Read the image data
    fread(image, sizeof(unsigned char), width * height, inputFile);
    fclose(inputFile);

    // Sobel edge detection
    int gx[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };
    int gy[3][3] = {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}
    };
    double sumx, sumy;
    unsigned char edgeImage[MAX_HEIGHT][MAX_WIDTH];

    for (int i = 1; i < height - 1; i++) {
        for (int j = 1; j < width - 1; j++) {
            sumx = 0.0;  
            sumy = 0.0;
            for (int p = -1; p <= 1; p++) {
                for (int q = -1; q <= 1; q++) {
                    sumx += (image[i + p][j + q] * gx[p + 1][q + 1]);
                    sumy += (image[i + p][j + q] * gy[p + 1][q + 1]);
                }
            } 
            edgeImage[i][j] = (unsigned char)sqrt(sumx * sumx + sumy * sumy);
        }
    }

    int threshold = 105; // 
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (edgeImage[i][j] < threshold) {
                edgeImage[i][j] = 0; // Setting it low values to get clearer edges
            } else {
                edgeImage[i][j] = 255; // Setting it to high values to get maximum for edges
            }
        }
    }

    // Write the output PGM file with the edges detected
    outputFile = fopen("output.pgm", "wb");
    if (outputFile == NULL) {
        printf("Error!! : Unable to create the output file.\n");
        return -1;
    }
    else
    
    printf("\nPreparing pgm file.......\n");

    fprintf(outputFile, "P5\n%d %d\n%d\n", width, height, maxVal);
    fwrite(edgeImage, sizeof(unsigned char), width * height, outputFile);
    fclose(outputFile);

    printf("\n\"Sobel edge detection completed\".\n\n Output saved as \"output.pgm\"\n");

    return 0;
}
