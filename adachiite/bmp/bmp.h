#include <cstdint>
#include <stdint.h>
#ifndef BMP_H
#define BMP_H

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
  uint16_t pixelSize;
  uint32_t compressionMode;
  uint32_t imageSize;
  int32_t horizontalResolution;
  int32_t veritcalResolution;
  uint32_t numberOfColors;
  uint32_t numberOfImportantColor; // idk what is this for?
} DIBBitmapInfoHeader;
#endif