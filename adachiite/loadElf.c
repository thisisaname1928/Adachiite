#include "adachiite.h"
#include "elf.h"
#include <stdbool.h>
#include <stdint.h>

#define CHECK_EFI_STATUS(s)                                                    \
  if (s != EFI_SUCCESS)                                                        \
    return false;

void *elfEntry;
void *loadedBin;
uint64_t kernelPages = 0;
void fillBss(char *ptr, uint64_t fileSize, uint64_t memSize) {
  // use char* to prevent we delete somethings wrong
  ptr += fileSize;
  uint64_t n = memSize - fileSize;
  for (uint64_t i = 0; i < n; i++) {
    ptr[i] = 0;
  }
}

bool loadElf(CHAR16 *path) {
  EFI_FILE *file;
  convertPath(path);
  EFI_STATUS s =
      currentDir->Open(currentDir, &file, path, EFI_FILE_MODE_READ, 0);

  if (s != EFI_SUCCESS) {
    return false;
  }

  EFI_FILE_INFO *info;
  EFI_GUID fileInfoGUID = EFI_FILE_INFO_ID;
  uint64_t bufferSize = 0;

  file->GetInfo(file, &fileInfoGUID, &bufferSize, info);
  info = allocPool(bufferSize);
  CHECK_EFI_STATUS(file->GetInfo(file, &fileInfoGUID, &bufferSize, info));

  // check dir bit
  if ((info->Attribute & EFI_FILE_DIRECTORY) == EFI_FILE_DIRECTORY)
    return false;

  freePool(info);

  // check elf header
  ELFHeader header;
  bufferSize = sizeof(ELFHeader);
  file->SetPosition(file, 0);
  CHECK_EFI_STATUS(file->Read(file, &bufferSize, &header));
  char elfMagic[] = {0x7f, 'E', 'L', 'F'};
  if (memcmp(header.magic, elfMagic, 4) != 0) {
    print(L"file ");
    print(path);
    print(L" is not a ELF binary!");
    return false;
  }

  // read program header
  bufferSize = header.programHeaderSize * header.numberOfProgramHeaderTables;
  ProgramHeader *programHeaders = allocPool(bufferSize);
  file->SetPosition(file, header.programHeaderTableOffset);
  CHECK_EFI_STATUS(file->Read(file, &bufferSize, programHeaders));

  // find the largest virt memory address
  uint64_t largestMemAdd = 0;
  uint64_t largestMemSize =
      0; // use for calculating number of pages to allocate
  for (int i = 0; i < header.numberOfProgramHeaderTables; i++) {
    if (programHeaders[i].type != PT_LOAD)
      continue;

    print(L"LOAD: VirtAddr=");
    printHex(programHeaders[i].virtualAddress);
    print(L"\n\r");

    if (programHeaders[i].virtualAddress > largestMemAdd) {
      largestMemAdd = programHeaders[i].virtualAddress;
      largestMemSize = largestMemAdd + programHeaders[i].memSize - virtAddr;
    }
  }

  uint64_t nPages = (largestMemSize / 0x1000) + 2;
  kernelPages = nPages;
  print(L"alloc ");
  printUint(nPages);
  print(L" page(s), first page begin at ");
  void *ptr = allocPage(nPages, EfiLoaderData);
  loadedBin = ptr;
  printHex((uint64_t)ptr);
  print(L"\n\r");

  // load
  for (int i = 0; i < header.numberOfProgramHeaderTables; i++) {
    if (programHeaders[i].type != PT_LOAD)
      continue;

    CHECK_EFI_STATUS(file->SetPosition(file, programHeaders[i].offset));
    uint64_t bufferSize = programHeaders[i].fileSize;
    void *curPtr = (char *)ptr + (programHeaders[i].virtualAddress - virtAddr);
    CHECK_EFI_STATUS(file->Read(file, &bufferSize, curPtr));
    fillBss(curPtr, programHeaders[i].fileSize, programHeaders[i].memSize);
  }

  elfEntry = (void *)header.EntryOffset;

  file->Close(file);
  return true;
}