#include "adachiite.h"
#include <stdint.h>

void *allocPool(uint64_t size) {
  void *ptr;
  EFI_STATUS s = sysTab->BootServices->AllocatePool(EfiLoaderData, size, &ptr);

  if (s != 0) {
    print(L"\rOUT OF MEMORY!\n");
    for (;;) {
    }
  }

  return ptr;
}

void freePool(void *ptr) { sysTab->BootServices->FreePool(ptr); }

void *allocPage(uint64_t n, EFI_MEMORY_TYPE type) {
  void *ptr;
  EFI_STATUS s = sysTab->BootServices->AllocatePages(AllocateAnyPages, type, n,
                                                     (void *)&ptr);
  if (s != 0) {
    print(L"OUT OF MEMORY");
    for (;;) {
    }
  }
  return ptr;
}