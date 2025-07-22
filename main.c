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

  EFI_GUID sfs = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
  uint64_t n = 0;
  EFI_HANDLE *h;
  EFI_STATUS s = bootServices->locateHandleBuffer(byProtocol, &sfs, 0, &n, &h);

  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *interface;
  s = bootServices->openProtocol(h[0], &sfs, (void *)&interface, imageHandle, 0,
                                 EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);

  print(u"\n");

  if (s != 0) {
    print(u"UUUUUU");
  }

  EFI_FILE_PROTOCOL *f;
  s = interface->openVolume(interface, &f);
  uintn bf = 90;
  EFI_FILE_INFO *buffer = allocPool(90);
  uint64_t currentPos = 0;

  f->setPos(f, 2);
  s = f->read(f, &bf, buffer);
  printUint(bf);
  if (s != 0)
    print(u"HAI");
  // s = f->read(f, &bf, buffer);

  print(buffer->FileName);

  print(u"\n\r");

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