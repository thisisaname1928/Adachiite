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
  uint64_t bufferSize = 0x0;
  EFI_PHYSICAL_ADDRESS p = 0x200000; // let this sound legit
  uint64_t sus;

  // fetch for size
  EFI_STATUS s = bootServices->getMemMap(
      &bufferSize, (EFI_MEMORY_DESCRIPTOR *)p, &sus, &sus, (uint32_t *)&sus);

  memoryMapKey = 0;
  memoryMapSize = 0x0;
  if (s == 5) {
    uint64_t numOfPages = 0;
    while (memoryMapSize < bufferSize) {
      memoryMapSize += 0x1000;
      numOfPages++;
    }
    memoryMapSize += 0x1000;

    p = allocPage(numOfPages + 1);
    s = bootServices->getMemMap(&memoryMapSize, (EFI_MEMORY_DESCRIPTOR *)p,
                                &memoryMapKey, &descriptionSize,
                                &descriptionVer);
  }

  if (s != 0) {
    print(u"Out of memory\n");
    for (;;) {
    }
  }

  memoryMapSize = bufferSize;

  return p;
}

EFI_MEMORY_DESCRIPTOR *nextDescriptor(EFI_MEMORY_DESCRIPTOR *current) {
  char *next = (char *)current + descriptionSize;

  return (EFI_MEMORY_DESCRIPTOR *)next;
}