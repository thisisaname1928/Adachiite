#include "adachiite.h"
#include "protocol.h"
#include <stdbool.h>
#include <stdint.h>

const EFI_MEMORY_TYPE reserved[] = {EfiReservedMemoryType, EfiUnusableMemory,
                                    EfiPalCode, EfiUnacceptedMemoryType};
const EFI_MEMORY_TYPE bootloader[] = {EfiLoaderCode, EfiLoaderData};
const EFI_MEMORY_TYPE uefi[] = {EfiBootServicesCode, EfiRuntimeServicesCode,
                                EfiBootServicesCode, EfiBootServicesData};
const EFI_MEMORY_TYPE acpi[] = {EfiACPIMemoryNVS, EfiACPIReclaimMemory};
const EFI_MEMORY_TYPE memoryMapped[] = {EfiMemoryMappedIO,
                                        EfiMemoryMappedIOPortSpace};
const EFI_MEMORY_TYPE usable[] = {EfiConventionalMemory};
const EFI_MEMORY_TYPE persistent[] = {EfiPersistentMemory};

#define TYPECHECK(typeArr, type)                                               \
  for (int i = 0; i < sizeof(typeArr) / sizeof(typeArr[0]); i++) {             \
    uint32_t t1 = input, t2 = typeArr[i];                                      \
    if (t1 == t2) {                                                            \
      return type;                                                             \
    }                                                                          \
  }
uint32_t checkMemType(EFI_MEMORY_TYPE input) {
  TYPECHECK(reserved, RESERVED);
  TYPECHECK(bootloader, BOOTLOADER);
  TYPECHECK(uefi, UEFIDATA);
  TYPECHECK(acpi, ACPI);
  TYPECHECK(memoryMapped, MEMORY_MAPPEDIO);
  TYPECHECK(usable, USABLE);
  TYPECHECK(persistent, PERSISTENT);

  return RESERVED;
}

void convertMemMapType(AdachiiteBootInfo *info) {
  EFI_MEMORY_DESCRIPTOR *ptr = (EFI_MEMORY_DESCRIPTOR *)info->memoryMap;
  uint64_t descSize = info->memoryDescSize, memMapSize = info->memoryMapSize;

  uint64_t curSize = 0;
  while (curSize < memMapSize) {
    ptr->Type = checkMemType(ptr->Type);

    ptr = (EFI_MEMORY_DESCRIPTOR *)((char *)ptr + descSize);
    curSize += descSize;
  }
}

bool adachiite(CHAR16 *kernelPath, CHAR16 *initPath) {

  print(L"start loading kernel ");
  print(kernelPath);
  print(L" with init = ");
  print(initPath);
  print(L"\n\r");

  AdachiiteBootInfo *info = allocPool(sizeof(AdachiiteBootInfo));
  info->revision = FIRST_ADACHIITE;
  info->firmwarePlatform = UEFI;

  if (!loadElf(kernelPath)) {
    print(L"error while loading kernel!\n\r");
    return false;
  }

  info->kernelPhysicalAddress = (uint64_t)loadedBin;
  info->kernelSize = kernelPages;

  print(L"loading init...\n\r");
  void *initAddress = &info; // avoid kernel from read null ptr
  uint64_t initSize;
  if (!loadFile(initPath, &initAddress, &initSize)) {
    print(L"load init failed!\n\r");
  }

  print(L"init size = ");
  printUint(initSize);
  print(L" byte(s)\n\r");
  info->init = (uint64_t)initAddress;
  info->initSize = initSize;

  info->framebuffer.framebuffer = gop->Mode->FrameBufferBase;
  info->framebuffer.colorFormat = gop->Mode->Info->PixelFormat;
  info->framebuffer.horizontalResolution =
      gop->Mode->Info->HorizontalResolution;
  info->framebuffer.verticalResolution = gop->Mode->Info->VerticalResolution;

  print(L"mapping higher half address...\n\r");
  mapAddress();
  print(L"\n\rget memory map...\n\r");
  print(L"exit boot services...");

  uint64_t memMapSize = 0, descriptorSize = 0;
  EFI_MEMORY_DESCRIPTOR *memMap;
  uint64_t mapKey;
  uint32_t descVer = 0;
  sysTab->BootServices->GetMemoryMap(&memMapSize, memMap, &mapKey,
                                     &descriptorSize, &descVer);
  memMap = allocPool(memMapSize + 60); // sometimes it increased after allocated
  memMapSize += 60;
  EFI_STATUS s = sysTab->BootServices->GetMemoryMap(
      &memMapSize, memMap, &mapKey, &descriptorSize, &descVer);

  if (s != EFI_SUCCESS) {
    print(L"get memory map failed!\n\r");
    printHex(s);
    return false;
  }

  info->memoryMap = (uint64_t)memMap;
  info->memoryDescSize = descriptorSize;
  info->memoryMapSize = memMapSize;

  convertMemMapType(info);

  s = sysTab->BootServices->ExitBootServices(imgHandle, mapKey);

  if (s != EFI_SUCCESS) {
    print(L"exit boot services failed!\n\r");
    return false;
  }

  callKernel((uint64_t)elfEntry, (uint64_t)info);

  for (;;) {
  }

  return true;
}