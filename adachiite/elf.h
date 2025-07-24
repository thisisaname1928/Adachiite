#include "stdint.h"
#ifndef ELF_H
#define ELF_H
enum PT_VAL {
  PT_NULL = 0,
  PT_LOAD = 1,
};

typedef struct __attribute__((packed)) {
  char magic[4];
  uint8_t bit;
  uint8_t endian;
  uint8_t headerVersion;
  uint8_t ABI;
  uint8_t ABIVersion;
  char reserved[7];
  uint16_t type;
  uint16_t instructionSet;
  uint32_t ELFVersion;
  uint64_t EntryOffset;
  uint64_t programHeaderTableOffset;
  uint64_t sectionHeaderTableOffset;
  uint32_t flags;
  uint16_t headerSize;
  uint16_t programHeaderSize;
  uint16_t numberOfProgramHeaderTables;
  uint16_t sectionHeaderEntrySize;
  uint16_t numberOfSectionHeaderEntry;
  uint16_t indexOfNameSectionHeader;
} ELFHeader;

typedef struct __attribute__((packed)) {
  uint32_t type;
  uint32_t flags;
  uint64_t offset;
  uint64_t virtualAddress;
  uint64_t physicalAddress;
  uint64_t fileSize;
  uint64_t memSize;
  uint64_t align;
} ProgramHeader;

typedef struct __attribute__((packed)) {
  uint32_t name;
  uint32_t type;
  uint64_t flags;
  uint64_t address;
  uint64_t offset;
  uint64_t size;
  uint32_t link;
  uint32_t info;
  uint64_t addressAlign;
  uint64_t entSize;
} SectionHeader;
#endif