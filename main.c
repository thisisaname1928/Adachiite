#include "inc/efi.h"
#include "inc/efiapi.h"
#include "inc/efierr.h"

EFI_STATUS efi_main(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE *sysTab) {

  sysTab->ConOut->OutputString(sysTab->ConOut, L"HIII");

  for (;;) {
  }
  return EFI_SUCCESS;
}