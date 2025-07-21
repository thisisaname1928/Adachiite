#include "../efi/efi.h"
#include "adachiite.h"
#include <stdint.h>

// 4KB page
uint64_t allocPage(uint64_t pageN) {
  EFI_PHYSICAL_ADDRESS p;
  EFI_STATUS s =
      bootServices->allocatePage(AllocateAnyPages, EfiLoaderData, pageN, &p);

  if (s != 0) {
    return 0;
  }

  return p;
}

void freePage(EFI_PHYSICAL_ADDRESS p, uint64_t pageSize) {
  bootServices->freePage(p, pageSize);
}

uint64_t memoryMapKey = 0;
uint64_t descriptionSize = 0;
uint64_t memoryMapSize = 0;
uint32_t descriptionVer = 0;
uint64_t getMemoryMap() {
  uint64_t bufferSize = 0x1000; // 4KB
  EFI_PHYSICAL_ADDRESS p = allocPage(1);
  EFI_PHYSICAL_ADDRESS last = p;

  EFI_STATUS s =
      bootServices->getMemMap(&bufferSize, (EFI_MEMORY_DESCRIPTOR *)&p,
                              &memoryMapKey, &descriptionSize, &descriptionVer);
  while (s == 5) { // memoryMap bigger than current buffer
    // alloc more pages
    freePage(last, bufferSize / 0x1000);
    bufferSize += 0x1000;
    p = allocPage(bufferSize / 0x1000);
    if (p == 0) {
      print(u"OUT OF MEMORY");
      for (;;) {
      }
    }
    last = p;

    s = bootServices->getMemMap(&bufferSize, (EFI_MEMORY_DESCRIPTOR *)&p,
                                &memoryMapKey, &descriptionSize,
                                &descriptionVer);
    printUint(s);
  }

  //   if (s != 0) {
  //     print(u"Out of memory\n");
  //     for (;;) {
  //     }
  //   }

  memoryMapSize = bufferSize;

  return p;
}

EFI_MEMORY_DESCRIPTOR *nextDescriptor(EFI_MEMORY_DESCRIPTOR *current) {
  char *next = (char *)current + descriptionSize;

  return (EFI_MEMORY_DESCRIPTOR *)next;
}