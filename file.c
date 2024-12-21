#include <stdio.h>
#include <math.h>

int main(void) {
    FILE *fp = fopen("input.bmp", "rb");
    FILE *output = fopen("output2.bmp", "wb");

    if (fp == NULL || output == NULL) {
        printf("Error in opening files.\n");
        return 1;
    }

    unsigned char header[54];
    if (fread(header, sizeof(unsigned char), 54, fp) != 54) {
        printf("Error reading BMP header.\n");
        return 1;
    }

    fwrite(header, sizeof(unsigned char), 54, output);

    // Extract image dimensions from the header
    int width = *(int *)&header[18];
    int height = *(int *)&header[22];
    int bitDepth = *(int *)&header[28];
    printf("width: %d, height: %d\n", width, height);
    if(bitDepth <= 8){
        unsigned char colorTable[1024];
        if(fread(colorTable, sizeof(unsigned char), sizeof(colorTable), fp) != sizeof(colorTable) ){
            printf("error : in reading colorTable");
            return 1;
        }
        fwrite(colorTable, sizeof(unsigned char), sizeof(colorTable), output);
    }
    int imageDataSize = 4*width*height;
    unsigned char buffer[imageDataSize];
    // int padding = (4 - (width * 3) % 4) % 4;

    if(fread(buffer, sizeof(unsigned char), imageDataSize, fp) != imageDataSize){
        printf("error: in reading buffer");
    }
    unsigned char THRESHOLD = 128, WHITE = 255, BLACK = 0;
    for(int i=0; i<imageDataSize; i++){
        buffer[i] = (buffer[i] > THRESHOLD) ? WHITE : BLACK;
        // printf("%d ", buffer[i]);
    }
    fwrite(buffer, sizeof(unsigned char), imageDataSize, output);
    fclose(fp);
    fclose(output);

    printf("Grayscale BMP created successfully.\n");
    return 0;
}
