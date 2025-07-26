#include "adachiite.h"
#include <stdint.h>
EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;

void putPixel(uint32_t color, int x, int y) {
  uint32_t *ptr =
      (uint32_t *)(gop->Mode->FrameBufferBase +
                   4 * gop->Mode->Info->PixelsPerScanLine * y + 4 * x);
  *ptr = color;
}

void printGOPMode(EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *info) {
  printUint(info->HorizontalResolution);
  print(L"x");
  printUint(info->VerticalResolution);
  print(L" | ");
}

void queryGOPModes() {
  if (gop == NULL) {
    print(L"should init gop before call query modes!\n\r");
    return;
  }

  uint64_t bufferSize = 1;
  EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *info;
  uint64_t i = 0;
  for (uint32_t i = 0; i < gop->Mode->MaxMode; i++) {
    EFI_STATUS s = gop->QueryMode(gop, i, &bufferSize, &info);
    if (s != EFI_SUCCESS)
      continue;

    printUint(i);
    print(L": ");
    printGOPMode(info);
  }
}

bool initGOP() {
  print(L"ok!\n\r");
  print(L"current mode: ");
  printUint(gop->Mode->Info->HorizontalResolution);
  putchar('x');
  printUint(gop->Mode->Info->VerticalResolution);
  print(L"\n\r");
  queryGOPModes();

  for (int x = 400; x < 500; x++)
    for (int y = 200; y < 300; y++)
      putPixel(0xfffffff, x, y);

  return true;
}

bool cmdInitGop(int n, CHAR16 **args) { return initGOP(); }