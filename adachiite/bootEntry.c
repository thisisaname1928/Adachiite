#include "../inc/efi.h"
#include "adachiite.h"
#include <stdbool.h>
#include <stdint.h>

bool isEndDescription = false;

bool isBootEntry(CHAR16 *f) {
  int i = 0;
  while (f[i] != '.' && f[i] != 0)
    i++;

  if (f[i] == '.')
    if (f[i + 1] == 'e' && f[i + 2] == 'n' && f[i + 3] == 't')
      return true;

  return false;
}

int findBootEntries(CHAR16 *output) {
  EFI_FILE *dir;
  uint32_t c = 0;

  EFI_STATUS s = root->Open(root, &dir, L"boot", EFI_FILE_MODE_READ, 0);
  if (s != EFI_SUCCESS)
    return 0;

  // read boot dir for .ent file
  while (true) {
    uint64_t bufferSize = 0;
    EFI_FILE_INFO *curFile;

    s = dir->Read(dir, &bufferSize, curFile);
    if (s == EFI_BUFFER_TOO_SMALL) {
      curFile = allocPool(bufferSize);
      s = dir->Read(dir, &bufferSize, curFile);
      if (s != EFI_SUCCESS) {
        print(L"error while searching for boot entries!");
        break;
      }

      if (isBootEntry(curFile->FileName)) {
        print(curFile->FileName);
        print(L"\n\r");
      }

      freePool(curFile);
      continue;
    }
    // end
    break;
  }

  dir->Close(dir);
  return c;
}