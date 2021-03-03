CROSS_COMPILE:=arm-linux-gnueabihf-
AS:=$(CROSS_COMPILE)as
CC:=$(CROSS_COMPILE)gcc

.PHONY:
all: return.bin mrc.bin c/main.bin

.PHONY:
clean:
	@rm -f *.bin */*.bin

%.bin: %.S
	@$(AS) $<
	@./extract.py a.out $@
	@rm -f a.out

c/main.bin:
	@$(CC) -nostdlib -static -fPIC -mcpu=cortex-a7 c/start.S c/main.c
	@./extract.py a.out $@
	#@rm -f a.out
