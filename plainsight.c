#include <stdio.h>
#include <stdlib.h>
#include "math.h"
#include <string.h>
#include "bmp.h"
#include <time.h>

#define RESERVED_OFFSET 6
#define FIRST_MODULUS 0x0C
#define SECOND_MODULUS 0X11
#define SPACE 0x06
#define EXTRA 0x00
unsigned short ReadLE2(FILE *fp);
unsigned int ReadLE4(FILE *fp);

union {
  unsigned int x;
  struct {
    unsigned int r:8;
    unsigned int g:8;
    unsigned int b:8;
  } rgb;
} pixel;

unsigned char* encryptedBuffer(unsigned char* buffer, unsigned long fileLen)
{
  for (int i = 56; i < fileLen + 1; i += 3)
  {
    pixel.rgb.r = (int)buffer[i];
    pixel.rgb.g = (int)buffer[i + 1];
    pixel.rgb.b = (int)buffer[i + 2];

    printf("%.2X%.2X%.2X ", pixel.rgb.r, pixel.rgb.g, pixel.rgb.b);
  }

  return NULL;
}

void output(unsigned char *buffer, int fileLen)
{
  for (int c = 0; c < fileLen + 1; c++)
  {
    printf("%.2X ", (int)buffer[c]);

    // 4 columns
    if (c % 4 == 3)
    {
      printf(" ");
    }

    // 16 bytes long
    if (c % 16 == 15)
    {
      printf("\n");
    }
  }

  printf("\n");
}

int genRandomPosition(char* inputMessage, BITMAPFILEHEADER *bmFileHeader, BITMAPINFOHEADER *bmInfoHeader, unsigned long messageSize)
{
  char* message = (char*)malloc(messageSize + 1);

  strcpy(message, inputMessage);

  srand(time(NULL));

  if (messageSize * 7 > (bmInfoHeader->biWidth * bmInfoHeader->biHeight - 54))
  {
    printf("The message is too big, either provide a smaller message, or a larger photo.\n");
    exit(0);
  }

  int max = bmInfoHeader->biWidth * bmInfoHeader->biHeight - (messageSize * 7);
  int randomPosition = rand() % (max + 1 - bmFileHeader->bfOffBits) + bmFileHeader->bfOffBits;

  free(message);

  return randomPosition;
}

char* caesarCipher(char* inputMessage, int shiftAmount, unsigned long messageSize)
{
  char* message = (char*)malloc(messageSize + 1);

  strcpy(message, inputMessage);

  // Begin implementation of caesar cipher
  char ch;
	int i;
  for(i = 0; message[i] != '\0'; ++i){
		ch = message[i];

		if(ch >= 'a' && ch <= 'z'){
			ch = ch + shiftAmount;

			if(ch > 'z'){
				ch = ch - 'z' + 'a' - 1;
			}

			message[i] = ch;
		}
		else if(ch >= 'A' && ch <= 'Z'){
			ch = ch + shiftAmount;

			if(ch > 'Z'){
				ch = ch - 'Z' + 'A' - 1;
			}

			message[i] = ch;
		}
	}

  return message;

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

  int offset = genRandomPosition(argv[2], bmFileHeader, bmInfoHeader, strlen(argv[2]));

  buffer[RESERVED_OFFSET] =     FIRST_MODULUS;  // The start of the escape character sequence
  buffer[RESERVED_OFFSET + 1] = SECOND_MODULUS;  // The modulus number
  buffer[RESERVED_OFFSET + 2] = SPACE;  // The spaces between pixels
  buffer[RESERVED_OFFSET + 3] = EXTRA;  // Reserving this value right now
  fwrite(buffer, 1, fileLen, fp);

  output(buffer, fileLen);

  //ciphered[sizeof(argv[2])] = {'\0'};
  char* ciphered = caesarCipher(argv[2], 1, strlen(argv[2]));
  //strcpy(ciphered, caesarCipher(argv[2], 1));

  printf("%s\n", ciphered);

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

  encryptedBuffer(buffer, fileLen);

  free(bmFileHeader);
  free(bmCoreHeader);
  free(bmInfoHeader);
  free(ciphered);

  return 0;
}
