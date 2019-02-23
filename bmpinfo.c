/*
 *
 * Read and display BMP image informations.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bmp.h"


int main(int argc, char *argv[])
{
    FILE *fp;
    BITMAPFILEHEADER *bmFileHeader = NULL;
    BITMAPCOREHEADER *bmCoreHeader = NULL;
    BITMAPINFOHEADER *bmInfoHeader = NULL;
    int headersize;

    if (argc != 2) {
        printf("Usage: bmpinfo <file.bmp>\n\n");
        exit(1);
    }

    if ((fp = fopen(argv[1], "rb")) == NULL) {
        printf("Cannot open file: %s\n\n", argv[1]);
        exit(1);
    }
    bmFileHeader = ReadBMFileHeader(fp);
    if (strcmp(bmFileHeader->bfType, "BM") != 0) {
        printf("The file is not BITMAP.\n");
        exit(1);
    }
    headersize = SizeOfInformationHeader(fp);
    if (headersize == 12) {
        bmCoreHeader = ReadBMCoreHeader(fp);
    } else if (headersize == 40) {
        bmInfoHeader = ReadBMInfoHeader(fp);
    } else {
        printf("Unsupported BITMAP.\n");
        exit(1);
    }
    fclose(fp);

    printf("File type          = %s\n", bmFileHeader->bfType);
    printf("File size          = %d bytes\n", bmFileHeader->bfSize);
    printf("Data offset        = %ld bytes\n", bmFileHeader->bfOffBits);
    if (headersize == 12) {
        printf("Info header size   = %d bytes\n", bmCoreHeader->bcSize);
        printf("Width              = %d pixels\n", bmCoreHeader->bcWidth);
        printf("Height             = %d pixels\n", bmCoreHeader->bcHeight);
        printf("Planes             = %d\n", bmCoreHeader->bcPlanes);
        printf("Bit count          = %d bits/pixel\n", bmCoreHeader->bcBitCount);
    } else if (headersize == 40) {
        printf("Info header size   = %d bytes\n", bmInfoHeader->biSize);
        printf("Width              = %ld pixels\n", bmInfoHeader->biWidth);
        printf("Height             = %ld pixels\n", bmInfoHeader->biHeight);
        printf("Planes             = %d\n", bmInfoHeader->biPlanes);
        printf("Bit count          = %d bits/pixel\n", bmInfoHeader->biBitCount);
        printf("Compression        = %d\n", bmInfoHeader->biCompression);
        printf("Size image         = %d bytes\n", bmInfoHeader->biSizeImage);
        printf("X pixels per meter = %ld\n", bmInfoHeader->biXPixPerMeter);
        printf("Y pixels per meter = %ld\n", bmInfoHeader->biYPixPerMeter);
        printf("Color used         = %ld colors\n", bmInfoHeader->biClrUsed);
    }

    free(bmFileHeader);
    free(bmCoreHeader);
    free(bmInfoHeader);

    return 0;
}

