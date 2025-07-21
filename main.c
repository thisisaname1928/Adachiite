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

  uint64_t *s = allocPool(sizeof(uint64_t));
  *s = 0xffffff;
  printHex(*s);
  freePool(s);

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