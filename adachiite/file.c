#include "adachiite.h"
#include <stdbool.h>
#include <stdint.h>

void listDir(EFI_FILE *root) {
  if (root == NULL) {
    sysTab->ConOut->OutputString(sysTab->ConOut, L"BRUH\n\r");
    return;
  }

  root->SetPosition(root, 0);
  while (true) {
    uint64_t bufferSize = 0;
    EFI_FILE_INFO *file;

    root->Read(root, &bufferSize, file);
    if (bufferSize == 0)
      break;

    sysTab->BootServices->AllocatePool(EfiLoaderData, bufferSize,
                                       (void **)&file);
    root->Read(root, &bufferSize, file);

    sysTab->ConOut->OutputString(sysTab->ConOut, file->FileName);
    sysTab->ConOut->OutputString(sysTab->ConOut, L"\n\r");

    sysTab->BootServices->FreePool(file);
  }
}

bool cd(CHAR16 *dirName) {
  EFI_FILE *tmp;
  EFI_STATUS s = currentDir->Open(currentDir, &tmp, dirName, EFI_FILE_READ_ONLY,
                                  EFI_FILE_DIRECTORY);

  if (s != 0)
    return false;

  // check if its a dir
  EFI_FILE_INFO *info;
  EFI_GUID fileInfoGUID = EFI_FILE_INFO_ID;
  uint64_t bufferSize = 0;
  tmp->GetInfo(tmp, &fileInfoGUID, &bufferSize, info); // get bufferSize
  info = allocPool(bufferSize);
  tmp->GetInfo(tmp, &fileInfoGUID, &bufferSize, info);
  if ((info->Attribute & EFI_FILE_DIRECTORY) != EFI_FILE_DIRECTORY) {
    freePool(info);
    return false; // not a dir
  }
  freePool(info);

  currentDir->Close(currentDir);
  currentDir = tmp;
  return true;
}

void convertPath(CHAR16 *path) {
  uint64_t i = 0;
  while (path[i] != 0) {
    if (path[i] == '/')
      path[i] = '\\';
    i++;
  }
}

EFI_FILE *currentFile;
// caller should free pool ptr after using it
bool loadFile(CHAR16 *path, void **ptr, uint64_t *size) {
  convertPath(path);
  EFI_STATUS s =
      currentDir->Open(currentDir, &currentFile, path, EFI_FILE_MODE_READ, 0);
  if (s != EFI_SUCCESS)
    return false;

  EFI_FILE_INFO *info;
  EFI_GUID fileInfoGUID = EFI_FILE_INFO_ID;
  uint64_t bufferSize = 0;
  currentFile->GetInfo(currentFile, &fileInfoGUID, &bufferSize, info);
  info = allocPool(bufferSize);
  currentFile->GetInfo(currentFile, &fileInfoGUID, &bufferSize, info);

  *ptr = allocPool(info->FileSize);
  bufferSize = info->FileSize;
  *size = info->FileSize;
  currentFile->SetPosition(currentFile, 0);
  s = currentFile->Read(currentFile, &bufferSize, *ptr);
  if (s != EFI_SUCCESS) {
    return false;
  }

  freePool(info);
  currentFile->Close(currentFile);
  return true;
}