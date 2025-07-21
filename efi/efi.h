#include <stdbool.h>
#include <stdint.h>
#ifndef EFI_H
#define EFI_H

typedef uint32_t EFI_STATUS;
typedef void *EFI_HANDLE;
typedef uint64_t uintn;

#define EFIAPI __attribute__((ms_abi))

typedef struct {
  uint64_t signature;
  uint32_t revision;
  uint32_t headerSize;
  uint32_t CRC32;
  uint32_t reserved;
} EFI_TABLE_HEADER;

typedef EFI_STATUS(EFIAPI *EFI_TEXT_STRING)(void *, const unsigned short *);
typedef EFI_STATUS(EFIAPI *EFI_TEXT_RESET)(void *, bool);
typedef EFI_STATUS(EFIAPI *EFI_TEXT_QUERY_MODE)(void *, uintn modeNumber,
                                                uintn *col, uintn *row);
typedef EFI_STATUS(EFIAPI *EFI_TEXT_SET_MODE)(void *, uintn modeNumber);
typedef EFI_STATUS(EFIAPI *EFI_TEXT_SET_ATTRIBUTE)(void *, uintn attribute);
typedef EFI_STATUS(EFIAPI *EFI_TEXT_CLEAR_SCREEN)(void *);
typedef EFI_STATUS(EFIAPI *EFI_TEXT_SET_CURSOR_POSITION)(void *, uintn col,
                                                         uintn row);
typedef EFI_STATUS(EFIAPI *EFI_TEXT_ENABLE_CURSOR)(void *, bool);

typedef struct {
  EFI_TEXT_RESET reset;
  EFI_TEXT_STRING outputString;
  EFI_HANDLE NOTESTSTRING; // I'll not support this
  EFI_TEXT_QUERY_MODE queryMode;
  EFI_TEXT_SET_MODE setMode;
  EFI_TEXT_SET_ATTRIBUTE setAttr;
  EFI_TEXT_CLEAR_SCREEN clearScreen;
  EFI_TEXT_SET_CURSOR_POSITION setCursorPos;
  EFI_TEXT_ENABLE_CURSOR enableCursor;
} EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;

// EFI_SIMPLE_TEXT_INPUT_PROTOCOL

typedef struct {
  uint16_t scanCode;
  unsigned short unicodeChar;
} EFI_INPUT_KEY;

typedef EFI_STATUS(EFIAPI *EFI_INPUT_READ_KEY)(void *, EFI_INPUT_KEY *);

typedef struct {
  EFI_TEXT_RESET reset; // they are have same params and return type,
                        // so I use TEXT_OUTPUT def for it
  EFI_INPUT_READ_KEY readKeyStroke;
} EFI_SIMPLE_TEXT_INPUT_PROTOCOL;

// EFI_BOOT_SERVICES

typedef enum {
  AllocateAnyPages,
  AllocateMaxAddress,
  AllocateAddress,
  MaxAllocateType
} EFI_ALLOCATE_TYPE;

typedef enum {
  EfiReservedMemoryType,
  EfiLoaderCode,
  EfiLoaderData,
  EfiBootServicesCode,
  EfiBootServicesData,
  EfiRuntimeServicesCode,
  EfiRuntimeServicesData,
  EfiConventionalMemory,
  EfiUnusableMemory,
  EfiACPIReclaimMemory,
  EfiACPIMemoryNVS,
  EfiMemoryMappedIO,
  EfiMemoryMappedIOPortSpace,
  EfiPalCode,
  EfiPersistentMemory,
  EfiMaxMemoryType
} EFI_MEMORY_TYPE;

typedef uint64_t EFI_PHYSICAL_ADDRESS;
typedef uint64_t EFI_VIRTUAL_ADDRESS;

typedef struct {
  uint32_t Type;
  EFI_PHYSICAL_ADDRESS PhysicalStart;
  EFI_VIRTUAL_ADDRESS VirtualStart;
  uint64_t NumberOfPages;
  uint64_t Attribute;
} EFI_MEMORY_DESCRIPTOR;

typedef EFI_STATUS(EFIAPI *EFI_ALLOCATE_PAGES)(EFI_ALLOCATE_TYPE type,
                                               EFI_MEMORY_TYPE memType,
                                               uintn pages,
                                               EFI_PHYSICAL_ADDRESS *memory);
typedef EFI_STATUS(EFIAPI *EFI_FREE_PAGES)(EFI_PHYSICAL_ADDRESS mem,
                                           uintn pages);
typedef EFI_STATUS(EFIAPI *EFI_GET_MEMORY_MAP)(uintn *memoryMapSize,
                                               EFI_MEMORY_DESCRIPTOR *memMap,
                                               uintn *mapKey, uintn *descSize,
                                               uint32_t *version);
typedef EFI_STATUS(EFIAPI *EFI_ALLOCATE_POOL)(EFI_MEMORY_TYPE poolType,
                                              uintn size, void **buffer);
typedef EFI_STATUS(EFIAPI *EFI_FREE_POOL)(void *buffer);

typedef struct {
  EFI_TABLE_HEADER header;
  EFI_HANDLE EFI_RAISE_TPL;   // not support yet
  EFI_HANDLE EFI_RESTORE_TPL; // not support yet
  EFI_ALLOCATE_PAGES allocatePage;
  EFI_FREE_PAGES freePage;
  EFI_GET_MEMORY_MAP getMemMap;
  EFI_ALLOCATE_POOL allocPool;
  EFI_FREE_POOL freePool;
} EFI_BOOT_SERVICES;

typedef struct {
  EFI_TABLE_HEADER tableHeader;
  uint16_t *firmwareVendor;
  uint32_t firmwareRevision;
  EFI_HANDLE consoleInHandle;
  EFI_SIMPLE_TEXT_INPUT_PROTOCOL *consoleIn; // EFI_SIMPLE_TEXT_INPUT_PROTOCOL
  EFI_HANDLE consoleOutHandle;
  EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL
  *consoleOut; // EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL
  EFI_HANDLE standardErrorHandle;
  EFI_HANDLE stdErr;          // EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL
  EFI_HANDLE runtimeServices; // EFI_RUNTIME_SERVICES
  EFI_HANDLE bootServices;    // EFI_BOOT_SERVICES
  uint64_t numberOfTableEntries;
  EFI_HANDLE configTab; // EFI_CONFIGURATION_TABLE
} EFI_SYSTEM_TABLE;

#define EFI_MEMORY_UC 0x0000000000000001
#define EFI_MEMORY_WC 0x0000000000000002
#define EFI_MEMORY_WT 0x0000000000000004
#define EFI_MEMORY_WB 0x0000000000000008
#define EFI_MEMORY_UCE 0x0000000000000010
#define EFI_MEMORY_WP 0x0000000000001000
#define EFI_MEMORY_RP 0x0000000000002000
#define EFI_MEMORY_XP 0x0000000000004000
#define EFI_MEMORY_NV 0x0000000000008000
#define EFI_MEMORY_MORE_RELIABLE 0x0000000000010000
#define EFI_MEMORY_RO 0x0000000000020000
#define EFI_MEMORY_RUNTIME 0x80000000000000 00

#endif