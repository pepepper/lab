CROSS_COMPILE := arm-linux-gnueabihf-
AS := $(CROSS_COMPILE)as
AR := $(CROSS_COMPILE)ar

TARGET := libbrain.a
OBJS := sys_closedir.o sys_feof.o sys_fflush.o sys_fgets.o sys_fputc.o sys_fread.o sys_ftell.o sys_malloc.o sys_opendir.o sys_readdir.o sys_rename.o sys_fclose.o sys_ferror.o sys_fgetc.o sys_fopen.o sys_fputs.o sys_free.o sys_fwrite.o sys_mkdir.o sys_printf.o sys_remove.o sys_stat.o

ASFLAGS := -W
ARFLAGS := -crs

.PHONY:
all: $(TARGET)

.PHONY:
clean:
	-rm -f $(TARGET) *.o

%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

$(TARGET): $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

