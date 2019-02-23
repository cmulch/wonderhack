#include "FreeImage.h"
#include <stdio.h>

int main(int argc, char** argv)
{
  FreeImage_Initialise(TRUE);

  FIBITMAP *dib = FreeImage_Load(FIF_JPEG, "cat.jpg", JPEG_ACCURATE);

  printf("%d\n", FreeImage_GetWidth(dib));
  printf("%d\n", FreeImage_GetHeight(dib));

  FreeImage_Unload(dib);
}
