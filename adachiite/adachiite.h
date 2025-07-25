#include "../inc/efi.h"
#include <stdbool.h>
#ifndef ADACHIITE_H
#define ADACHIITE_H

#define virtAddr 0xffffffff80000000

extern EFI_FILE *root;
extern EFI_FILE *currentDir;
extern EFI_SYSTEM_TABLE *sysTab;
extern void *elfEntry;
extern void *loadedBin;
extern EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;

void print(CHAR16 *inp);
void printHex(uint64_t n);
void printUint(uint64_t n);
void clearScreen();
void putchar(CHAR16 c);
void backspace();
void *memcpy(void *dest, const void *src, size_t l);
void *memmove(void *dest, const void *src, size_t l);
void *memset(void *ptr, int value, size_t num);
int memcmp(const void *ptr1, const void *ptr2, size_t num);
void *allocPool(uint64_t size);
void freePool(void *ptr);
void *allocPage(uint64_t n, EFI_MEMORY_TYPE type);
int c16strcmp(CHAR16 *ptr1, CHAR16 *ptr2);
void listDir(EFI_FILE *root);
bool cd(CHAR16 *dirName);
bool loadFile(CHAR16 *path, void **ptr, uint64_t *size);
bool loadElf(CHAR16 *path);
bool executeShell(CHAR16 *cmd);
void convertPath(CHAR16 *path);
bool mapAddress(uint64_t physicalAddress, uint64_t nPages);

void shell();

#endif