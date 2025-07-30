#include "adachiite.h"
#include <stdbool.h>
#include <stdint.h>

#define PRESENT 1
#define RW (1 << 1)
#define HUGE_PAGE (1 << 7)

uint64_t createPageEntry(uint64_t address, uint64_t attributes) {
  return address | attributes;
}

extern __attribute__((sysv_abi)) uint64_t getCr3();
extern void invlpgM(uint64_t);

uint64_t *mapPT(uint64_t baseAddress) {
  uint64_t *PT = allocPage(1, EfiBootServicesData);
  for (int i = 0; i < 512; i++) {
    PT[i] = createPageEntry(baseAddress, PRESENT | RW);
    baseAddress += 0x1000;
  }

  return PT;
}

bool mapAddress() {
  uint64_t *PML4Addr = (uint64_t *)getCr3();

  if ((PML4Addr[511] & 1) == 1)
    print(L"PML4 is present!\n\r");
  // copy PML4
  uint64_t *PML4 = allocPage(1, EfiLoaderData);
  for (int i = 0; i < 512; i++)
    PML4[i] = PML4Addr[i];

  // map PD
  uint64_t *PD = allocPage(1, EfiLoaderData);
  uint64_t offset = (uint64_t)loadedBin;
  for (int i = 0; i < 512; i++) {
    PD[i] = createPageEntry((uint64_t)mapPT((uint64_t)offset), PRESENT | RW);
    offset += 0x200000;
  }
  printHex(PD[0]);
  // map PDP
  uint64_t *PDP = allocPage(1, EfiLoaderData);
  for (int i = 0; i < 512; i++) {
    PDP[i] = 0;
  }
  PDP[510] = createPageEntry((uint64_t)PD, PRESENT | RW);
  PML4[511] = createPageEntry((uint64_t)PDP, PRESENT | RW);

  invlpgM((uint64_t)PML4);
  print(L"the hell\n\r");

  CHAR16 *c = (CHAR16 *)(0xffffffff80000000);
  printHex(*c);

  print(L"\n\rok!");

  return true;
}