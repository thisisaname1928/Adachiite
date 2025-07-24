#include "../inc/efi.h"
#include <stdbool.h>
#ifndef ADACHIITE_H
#define ADACHIITE_H

extern EFI_FILE *root;
extern EFI_FILE *currentDir;
extern EFI_SYSTEM_TABLE *sysTab;

void print(CHAR16 *inp);
void printHex(uint64_t n);
void clearScreen();
void putchar(CHAR16 c);
void backspace();
void *memcpy(void *dest, const void *src, size_t l);
void *memmove(void *dest, const void *src, size_t l);
void *memset(void *ptr, int value, size_t num);
int memcmp(const void *ptr1, const void *ptr2, size_t num);
void *allocPool(uint64_t size);
void freePool(void *ptr);
int c16strcmp(CHAR16 *ptr1, CHAR16 *ptr2);
void listDir(EFI_FILE *root);
bool cd(CHAR16 *dirName);
bool loadFile(CHAR16 *path, void **ptr, uint64_t *size);

void shell();

#endif