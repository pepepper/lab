CROSS_COMPILE:=arm-linux-gnueabihf-
AS:=$(CROSS_COMPILE)as
CC:=$(CROSS_COMPILE)gcc
STRIP:=$(CROSS_COMPILE)strip

.PHONY:
all: return.bin mrc.bin c/main.bin

.PHONY:
clean:
	@rm -f *.bin spray/*.bin c/*.bin c/*.elf injector/*.bin injector/*.elf

%.bin: %.S
	@$(AS) $<
	@./extract.py a.out $@
	@rm -f a.out

c/main.elf:
	@$(CC) -nostdlib -static -fPIC -marm -O0 c/start.S c/main.c c/scg.c c/divmod.c c/divmod.S -o c/main.elf

c/main.bin: c/main.elf
	@./extract.py c/main.elf $@

c/uboot_loader.bin:
	@if [ "$(UBOOT)" = "" ]; then \
		echo "Please specify UBOOT."; \
		exit 1; \
	fi
	@c/generate.py $(UBOOT) c/uboot_bin.h.tpl c/uboot_bin.h
	@$(CC) -nostdlib -static -fPIC -marm -O0 c/start.S c/main_uboot.c -o c/uboot_loader.elf
	@./extract.py c/uboot_loader.elf $@

spray/main.bin:
	@$(AS) spray/top.S
	@./extract.py -p a.out spray/top.bin
	@$(AS) spray/bottom.S
	@./extract.py -p a.out spray/bottom.bin
	@$(AS) spray/bottom_reset.S
	@./extract.py -p a.out spray/bottom_reset.bin
	@./append_nop.py spray/top.bin spray/bottom.bin spray/bottom_reset.bin spray/main.bin 112 113
	@rm -f a.out

injector/AppMain.bin:
	@if [ "$(ELF)" = "" ]; then \
		echo "Please specify ELF."; \
		exit 1; \
	fi
	@$(STRIP) $(ELF)
	@./extract.py $(ELF) injector/injected.bin
	@$(AS) injector/disable_mmu.S -o injector/disable_mmu.elf
	@./extract.py -p injector/disable_mmu.elf injector/disable_mmu.bin
	@./injector/inject.py 0xf00000 0x700000 injector/disable_mmu.bin injector/injected.bin injector/AppMain.bin

injector/uboot.bin:
	@$(AS) injector/disable_mmu.S -o injector/disable_mmu.elf
	@./extract.py -p injector/disable_mmu.elf injector/disable_mmu.bin
	@./extract.py -p injector/disable_mmu.elf injector/disable_mmu.bin
	@./injector/inject.py 0xed0000 0x700000 injector/disable_mmu.bin u-boot.bin injector/uboot.bin
