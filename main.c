#include "adachiite/adachiite.h"
#include "adachiite/bmp/bmp.h"
#include "inc/efi.h"
#include "inc/efiapi.h"
#include "inc/efierr.h"
#include "inc/efiprot.h"
#include <stdbool.h>
#include <stdint.h>

EFI_FILE *root;
EFI_FILE *currentDir;
EFI_SYSTEM_TABLE *sysTab;

EFI_STATUS efi_main(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE *systemTable) {
  sysTab = systemTable;
  sysTab->ConOut->OutputString(sysTab->ConOut, L"HIII\n\r");

  // open root
  EFI_LOADED_IMAGE_PROTOCOL *loadedImg;
  EFI_GUID LIPGUID = EFI_LOADED_IMAGE_PROTOCOL_GUID;
  sysTab->BootServices->HandleProtocol(imageHandle, &LIPGUID,
                                       (void **)&loadedImg);
  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *fsP;
  EFI_GUID FSG = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
  sysTab->BootServices->HandleProtocol(loadedImg->DeviceHandle, &FSG,
                                       (void **)&fsP);
  fsP->OpenVolume(fsP, &root);
  fsP->OpenVolume(fsP, &currentDir);

  clearScreen();
  EFI_GUID gopGUID = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;

  EFI_STATUS s =
      sysTab->BootServices->LocateProtocol(&gopGUID, NULL, (void **)&gop);

  if (s != EFI_SUCCESS) {
    print(L"init GOP failed!\n\r");
    return false;
  }

  if (gop->Mode->Info->PixelFormat == PixelRedGreenBlueReserved8BitPerColor)
    print(L"current pixel format: PixelRedGreenBlueReserved8BitPerColor\n\r");
  else if (gop->Mode->Info->PixelFormat ==
           PixelBlueGreenRedReserved8BitPerColor)
    print(L"current pixel format: PixelBlueGreenRedReserved8BitPerColor\n\r");

  drawIcon(L"boot/icon.bmp",
           gop->Mode->Info->VerticalResolution -
               gop->Mode->Info->HorizontalResolution / 40,
           gop->Mode->Info->HorizontalResolution / 20);

  shell();

  print(L"quit shell!");

  for (;;) {
  }
  return EFI_SUCCESS;
}