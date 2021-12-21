CROSS_COMPILE := arm-linux-gnueabihf-
AS := $(CROSS_COMPILE)as
CC := $(CROSS_COMPILE)gcc
LD := $(CROSS_COMPILE)ld
OBJCOPY := $(CROSS_COMPILE)objcopy

TARGET := AppMain
OBJS := crt0.o main.o

LIBS := libbrain/libbrain.a

ASFLAGS := -W
CFLAGS := -Wall -ffreestanding -fomit-frame-pointer -Os -Ilibbrain
LDFLAGS := -T x1.ld

.PHONY:
all: $(TARGET).bin

.PHONY:
clean:
	-rm -f $(TARGET).bin $(TARGET).elf $(OBJS)

%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

%.o: %.c
	$(CC) -c $(CFLAGS) $<

$(TARGET).elf: $(OBJS)
	$(LD) $(LDFLAGS) $^ $(LIBS) -o $@

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@
