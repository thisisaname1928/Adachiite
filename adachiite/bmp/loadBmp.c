#include "../adachiite.h"
#include "../elf.h"
#include "bmp.h"
#include <stdbool.h>
#include <stdint.h>

bool loadBmp(CHAR16 *path, BmpImage *img) {
  char *buffer;
  uint64_t size = 0;
  // icon file usually isn't big, so I load all of the file into memory
  int s = loadFile(path, (void **)&buffer, &size);
  if (!s)
    return false;

  Bmp *image = (Bmp *)buffer;
  DIBBitmapInfoHeader *infoHeader = (DIBBitmapInfoHeader *)(buffer + 14);
  if (image->header.magic[0] != 'B' || image->header.magic[1] != 'M')
    return false;

  img->width = infoHeader->bitmapWidth;
  img->height = infoHeader->bitmapHeight;
  img->colorDepth = infoHeader->colorDepth;
  void *imgBuffer = allocPool(infoHeader->imageSize);

  uint64_t offset = *(uint32_t *)((char *)buffer + 10);
  for (uint32_t i = 0; i < infoHeader->imageSize; i += 4) {
    uint32_t *dest = (uint32_t *)((char *)imgBuffer + i),
             *src = (uint32_t *)(buffer + offset + i);

    *dest = *src;
  }

  img->imageData = imgBuffer;
  img->imageSize = image->infoHeader.imageSize;

  freePool(buffer);

  return true;
}

uint32_t flipColor(uint32_t color) {
  uint32_t r = (color >> 16) & 0xff, g = (color >> 8) & 0xff, b = color & 0xff;
  return (b << 16) | (g << 8) | r;
}

void drawIcon(CHAR16 *path, int offsetx, int offsety) {
  BmpImage image;
  if (!loadBmp(path, &image)) {
    print(L"sus bmp file!");
    return;
  }

  void *offset = image.imageData;
  uint32_t rowSize = image.imageSize / image.height; // avoid padding
  if (image.colorDepth == 32) {
    uint32_t *color = (uint32_t *)offset;

    for (uint32_t y = image.height; y > 0; y--) {
      for (uint32_t x = 0; x < image.width; x++) {
        if (gop->Mode->Info->PixelFormat ==
            PixelBlueGreenRedReserved8BitPerColor)
          putPixel(*color, x + offsetx,
                   y + offsety); // trunc out
        else
          putPixel(flipColor(*color), x + offsetx, y + offsety);
        // 32 bit = 4 bytes
        color++;
      }
    }
  }
}