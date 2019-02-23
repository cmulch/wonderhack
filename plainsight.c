#include <stdio.h>
#include <stdlib.h>
#include "math.h"
#include <string.h>
#include "bmp.h"
#include <time.h>
unsigned short ReadLE2(FILE *fp);
unsigned int ReadLE4(FILE *fp);

/*
 * Read bitmap file header
 */
BITMAPFILEHEADER *ReadBMFileHeader(FILE *fp)
{
    BITMAPFILEHEADER *header;
    char           filetype[3] = {'\0', '\0', '\0'};
    unsigned int   filesize;
    unsigned short reserved1;
    unsigned short reserved2;
    unsigned long  offset;

    /* File type (2 bytes) */
    fread(&filetype, 1, 2, fp);

    /* File size (4 bytes) */
    filesize = (unsigned int) ReadLE4(fp);

    /* Reserved 1 (2 bytes) */
    fread(&reserved1, 2, 1, fp);

    /* Reserved 2 (2 bytes) */
    fread(&reserved2, 2, 1, fp);

    /* Offset (4 bytes) */
    offset = (unsigned long) ReadLE4(fp);

    header = (BITMAPFILEHEADER *) malloc(sizeof(BITMAPFILEHEADER));
    strcpy(header->bfType, filetype);
    header->bfSize      = filesize;
    header->bfReserved1 = reserved1;
    header->bfReserved2 = reserved2;
    header->bfOffBits   = offset;

    return header;
}

/*
 * Returns size of information header
 */
int SizeOfInformationHeader(FILE *fp)
{
    int headersize;
    unsigned char buf[4];
    int i;

    fread(buf, 1, 4, fp);
    for (i = 3; i >= 0; i--) {
        headersize = (headersize << 8) | (int) buf[i];
    }

    fseek(fp, 14, SEEK_SET);

    return headersize;
}

/*
 * Read bitmap core header (OS/2 bitmap)
 */
BITMAPCOREHEADER *ReadBMCoreHeader(FILE *fp)
{
    BITMAPCOREHEADER *header;
    unsigned int   headersize;
    int            width;
    int            height;
    unsigned short planes;
    unsigned short bitcount;

    /* Header size (4 bytes) */
    headersize = (unsigned int) ReadLE4(fp);

    /* Width (2 bytes) */
    width = (int) ReadLE2(fp);

    /* Height (2 bytes) */
    height = (int) ReadLE2(fp);

    /* Planes (2 bytes) */
    planes = (unsigned short) ReadLE2(fp);

    /* Bit Count (2 bytes) */
    bitcount = (unsigned short) ReadLE2(fp);

    header = (BITMAPCOREHEADER *) malloc(sizeof(BITMAPCOREHEADER));
    header->bcSize     = headersize;
    header->bcWidth    = width;
    header->bcHeight   = height;
    header->bcPlanes   = planes;
    header->bcBitCount = bitcount;

    return header;
}

/*
 * Read bitmap info header (Windows bitmap)
 */
BITMAPINFOHEADER *ReadBMInfoHeader(FILE *fp)
{
    BITMAPINFOHEADER *header;
    unsigned int   headersize;
    int            width;
    int            height;
    unsigned short planes;
    unsigned short bitcount;
    unsigned int   compression;
    unsigned int   size_image;
    int            x_pix_per_meter;
    int            y_pix_per_meter;
    unsigned int   clr_used;
    unsigned int   clr_important;

    /* Header size (4 bytes) */
    headersize = (unsigned int) ReadLE4(fp);

    /* Width (4 bytes) */
    width = (int) ReadLE4(fp);

    /* Height (4 bytes) */
    height = (int) ReadLE4(fp);

    /* Planes (2 bytes) */
    planes = (unsigned short) ReadLE2(fp);

    /* Bit Count (2 bytes) */
    bitcount = (unsigned short) ReadLE2(fp);

    /* Compression (4 bytes) */
    compression = (unsigned int) ReadLE4(fp);

    /* Size image (4 bytes) */
    size_image = (unsigned int) ReadLE4(fp);

    /* X pix per meter (4 bytes) */
    x_pix_per_meter = (int) ReadLE4(fp);

    /* Y pix per meter (4 bytes) */
    y_pix_per_meter = (int) ReadLE4(fp);

    /* Color used (4 bytes) */
    clr_used = (unsigned int) ReadLE4(fp);

    /* Color important (4 bytes) */
    clr_important = (unsigned int) ReadLE4(fp);

    header = (BITMAPINFOHEADER *) malloc(sizeof(BITMAPINFOHEADER));
    header->biSize         = headersize;
    header->biWidth        = width;
    header->biHeight       = height;
    header->biPlanes       = planes;
    header->biBitCount     = bitcount;
    header->biCompression  = compression;
    header->biSizeImage    = size_image;
    header->biXPixPerMeter = x_pix_per_meter;
    header->biYPixPerMeter = y_pix_per_meter;
    header->biClrUsed      = clr_used;
    header->biClrImportant = clr_important;

    return header;
}

/*
 * Read 2 bytes in little endian
 */
unsigned short ReadLE2(FILE *fp)
{
    unsigned char buf[2];
    unsigned short result = 0;
    int i;

    fread(buf, 1, 2, fp);
    for (i = 1; i >= 0; i--) {
        result = (result << 8) | (unsigned short) buf[i];
    }

    return result;
}

/*
 * Read 4 bytes in little endian
 */
unsigned int ReadLE4(FILE *fp)
{
    unsigned char buf[4];
    unsigned int result = 0;
    int i;

    fread(buf, 1, 4, fp);
    for (i = 3; i >= 0; i--) {
        result = (result << 8) | (unsigned int) buf[i];
    }

    return result;
}

void output(unsigned char *buffer, int fileLen)
{
  for (int c = 0; c < fileLen + 1; c++)
  {
    printf("%.2X ", (int)buffer[c]);

    if (c % 4 == 3)
    {
      printf(" ");
    }

    if (c % 16 == 15)
    {
      printf("\n");
    }
  }
}

int genRandomPosition(char* inputMessage, BITMAPFILEHEADER *bmFileHeader, BITMAPINFOHEADER *bmInfoHeader)
{
  char message[sizeof(inputMessage)] = {'\0'};

  strcpy(message, inputMessage);

  srand(time(NULL));

  int max = bmInfoHeader->biWidth * bmInfoHeader->biHeight - 13;

  int randomPosition = rand() % (max + 1 - bmFileHeader->bfOffBits);

  return randomPosition;
}

int main(int argc,char **argv)
{
  FILE *fp;
  BITMAPFILEHEADER *bmFileHeader = NULL;
  BITMAPCOREHEADER *bmCoreHeader = NULL;
  BITMAPINFOHEADER *bmInfoHeader = NULL;
  int headersize;

  if (argc != 3) {
      printf("Usage: bmpinfo <file.bmp> <message>\n\n");
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

  //printf("HEADER SIZE: %d", headersize);

  if (headersize == 12) {
      bmCoreHeader = ReadBMCoreHeader(fp);
  } else if (headersize == 40) {
      bmInfoHeader = ReadBMInfoHeader(fp);
  } else {
      printf("Unsupported BITMAP.\n");
      exit(1);
  }

  // Get file length
  fseek(fp, 0, SEEK_END);
  unsigned long fileLen = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  // Create our buffer with all the data from the file. Cast malloc return to a char pointer
  // So that the program knows we are pointing to characters instead of any data type
  unsigned char *buffer = (unsigned char *)malloc(fileLen + 1);
  if (!buffer)
  {
    fprintf(stderr, "Memory error!");
    fclose(fp);
    return 0;
  }

  // Read everything into our buffer
  fread(buffer, fileLen, 1, fp);

  output(buffer, fileLen);

  int offset = genRandomPosition(argv[2], bmFileHeader, bmInfoHeader);

  int reserved_offset = 6;
  buffer[reserved_offset] = 0xFF;
  buffer[reserved_offset + 1] = 0x11;
  buffer[reserved_offset + 2] = 0x06;
  buffer[reserved_offset + 3] = 0x00;
  fwrite(buffer, 1, fileLen, fp);

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

  printf("Random Offset: %d\n", offset);

  free(bmFileHeader);
  free(bmCoreHeader);
  free(bmInfoHeader);

  return 0;
}
