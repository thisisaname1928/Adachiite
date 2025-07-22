CC=clang
CC_FLAGS=-target x86_64-unknown-windows -ffreestanding -fshort-wchar -mno-red-zone 
LD=clang
LD_FLAGS=-target x86_64-unknown-windows -nostdlib -Wl,-entry:efi_main -Wl,-subsystem:efi_application -fuse-ld=lld-link
CC_SRC=$(shell find . -type f -name "*.c")
CC_OBJ=$(patsubst %, %.o, $(CC_SRC))
OUTPUT=BOOTX64.EFI

test: test.img
	@qemu-system-x86_64 -L ./x64 -pflash ./x64/OVMF_CODE.4m.fd $< -enable-kvm -no-reboot

test.img: $(OUTPUT)
	@dd if=/dev/zero of=$@ bs=1M count=50
	@mkfs.fat -F 32 $@
	@mmd -i $@ ::/EFI
	@mmd -i $@ ::/EFI/BOOT
	@mcopy -i $@ $< ::/EFI/BOOT
	@mcopy -i $@ boot ::

$(OUTPUT): $(CC_OBJ)
	@echo "Linking -> $@"
	@$(LD) $(CC_OBJ) -o $@ $(LD_FLAGS)

%.c.o: %.c
	@echo "CC $<"
	@$(CC) $(CC_FLAGS) -c $< -o $@

clean:
	@rm -f $(shell find . -type f -name "*.o") test.img $(OUTPUT) 
