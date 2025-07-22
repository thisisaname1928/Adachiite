#include <stdbool.h>
#include <stdint.h>
#ifndef EFI_H
#define EFI_H

typedef uint32_t EFI_STATUS;
typedef void *EFI_HANDLE;
typedef void *NOT_SUPPORT;
typedef uint64_t uintn;

#define EFIAPI __attribute__((ms_abi))
#define EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID                                   \
  {0x0964e5b22,                                                                \
   0x6459,                                                                     \
   0x11d2,                                                                     \
   {0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}}

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
  uint32_t data1;
  uint16_t data2;
  uint16_t data3;
  uint8_t data4[8];
} EFI_GUID;

typedef struct _EFI_DEVICE_PATH_PROTOCOL {
  uint8_t Type;
  uint8_t SubType;
  uint8_t Length[2];
} EFI_DEVICE_PATH_PROTOCOL;

typedef enum {
  allHandles,
  byRegisterNotify,
  byProtocol
} EFI_LOCATE_SEARCH_TYPE;

typedef EFI_STATUS(EFIAPI *EFI_LOCATE_DEVICE_PATH)(
    EFI_GUID *protocol, EFI_DEVICE_PATH_PROTOCOL *devicePath,
    EFI_HANDLE *device);
typedef EFI_STATUS(EFIAPI *EFI_LOCATE_HANDLE)(EFI_LOCATE_SEARCH_TYPE type,
                                              EFI_GUID *protocol,
                                              void *searchKey,
                                              uintn *bufferSize,
                                              EFI_HANDLE *buffer);
typedef EFI_STATUS(EFIAPI *EFI_HANDLE_PROTOCOL)(EFI_HANDLE *handle,
                                                EFI_GUID *protocol,
                                                void **interface);

typedef EFI_STATUS(EFIAPI *EFI_LOCATE_HANDLE_BUFFER)(
    EFI_LOCATE_SEARCH_TYPE type, EFI_GUID *protocol, void *searchKey,
    uintn *noHandle, EFI_HANDLE **buffer);

typedef EFI_STATUS(EFIAPI *EFI_OPEN_PROTOCOL)(
    EFI_HANDLE handle, EFI_GUID *protocol, void **interface,
    EFI_HANDLE agentHandle, EFI_HANDLE controllerHandle, uint32_t attr);

typedef struct {
  EFI_TABLE_HEADER header;
  EFI_HANDLE EFI_RAISE_TPL;   // not support yet
  EFI_HANDLE EFI_RESTORE_TPL; // not support yet
  EFI_ALLOCATE_PAGES allocatePage;
  EFI_FREE_PAGES freePage;
  EFI_GET_MEMORY_MAP getMemMap;
  EFI_ALLOCATE_POOL allocPool;
  EFI_FREE_POOL freePool;
  NOT_SUPPORT createEvent;
  NOT_SUPPORT setTimer;
  NOT_SUPPORT waitForEvent;
  NOT_SUPPORT signalEvent;
  NOT_SUPPORT closeEvent;
  NOT_SUPPORT checkEvent;
  NOT_SUPPORT installProtocolInterface;
  NOT_SUPPORT reinstallProtocolInterface;
  NOT_SUPPORT uninstallProtocolInterface;
  EFI_HANDLE_PROTOCOL handleProtocol;
  void *reserved;
  NOT_SUPPORT registerProtocolNotify;
  EFI_LOCATE_HANDLE locateHandle;
  EFI_LOCATE_DEVICE_PATH locateDevicePath;
  NOT_SUPPORT installConfTable;
  NOT_SUPPORT loadImage;
  NOT_SUPPORT startImage;
  NOT_SUPPORT exit;
  NOT_SUPPORT imageUnload;
  NOT_SUPPORT exitBootServices;
  NOT_SUPPORT getNextMonotonicCount;
  NOT_SUPPORT stall;
  NOT_SUPPORT setWatchDogTimer;
  NOT_SUPPORT connectController;
  NOT_SUPPORT disconnectController;
  EFI_OPEN_PROTOCOL openProtocol;
  NOT_SUPPORT closeProtocol;
  NOT_SUPPORT openProtocolInfo;
  NOT_SUPPORT protocolsPerHandle;
  EFI_LOCATE_HANDLE_BUFFER locateHandleBuffer;
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
#define EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL 0x00000001
#define EFI_OPEN_PROTOCOL_GET_PROTOCOL 0x00000002
#define EFI_OPEN_PROTOCOL_TEST_PROTOCOL 0x00000004
#define EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER 0x00000008
#define EFI_OPEN_PROTOCOL_BY_DRIVER 0x00000010
#define EFI_OPEN_PROTOCOL_EXCLUSIVE 0x00000020

typedef struct {
  uint32_t revision;
  EFI_HANDLE parentHandle;
  EFI_SYSTEM_TABLE *systemTable;
  EFI_HANDLE deviceHandle;
  EFI_DEVICE_PATH_PROTOCOL *filePath;
  void *reserved;
  uint32_t loadOptionsSize;
  void *loadOptions;
  void *imageBase;
  uint64_t imageSize;
  EFI_MEMORY_TYPE imageCodeType;
  EFI_MEMORY_TYPE imageDataType;
  NOT_SUPPORT unload;
} EFI_LOADED_IMAGE_PROTOCOL;

// SIMPLE FS

#define EFI_FILE_MODE_READ 0x0000000000000001
#define EFI_FILE_MODE_WRITE 0x0000000000000002
#define EFI_FILE_MODE_CREATE 0x8000000000000000
#define EFI_FILE_READ_ONLY 0x0000000000000001
#define EFI_FILE_HIDDEN 0x0000000000000002
#define EFI_FILE_SYSTEM 0x0000000000000004
#define EFI_FILE_RESERVED 0x0000000000000008
#define EFI_FILE_DIRECTORY 0x0000000000000010
#define EFI_FILE_ARCHIVE 0x0000000000000020
#define EFI_FILE_VALID_ATTR 0x0000000000000037

typedef EFI_STATUS(EFIAPI *EFI_FILE_OPEN)(void *self, void **newHandle,
                                          const uint16_t *fileName,
                                          uint64_t openMode,
                                          uint64_t attribute);
typedef EFI_STATUS(EFIAPI *EFI_FILE_CLOSE)(void *self);
typedef EFI_STATUS(EFIAPI *EFI_FILE_READ)(void *self, uintn *bufferSize,
                                          void *buffer);
typedef EFI_STATUS(EFIAPI *EFI_FILE_GET_POSITION)(void *self, uint64_t *pos);
typedef EFI_STATUS(EFIAPI *EFI_FILE_SET_POSITION)(void *self, uint64_t pos);
typedef EFI_STATUS(EFIAPI *EFI_FILE_GET_INFO)(void *self,
                                              EFI_GUID *informationType,
                                              uintn *bufferSize, void *buffer);

#define FILE_BUFFER_TOO_SMALL 6
#define READ_EOF 5
// just support minimal, readonly
typedef struct {
  uint64_t revision;
  EFI_FILE_OPEN open;
  EFI_FILE_CLOSE close;
  NOT_SUPPORT del;
  EFI_FILE_READ read;
  NOT_SUPPORT write;
  EFI_FILE_GET_POSITION getPos;
  EFI_FILE_SET_POSITION setPos;
} EFI_FILE_PROTOCOL;

typedef EFI_STATUS(EFIAPI *EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_OPEN_VOLUME)(
    void *self, EFI_FILE_PROTOCOL **root);

typedef struct {
  uint64_t revision;
  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_OPEN_VOLUME openVolume;
} EFI_SIMPLE_FILE_SYSTEM_PROTOCOL;

typedef struct {
  uint16_t Year;
  uint8_t Month;
  uint8_t Day;
  uint8_t Hour;
  uint8_t Minute;
  uint8_t Second;
  uint8_t Pad1;
  uint32_t Nanosecond;
  int16_t TimeZone;
  uint8_t Daylight;
  uint8_t Pad2;
} EFI_TIME;

typedef struct {
  uint64_t Size;
  uint64_t FileSize;
  uint64_t PhysicalSize;
  EFI_TIME CreateTime;
  EFI_TIME LastAccessTime;
  EFI_TIME ModificationTime;
  uint64_t Attribute;
  uint16_t *FileName[];
} EFI_FILE_INFO;

#define EFI_FILE_READ_ONLY 0x0000000000000001
#define EFI_FILE_HIDDEN 0x0000000000000002
#define EFI_FILE_SYSTEM 0x0000000000000004
#define EFI_FILE_RESERVED 0x0000000000000008
#define EFI_FILE_DIRECTORY 0x0000000000000010
#define EFI_FILE_ARCHIVE 0x0000000000000020
#define EFI_FILE_VALID_ATTR 0x0000000000000037

#endif