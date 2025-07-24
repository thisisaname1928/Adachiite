#include "adachiite.h"
#include <stdbool.h>
#include <stdint.h>

bool executeShell(CHAR16 *cmd);
typedef struct {
  const CHAR16 *name;
  bool (*funcPtr)(int n, CHAR16 **args);
} command;

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
bool echo(int n, CHAR16 **args);
bool cmdListDir(int n, CHAR16 **args);
bool cmdCd(int n, CHAR16 **args);
bool cmdCat(int n, CHAR16 **args);

const CHAR16 echoN[] = L"echo";
bool echo(int n, CHAR16 **args) {
  for (int i = 1; i < n; i++) {
    print(args[i]);
    print(L" ");
  }

  return true;
}

const CHAR16 lsN[] = L"ls";
bool cmdListDir(int n, CHAR16 **args) {
  EFI_FILE *root = currentDir;
  if (root == NULL) {
    sysTab->ConOut->OutputString(sysTab->ConOut, L"BRUH\n\r");
    return false;
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
  return true;
}

const CHAR16 cdN[] = L"cd";
bool cmdCd(int n, CHAR16 **args) {
  if (n < 2) {
    print(L"....");
    return false;
  }

  if (!cd(args[1])) {
    print(L"No such directory: ");
    print(args[1]);
  }

  return true;
}

const CHAR16 catN[] = L"cat";
bool cmdCat(int n, CHAR16 **args) {
  if (n < 2) {
    return false;
  }

  char *buf;
  uint64_t size;
  bool s = loadFile(args[1], (void **)&buf, &size);
  if (!s) {
    print(L"No such file: ");
    print(args[1]);
    return false;
  }

  for (uint64_t i = 0; i < size; i++) {
    CHAR16 x[2];
    x[1] = 0;
    x[0] = buf[i];
    print(x);
  }

  freePool(buf);
  return true;
}

void testt(int n, CHAR16 **args) {
  for (int i = 0; i < n; i++) {
    print(L"\n\rarg: ");
    print(args[i]);
  }
}

CHAR16 helpN[] = L"help";
bool cmdHelp(int n, CHAR16 **args);
CHAR16 loadElfN[] = L"loadelf";
bool cmdLoadELF(int n, CHAR16 **args);
CHAR16 mapAddrN[] = L"mapaddr";
bool cmdMapAddr(int n, CHAR16 **args);
CHAR16 clrN[] = L"clear";
bool cmdClearScreen(int n, CHAR16 **args);

#define NEW_COMMAND(cName, func) {.name = cName, .funcPtr = func}
command cmds[] = {
    NEW_COMMAND(echoN, echo),          NEW_COMMAND(lsN, cmdListDir),
    NEW_COMMAND(cdN, cmdCd),           NEW_COMMAND(catN, cmdCat),
    NEW_COMMAND(helpN, cmdHelp),       NEW_COMMAND(loadElfN, cmdLoadELF),
    NEW_COMMAND(mapAddrN, cmdMapAddr), NEW_COMMAND(clrN, cmdClearScreen)};
#define cmdLength 8

bool cmdClearScreen(int n, CHAR16 **args) {
  clearScreen();
  return true;
}

bool cmdHelp(int n, CHAR16 **args) {
  print(L"available commands: ");
  for (int i = 0; i < cmdLength; i++) {
    print((CHAR16 *)cmds[i].name);
    print(L" ");
  }

  return true;
}

bool cmdLoadELF(int n, CHAR16 **args) {
  if (n < 2) {
    return false;
  }

  bool s = loadElf(args[1]);
  if (!s) {
    print(L"load ELF ");
    print(args[1]);
    print(L" failed!");
  }

  return s;
}

bool cmdMapAddr(int n, CHAR16 **args) {
  mapAddress((uint64_t)loadedBin, 2);
  return true;
}

bool executeShell(CHAR16 *cmd) {
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

    for (int i = 0; i < cmdLength; i++) {
      if (c16strcmp(args[0], (CHAR16 *)cmds[i].name)) {
        bool res = cmds[i].funcPtr(n, args);
        freePool(args);
        return res;
      }
    }

    print(L"command not found: ");
    print(args[0]);
  }
  return false;
}