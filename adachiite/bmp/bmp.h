#include "../../inc/efi.h"
#include <stdint.h>
#ifndef BMP_H
#define BMP_H

// idk why my clang doesn't packed this struct, so my reading implementation is
// a bit weird
typedef struct __attribute__((packed)) {
  char magic[2];
  uint32_t fileSize;
  uint32_t reserved;
  uint32_t offset;
} BmpHeader;

// the most common bitmap DIB
typedef struct __attribute__((packed)) {
  uint32_t headerSize;
  int32_t bitmapWidth;
  int32_t bitmapHeight;
  uint16_t colorPlane;
  uint16_t colorDepth;
  uint32_t compressionMode;
  uint32_t imageSize;
  int32_t horizontalResolution;
  int32_t veritcalResolution;
  uint32_t numberOfColors;
  uint32_t numberOfImportantColor; // idk what is this for?
} DIBBitmapInfoHeader;

// only support 24bit color, so this isn't neccessary
typedef struct __attribute__((packed)) {
  int8_t red;
  int8_t green;
  int8_t blue;
  int8_t reserved;
} ColorTableEntry;

typedef struct __attribute__((packed)) {
  BmpHeader header;
  DIBBitmapInfoHeader infoHeader;
} Bmp;

typedef struct {
  uint32_t width, height;
  uint16_t colorDepth;
  uint32_t imageSize;
  void *imageData;
} BmpImage;

bool loadBmp(CHAR16 *path, BmpImage *img);
void drawIcon(CHAR16 *path, int offsetx, int offsety);
#endif