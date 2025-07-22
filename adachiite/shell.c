#include "adachiite.h"
#include <stdbool.h>

void executeShell(CHAR16 *cmd);

void shell() {
  CHAR16 buffer[256];
  sysTab->ConOut->EnableCursor(sysTab->ConOut, true);
  sysTab->ConIn->Reset(sysTab->ConIn, true);

  int c = 0;
  print(L"adachiite> ");
  while (true) {

    EFI_INPUT_KEY key;

    EFI_STATUS s = EFI_NOT_READY;
    while (s == EFI_NOT_READY)
      s = sysTab->ConIn->ReadKeyStroke(sysTab->ConIn, &key);

    if (key.ScanCode != 0) { // control key

      if (key.ScanCode == 0x17) { // ESC
        break;
      }

    } else {
      if (c < 256 && c >= 0) {
        if (key.UnicodeChar == 0x8) {
          if (c > 0) {
            backspace();
            c--;
          }
        } else if (key.UnicodeChar == 0xd) {
          buffer[c] = 0;
          executeShell(buffer);
          buffer[0] = 0;
          print(L"\n\radachiite> ");
          c = 0;
        } else if (c < 254) {
          buffer[c] = key.UnicodeChar;
          c++;
          putchar(buffer[c - 1]);
        }
      }
    }
  }
}

void testt(int n, CHAR16 **args);
void echo(int n, CHAR16 **args);
void cmdListDir(int n, CHAR16 **args);
void cmdCd(int n, CHAR16 **args);

void executeShell(CHAR16 *cmd) {
  CHAR16 **args = allocPool(120 * sizeof(CHAR16 *));
  int n = 0;
  for (int i = 0; cmd[i] != 0;) {
    while (cmd[i] == L' ' && cmd[i] != 0) {
      i++;
    }

    if (n > 119) {
      break;
    }

    if (cmd[i] != 0 && cmd[i] != '"') { // handle text only agrument
      args[n] = &cmd[i];
      n++;
      while (cmd[i] != 0 &&
             cmd[i] != L' ') { // loop til find a space or end of string
        i++;
      }

      if (cmd[i] != 0) { // if there is a space
        cmd[i] = 0;
        i++;
      }
    } else if (cmd[i] != 0 && cmd[i] == L'"') {
      i++; // pass "
      args[n] = &cmd[i];
      n++;
      while (cmd[i] != 0 &&
             cmd[i] != L'"') { // loop til find a " or end of string
        i++;
      }

      if (cmd[i] == '"') { // if there is a "
        cmd[i] = 0;
        i++;
      }
    }
  }

  if (n > 0) {
    print(L"\n\r");
    if (c16strcmp(args[0], L"echo")) {
      echo(n, args);
    } else if (c16strcmp(args[0], L"clear")) {
      clearScreen();
    } else if (c16strcmp(args[0], L"ls")) {
      cmdListDir(n, args);
    } else if (c16strcmp(args[0], L"cd")) {
      cmdCd(n, args);
    } else
      print(L"command not found!");
  }
  freePool(args);
}

void echo(int n, CHAR16 **args) {
  for (int i = 1; i < n; i++) {
    print(args[i]);
    print(L" ");
  }
}

void cmdListDir(int n, CHAR16 **args) {
  EFI_FILE *root = currentDir;
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

    if ((file->Attribute & EFI_FILE_DIRECTORY) == EFI_FILE_DIRECTORY)
      print(L"dir  | ");
    else
      print(L"file | ");
    sysTab->ConOut->OutputString(sysTab->ConOut, file->FileName);
    sysTab->ConOut->OutputString(sysTab->ConOut, L"\n\r");

    sysTab->BootServices->FreePool(file);
  }
}

void cmdCd(int n, CHAR16 **args) {
  if (n < 2) {
    print(L"....");
    return;
  }

  if (!cd(args[1])) {
    print(L"No such directory: ");
    print(args[1]);
  }
}

void testt(int n, CHAR16 **args) {
  for (int i = 0; i < n; i++) {
    print(L"\n\rarg: ");
    print(args[i]);
  }
}