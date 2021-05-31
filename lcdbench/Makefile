CC?=arm-linux-gnueabi-gcc

all: lcdbench

.PHONY:
clean:
	rm -f lcdbench

lcdbench: lcdbench.c
	$(CC) $< -o $@

