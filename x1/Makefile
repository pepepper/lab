CROSS_COMPILE:=arm-linux-gnueabihf-
AS:=$(CROSS_COMPILE)as
CC:=$(CROSS_COMPILE)gcc

.PHONY:
all: return.bin mrc.bin c/main.bin

.PHONY:
clean:
	@rm -f *.bin spray/*.bin c/*.bin

%.bin: %.S
	@$(AS) $<
	@./extract.py a.out $@
	@rm -f a.out

c/main.bin:
	@$(CC) -nostdlib -static -fPIC -mcpu=cortex-a7 c/start.S c/main.c
	@./extract.py a.out $@
	@rm -f a.out

spray/main.bin:
	@$(AS) spray/top.S
	@./extract.py -p a.out spray/top.bin
	@$(AS) spray/bottom.S
	@./extract.py -p a.out spray/bottom.bin
	@./append_nop.py 15728640 spray/top.bin spray/bottom.bin spray/main.bin
	@rm -f a.out
