#include "adachiite/adachiite.h"
#include "efi/efi.h"
#include <stdint.h>

void hlt() {
  for (;;) {
    __asm__ volatile("hlt");
  }
}

EFI_SYSTEM_TABLE *sysTab;
EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *outConsole;
EFI_SIMPLE_TEXT_INPUT_PROTOCOL *inConsole;
EFI_BOOT_SERVICES *bootServices;

EFI_STATUS EFIAPI efi_main(EFI_HANDLE imageHandle,
                           EFI_SYSTEM_TABLE *systemTable) {
  sysTab = systemTable;
  outConsole = sysTab->consoleOut;
  inConsole = sysTab->consoleIn;
  bootServices = sysTab->bootServices;

  enableCursor();
  clearScreen();

  EFI_MEMORY_DESCRIPTOR *memMap = (EFI_MEMORY_DESCRIPTOR *)getMemoryMap();

  // for (int i = 0; i < memoryMapSize / descriptionSize; i++) {
  //   EFI_MEMORY_DESCRIPTOR *next = nextDescriptor(memMap);
  //   printHex(memMap->VirtualStart);
  //   print(u"\r\n");
  //   memMap = nextDescriptor(memMap);
  // }

  print(u"\r\nadachiite> ");
  resetInput();

  while (true) {
    uint16_t a[2];
    a[1] = 0;

    a[0] = readKey();
    print(a);
  }

  hlt();
  return 0;
}