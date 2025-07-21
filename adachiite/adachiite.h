#include "../efi/efi.h"
#include <stdint.h>
#ifndef ADACHIITE_H
#define ADACHIITE_H

extern EFI_SYSTEM_TABLE *sysTab;
extern EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *outConsole;
extern EFI_SIMPLE_TEXT_INPUT_PROTOCOL *inConsole;
extern EFI_BOOT_SERVICES *bootServices;

#define UP_KEY 0x01
#define DOWN_KEY 0x02

void print(const unsigned short *str);
void clearScreen();
void resetInput();
uint16_t readKey();
uint16_t readKeyCode();
void enableCursor();
void printUint(uint64_t n);
void printHex(uint64_t n);
uint64_t allocPage(uint64_t pageN);
void freePage(EFI_PHYSICAL_ADDRESS p, uint64_t pageSize);
uint64_t getMemoryMap();
EFI_MEMORY_DESCRIPTOR *nextDescriptor(EFI_MEMORY_DESCRIPTOR *current);
void *allocPool(uint64_t size);
void freePool(void *ptr);

extern uint64_t memoryMapKey;
extern uint64_t descriptionSize;
extern uint64_t memoryMapSize;
extern uint32_t descriptionVer;

#endif