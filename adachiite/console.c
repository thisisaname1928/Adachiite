#include "adachiite.h"
#include <stdint.h>

// wrapper

void print(const unsigned short *str) {
  outConsole->outputString(outConsole, str);
}

void clearScreen() { outConsole->clearScreen(outConsole); }

void resetInput() { inConsole->reset(inConsole, true); }

uint16_t readKey() {
  EFI_INPUT_KEY k;

  inConsole->readKeyStroke(inConsole, &k);

  return k.unicodeChar;
}

uint16_t readKeyCode() {
  EFI_INPUT_KEY k;

  inConsole->readKeyStroke(inConsole, &k);

  return k.scanCode;
}

void enableCursor() { outConsole->enableCursor(outConsole, true); }

void printUint(uint64_t n) {
  if (n == 0) {
    print(u"0");
    return;
  }
  uint16_t buffer[20];
  buffer[19] = 0;
  int c = 18;

  while (n != 0) {
    buffer[c] = n % 10 + 48;
    n /= 10;
    c--;
  }

  print(&buffer[c + 1]);
}

uint16_t hexTab[16] = {'0', '1', '2', '3', '4', '5', '6', '7',
                       '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

void printHex(uint64_t n) {
  print(u"0x");
  if (n == 0) {
    print(u"0");
    return;
  }
  uint16_t buffer[20];
  buffer[19] = 0;
  int c = 18;

  while (n != 0) {
    buffer[c] = hexTab[n % 16];
    n /= 16;
    c--;
  }

  print(&buffer[c + 1]);
}