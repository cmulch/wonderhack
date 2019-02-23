/*
 *
 * Reading bitmap file informations.
 *
 */

/*
 * Bitmap file header
 */
typedef struct tagBITMAPFILEHEADER {
    char           bfType[3];    /* 2 bytes + null char */
    unsigned int   bfSize;       /* 4 bytes */
    unsigned short bfReserved1;  /* 2 bytes */
    unsigned short bfReserved2;  /* 2 bytes */
    unsigned long  bfOffBits;    /* 4 bytes */
} BITMAPFILEHEADER;

/*
 * Bitmap info header (Windows)
 */
typedef struct tagBITMAPINFOHEADER {
    unsigned int   biSize;          /* 4 bytes */
    long           biWidth;         /* 4 bytes */
    long           biHeight;        /* 4 bytes */
    unsigned short biPlanes;        /* 2 bytes */
    unsigned short biBitCount;      /* 2 bytes */
    unsigned int   biCompression;   /* 4 bytes */
    unsigned int   biSizeImage;     /* 4 bytes */
    long           biXPixPerMeter;  /* 4 bytes */
    long           biYPixPerMeter;  /* 4 bytes */
    unsigned long  biClrUsed;       /* 4 bytes */
    unsigned long  biClrImportant;  /* 4 bytes */
} BITMAPINFOHEADER;

/*
 * Bitmap core header (OS/2)
 */
typedef struct tagBITMAPCOREHEADER {
    unsigned int   bcSize;      /* 4 bytes */
    short          bcWidth;     /* 2 bytes */
    short          bcHeight;    /* 2 bytes */
    unsigned short bcPlanes;    /* 2 bytes */
    unsigned short bcBitCount;  /* 2 bytes */
} BITMAPCOREHEADER;


BITMAPFILEHEADER *ReadBMFileHeader(FILE *fp);
BITMAPINFOHEADER *ReadBMInfoHeader(FILE *fp);
BITMAPCOREHEADER *ReadBMCoreHeader(FILE *fp);

int SizeOfInformationHeader(FILE *fp);

