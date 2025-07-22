#include "adachiite/adachiite.h"
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

  shell();

  for (;;) {
  }
  return EFI_SUCCESS;
}