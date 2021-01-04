#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <time.h>

#define PINCTRL_REG_BASE 0x80018000

#define PINCTRL_BANK2_PULLUP_REG 0x620
#define PINCTRL_BANK4_PULLUP_REG 0x640

#define PINCTRL_DRIVE10_REG 0x3A0
#define PINCTRL_DRIVE17_REG 0x410

#define PINCTRL_BANK2_OUTPUT_REG 0x720
#define PINCTRL_BANK4_OUTPUT_REG 0x740

#define PINCTRL_BANK2_OUTPUT_ENABLE_REG 0xB20
#define PINCTRL_BANK4_OUTPUT_ENABLE_REG 0xB40

#define PINCTRL_BANK4_INPUT_REG 0x940

#define REG_SET 0x4
#define REG_CLR 0x8

void keyread(void *mapped_addr, char *keys) {
    unsigned int in;
    struct timespec wait = {0, 100000};
    // Bank2 Pin16~21
    for (int i = 0; i < 7; i++) {
        *(volatile unsigned int *)(mapped_addr + PINCTRL_BANK2_OUTPUT_REG + REG_SET) = 0x10000 << i;
        nanosleep(&wait, NULL);
        in = *(volatile unsigned int *)(mapped_addr + PINCTRL_BANK4_INPUT_REG);
        in = ~((in ^ (in >> 1)) & 0x1f ^ (in >> 1)) & 0x7f;
        *(volatile unsigned int *)(mapped_addr + PINCTRL_BANK2_OUTPUT_REG + REG_CLR) = 0x10000 << i;
        nanosleep(&wait, NULL);
        keys[i] = (char)in;
    }
    // Bank4 Pin8
    *(volatile unsigned int *)(mapped_addr + PINCTRL_BANK4_OUTPUT_REG + REG_SET) = 0x100;
    nanosleep(&wait, NULL);
    in = *(volatile unsigned int *)(mapped_addr + PINCTRL_BANK4_INPUT_REG);
    in = ~((in ^ (in >> 1)) & 0x1f ^ (in >> 1)) & 0x7f;
    *(volatile unsigned int *)(mapped_addr + PINCTRL_BANK4_OUTPUT_REG + REG_CLR) = 0x100;
    nanosleep(&wait, NULL);
    keys[6] = (char)in;
}

int main() {
    /* init */
    int fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (fd < 0) {
        printf("failed to open /dev/mem");
        return -1;
    }

    void *mapped_addr = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, PINCTRL_REG_BASE);
    if (mapped_addr == MAP_FAILED) {
        printf("failed to mapping addr");
        close(fd);
        return -1;
    }

    /* GPIO setup */
    *(volatile unsigned int *)(mapped_addr + PINCTRL_DRIVE10_REG + REG_SET) = 0x00444444;
    *(volatile unsigned int *)(mapped_addr + PINCTRL_DRIVE17_REG + REG_SET) = 0x44444444;
    *(volatile unsigned int *)(mapped_addr + PINCTRL_BANK2_PULLUP_REG + REG_SET) = 0x003F03FF;
    *(volatile unsigned int *)(mapped_addr + PINCTRL_BANK2_OUTPUT_REG + REG_SET) = 0x00000000;
    *(volatile unsigned int *)(mapped_addr + PINCTRL_BANK4_OUTPUT_REG + REG_SET) = 0x00000000;
    *(volatile unsigned int *)(mapped_addr + PINCTRL_BANK2_OUTPUT_ENABLE_REG + REG_SET) =
        0x0F3FF5FF;
    *(volatile unsigned int *)(mapped_addr + PINCTRL_BANK4_OUTPUT_ENABLE_REG + REG_SET) =
        0x0001FF20;

    /* read */
    char keys[7], oldkeys[7] = {0};
    while (1) {
        keyread(mapped_addr, keys);
        if (memcmp(keys, oldkeys, sizeof(keys))) {
            memcpy(oldkeys, keys, sizeof(keys));
            continue;
        }
        unsigned char temp = 0;
        for (int i = 0; i < 7; i++) {
            temp |= keys[i];
        }
        if (temp == 0)
            continue;
        // key decode
        if (temp & 0x1)
            for (int i = 0; i < 7; i++)
                if ((keys[i] & 0x1) == 0) {
                    if (i == 0)
                        printf("英和和英 ");
                    if (i == 1)
                        printf("調べる ");
                    if (i == 2)
                        printf("国語漢字 ");
                    if (i == 3)
                        printf("My辞書 ");
                    if (i == 4)
                        printf("履歴/しおり ");
                    if (i == 5)
                        printf("マーカーテスト ");
                    if (i == 6)
                        printf("HOME ");
                }
        if (temp & 0x2)
            for (int i = 0; i < 7; i++)
                if ((keys[i] & 0x2) == 0) {
                    if (i == 0)
                        printf("Q ");
                    if (i == 1)
                        printf("W ");
                    if (i == 2)
                        printf("E ");
                    if (i == 3)
                        printf("T ");
                    if (i == 4)
                        printf("Y ");
                    if (i == 5)
                        printf("U ");
                    if (i == 6)
                        printf("I ");
                }
        if (temp & 0x4)
            for (int i = 0; i < 7; i++)
                if ((keys[i] & 0x4) == 0) {
                    if (i == 0)
                        printf("A ");
                    if (i == 1)
                        printf("S ");
                    if (i == 2)
                        printf("D ");
                    if (i == 3)
                        printf("G ");
                    if (i == 4)
                        printf("H ");
                    if (i == 5)
                        printf("O ");
                    if (i == 6)
                        printf("P ");
                }
        if (temp & 0x8)
            for (int i = 0; i < 7; i++)
                if ((keys[i] & 0x8) == 0) {
                    if (i == 0)
                        printf("Z ");
                    if (i == 1)
                        printf("X ");
                    if (i == 2)
                        printf("C ");
                    if (i == 3)
                        printf("B ");
                    if (i == 4)
                        printf("J ");
                    if (i == 5)
                        printf("K ");
                    if (i == 6)
                        printf("L ");
                }
        if (temp & 0x10)
            for (int i = 0; i < 7; i++)
                if ((keys[i] & 0x10) == 0) {
                    if (i == 0)
                        printf("Shift ");
                    if (i == 1)
                        printf("R ");
                    if (i == 2)
                        printf("F ");
                    if (i == 3)
                        printf("記号 ");
                    if (i == 4)
                        printf("N ");
                    if (i == 5)
                        printf("M ");
                    if (i == 6)
                        printf("- ");
                }
        if (temp & 0x20)
            for (int i = 0; i < 7; i++)
                if ((keys[i] & 0x20) == 0) {
                    if (i == 0)
                        printf("V ");
                    if (i == 1)
                        printf("PageUp ");
                    if (i == 2)
                        printf("Space ");
                    if (i == 3)
                        printf("Left ");
                    if (i == 4)
                        printf("Up ");
                    if (i == 5)
                        printf("Down ");
                    if (i == 6)
                        printf("Right ");
                }
        if (temp & 0x40)
            for (int i = 0; i < 7; i++)
                if ((keys[i] & 0x40) == 0) {
                    if (i == 0)
                        printf("文字切替 ");
                    if (i == 1)
                        printf("PageDown ");
                    if (i == 2)
                        printf("ツール ");
                    if (i == 3)
                        printf("Esc ");
                    if (i == 4)
                        printf("Enter ");
                    if (i == 5)
                        printf("? ");
                    if (i == 6)
                        printf("BackSpace ");
                }
        printf("\nkeys = 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x\n", keys[0], keys[1],
               keys[2], keys[3], keys[4], keys[5], keys[6]);
        memcpy(oldkeys, keys, sizeof(keys));
    }

    /* deinit */
    munmap(mapped_addr, 4096);
    close(fd);
    return 0;
}
