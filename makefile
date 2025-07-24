CC=clang
ASM=nasm
CC_FLAGS=-target x86_64-unknown-windows -ffreestanding -fshort-wchar -mno-red-zone 
LD=clang
LD_FLAGS=-target x86_64-unknown-windows -nostdlib -Wl,-entry:efi_main -Wl,-subsystem:efi_application -fuse-ld=lld-link
CC_SRC=$(shell find . -type f -name "*.c")
ASM_SRC=$(shell find . -type f -name "*.asm")
CC_OBJ=$(patsubst %, %.o, $(CC_SRC))
ASM_OBJ=$(patsubst %, %.o, $(ASM_SRC))
OUTPUT=BOOTX64.EFI

test: test.img
	@qemu-system-x86_64 -L ./x64 -pflash ./x64/OVMF.4m.fd $< -enable-kvm -no-reboot -m 1G -D log.txt

test.img: $(OUTPUT)
	@dd if=/dev/zero of=$@ bs=1M count=50
	@mkfs.fat -F 32 $@
	@mmd -i $@ ::/EFI
	@mmd -i $@ ::/EFI/BOOT
	@mcopy -i $@ $< ::/EFI/BOOT
	@mcopy -i $@ boot ::

$(OUTPUT): $(CC_OBJ) $(ASM_OBJ)
	@echo "Linking -> $@"
	@$(LD) $(CC_OBJ) $(ASM_OBJ) -o $@ $(LD_FLAGS)

%.asm.o:  %.asm
	@echo "ASM $<"
	@$(ASM) -f win64 $< -o $@

%.c.o: %.c
	@echo "CC $<"
	@$(CC) $(CC_FLAGS) -c $< -o $@

clean:
	@rm -f $(shell find . -type f -name "*.o") test.img $(OUTPUT) 
