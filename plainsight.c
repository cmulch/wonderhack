#include <stdio.h>
#include <stdlib.h>
#include "math.h"
#include <string.h>
#include "bmp.h"
#include <time.h>

#define RESERVED_OFFSET 6
#define FIRST_MODULUS 0x0C
#define SECOND_MODULUS 0X11
#define SPACE 0xFF
#define EXTRA 0x00
unsigned short ReadLE2(FILE *fp);
unsigned int ReadLE4(FILE *fp);

struct {
  unsigned int r:8;
  unsigned int g:8;
  unsigned int b:8;
} rgb;

// Possibly using wrong & in if statements?
unsigned char* encryptedBuffer(unsigned char* buffer, unsigned long fileLen, int offset, char* message, unsigned long messageSize)
{
  unsigned char* encryptedBuf = (unsigned char*)malloc(fileLen + 1);

  int messageCounter = 0;

  int rgbTotal = 0;
  int rgbMod = 0;
  char currentChar;
  int toReach = 0;
  int difference = 0;

  int currentPos = 0;

  for (int i = 0; i < fileLen + 1; i++)
  {
    encryptedBuf[i] = buffer[i];
  }

  for (int i = offset; i < fileLen + 1; i += SPACE)
  {
    rgb.r = (int)encryptedBuf[i];
    rgb.g = (int)encryptedBuf[i + 1];
    rgb.b = (int)encryptedBuf[i + 2];

    rgbTotal = 0;

    rgbTotal += rgb.r;
    rgbTotal += rgb.g;
    rgbTotal += rgb.b;

    currentChar = message[messageCounter];

    if (messageCounter == messageSize)
    {
      //printf("We've finished writing our method...\n");
      break;
    }
    else if (messageCounter < messageSize)
    {
      //printf("Writing normal character...\n");
      //printf("Current Character: %d\n", (int)currentChar - 97);
      //printf("Current Character: %c\n", currentChar);

      toReach = (int)currentChar - 97;

      rgbMod = rgbTotal % 26;

      //printf("Current rgbMod: %d\n", rgbMod);

      //printf("Initial RGB Total: %d\n", rgbTotal);

      //printf("%.2X%.2X%.2X\n", rgb.r, rgb.g, rgb.b);

      difference = toReach - rgbMod;

      //printf("Current difference: %d\n", difference);

      if (difference < 0 & abs(difference) > 13)
      {
        //printf("Negative difference, overflow increment...\n");

        for (int j = 0; j < abs(difference); j++)
        {
          if (currentPos == 3)
          {
            //printf("Switching back to 0\n");
            currentPos = 0;
          }

          if (currentPos == 0)
          {
            //printf("Updating R\n");

            if (rgb.r == 255)
            {
              //printf("This should be an overflow...\n");
              currentPos++;
              j--;
              continue;
            }

            rgb.r = rgb.r + 1;
            currentPos++;
            continue;
          }
          else if (currentPos == 1)
          {
            //printf("Updating G\n");

            if (rgb.g == 255)
            {
              //printf("This should be an overflow...\n");
              currentPos++;
              j--;
              continue;
            }

            rgb.g = rgb.g + 1;
            currentPos++;
            continue;
          }
          else if (currentPos == 2)
          {
            //printf("Updating B\n");

            if (rgb.b == 255)
            {
              //printf("This should be an overflow...\n");
              currentPos++;
              j--;
              continue;
            }

            rgb.b = rgb.b + 1;
            currentPos++;
            continue;
          }
        }

        rgbTotal = 0;
        rgbTotal += rgb.r;
        rgbTotal += rgb.g;
        rgbTotal += rgb.b;

        //printf("%.2X%.2X%.2X\n", rgb.r, rgb.g, rgb.b);
        //printf("Updated RGB Total: %d\n", rgbTotal);
      }
      else if (difference > 0 & abs(difference) > 13)
      {
        //printf("Positive difference, overflow decrement...\n");

        for (int j = 0; j < abs(difference); j++)
        {
          if (currentPos == 3)
          {
            //printf("Switching back to 0\n");
            currentPos = 0;
          }

          if (currentPos == 0)
          {
            //printf("Updating R\n");

            if (rgb.r == 0)
            {
              //printf("This should be an overflow...\n");
              currentPos++;
              j--;
              continue;
            }

            rgb.r = rgb.r - 1;
            currentPos++;
            continue;
          }
          else if (currentPos == 1)
          {
            //printf("Updating G\n");

            if (rgb.g == 0)
            {
              //printf("This should be an overflow...\n");
              currentPos++;
              j--;
              continue;
            }

            rgb.g = rgb.g - 1;
            currentPos++;
            continue;
          }
          else if (currentPos == 2)
          {
            //printf("Updating B\n");

            if (rgb.b == 0)
            {
              //printf("This should be an overflow...\n");
              currentPos++;
              j--;
              continue;
            }

            rgb.b = rgb.b - 1;
            currentPos++;
            continue;
          }
        }

        rgbTotal = 0;
        rgbTotal += rgb.r;
        rgbTotal += rgb.g;
        rgbTotal += rgb.b;

        //printf("%d%d%d\n", rgb.r, rgb.g, rgb.b);
        //printf("Updated RGB Total: %d\n", rgbTotal);
      }
      else if (difference < 0)
      {
        //printf("Negative difference, decrement...\n");

        for (int j = 0; j < abs(difference); j++)
        {
          if (currentPos == 3)
          {
            //printf("Switching back to 0\n");
            currentPos = 0;
          }

          if (currentPos == 0)
          {
            //printf("Updating R\n");

            if (rgb.r == 0)
            {
              //printf("This should be an overflow...\n");
              currentPos++;
              j--;
              continue;
            }

            rgb.r = rgb.r - 1;
            currentPos++;
            continue;
          }
          else if (currentPos == 1)
          {
            //printf("Updating G\n");

            if (rgb.g == 0)
            {
              //printf("This should be an overflow...\n");
              currentPos++;
              j--;
              continue;
            }

            rgb.g = rgb.g - 1;
            currentPos++;
            continue;
          }
          else if (currentPos == 2)
          {
            //printf("Updating B\n");

            if (rgb.b == 0)
            {
              //printf("This should be an overflow...\n");
              currentPos++;
              j--;
              continue;
            }

            rgb.b = rgb.b - 1;
            currentPos++;
            continue;
          }
        }

        rgbTotal = 0;
        rgbTotal += rgb.r;
        rgbTotal += rgb.g;
        rgbTotal += rgb.b;

        //printf("%d%d%d\n", rgb.r, rgb.g, rgb.b);
        //printf("Updated RGB Total: %d\n", rgbTotal);
      }
      else if (difference > 0)
      {
        //printf("Positive difference, increment...\n");

        for (int j = 0; j < abs(difference); j++)
        {
          if (currentPos == 3)
          {
            //printf("Switching back to 0\n");
            currentPos = 0;
          }

          if (currentPos == 0)
          {
            //printf("Updating R\n");

            if (rgb.r == 255)
            {
              //printf("This should be an overflow...\n");
              currentPos++;
              j--;
              continue;
            }

            rgb.r = rgb.r + 1;
            currentPos++;
            continue;
          }
          else if (currentPos == 1)
          {
            //printf("Updating G\n");

            if (rgb.g == 255)
            {
              //printf("This should be an overflow...\n");
              currentPos++;
              j--;
              continue;
            }

            rgb.g = rgb.g + 1;
            currentPos++;
            continue;
          }
          else if (currentPos == 2)
          {
            //printf("Updating B\n");

            if (rgb.b == 255)
            {
              //printf("This should be an overflow...\n");
              currentPos++;
              j--;
              continue;
            }

            rgb.b = rgb.b + 1;
            currentPos++;
            continue;
          }
        }

        rgbTotal = 0;
        rgbTotal += rgb.r;
        rgbTotal += rgb.g;
        rgbTotal += rgb.b;

        //printf("%d%d%d\n", rgb.r, rgb.g, rgb.b);
        //printf("Updated RGB Total: %d\n", rgbTotal);
      }

      messageCounter++;
    }

    //printf("%.2X%.2X%.2X\n", rgb.r, rgb.g, rgb.b);

    encryptedBuf[i] = (char)rgb.r;
    encryptedBuf[i + 1] = (char)rgb.g;
    encryptedBuf[i + 2] = (char)rgb.b;
  }

  return encryptedBuf;
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

short int genRandomPosition(char* inputMessage, BITMAPFILEHEADER *bmFileHeader, BITMAPINFOHEADER *bmInfoHeader, unsigned long messageSize)
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
  short int randomPosition = rand() % (max + 1 - bmFileHeader->bfOffBits) + bmFileHeader->bfOffBits;

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
  FILE *out;
  BITMAPFILEHEADER *bmFileHeader = NULL;
  BITMAPCOREHEADER *bmCoreHeader = NULL;
  BITMAPINFOHEADER *bmInfoHeader = NULL;
  int headersize;

  if (argc != 4) {
      printf("Usage: bmpinfo <infile.bmp> <outfile.bmp> <message>\n\n");
      exit(1);
  }

  if ((fp = fopen(argv[1], "rb")) == NULL) {
      printf("Cannot open file: %s\n\n", argv[1]);
      exit(1);
  }
  if ((out = fopen(argv[2], "w+")) == NULL) {
      printf("Cannot open file: %s\n\n", argv[2]);
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

  if (strlen(argv[3] > 8192))
  {
    printf("String is too long, please try again with a smaller string.\n")
    exit(1);
  }

  short int offset = genRandomPosition(argv[3], bmFileHeader, bmInfoHeader, strlen(argv[3]));

  buffer[RESERVED_OFFSET] = (offset >> 8);  // The index into the file where the data starts
  buffer[RESERVED_OFFSET + 1] = (offset & 0xFF);
  buffer[RESERVED_OFFSET + 2] = SPACE;  // The spaces between pixels
  buffer[RESERVED_OFFSET + 3] = EXTRA;  // Reserving this value right now
  output(buffer, fileLen);

  //ciphered[sizeof(argv[2])] = {'\0'};
  char* ciphered = caesarCipher(argv[3], 1, strlen(argv[3]));
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

  unsigned char* newBuf = encryptedBuffer(buffer, fileLen, offset, ciphered, strlen(argv[3]));
  //output(newBuf, fileLen);

  //printf("%d\n", memcmp(newBuf, buffer, 20000));

  fwrite(newBuf, 1, fileLen, out);

  fclose(out);
  free(bmFileHeader);
  free(bmCoreHeader);
  free(bmInfoHeader);
  free(ciphered);

  return 0;
}
