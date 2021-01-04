import gpio  # https://github.com/vitiral/gpio
import time


def setup():
    # Bank2 Pin16~PIN21
    gpio.setup(80, gpio.OUT)
    gpio.setup(81, gpio.OUT)
    gpio.setup(82, gpio.OUT)
    gpio.setup(83, gpio.OUT)
    gpio.setup(84, gpio.OUT)
    gpio.setup(85, gpio.OUT)
    # Bank4 Pin0~7
    gpio.setup(128, gpio.IN)
    gpio.setup(129, gpio.IN)
    gpio.setup(130, gpio.IN)
    gpio.setup(131, gpio.IN)
    gpio.setup(132, gpio.IN)
    gpio.setup(133, gpio.IN)
    gpio.setup(134, gpio.IN)
    gpio.setup(135, gpio.IN)
    # Bank4 Pin8
    gpio.setup(136, gpio.OUT)


def gpioread():
    keys = []
    for writeport in range(80, 87):
        if writeport == 86:
            writeport = 136
        gpio.set(writeport, 1)
        time.sleep(0.002)
        key = 0
        for readport in range(135, 127, -1):
            key = (key << 1) + gpio.read(readport)
        key = ~((key ^ (key >> 1)) & 0x1f ^ (key >> 1)) & 0x7f
        keys.append(key)
        gpio.set(writeport, 0)
    return keys


def keyread():
    keys = readgpio()
    verify_keys = readgpio()
    if not keys == verify_keys:
        return
    temp = 0
    for key in keys:
        temp = temp | key
    if temp & 0x1:
        for i, key in enumerate(keys):
            if key & 0x1 == 0:
                if i == 0:
                    print("英和和英", end=" ")
                if i == 1:
                    print("調べる", end=" ")
                if i == 2:
                    print("国語漢字", end=" ")
                if i == 3:
                    print("My辞書", end=" ")
                if i == 4:
                    print("履歴/しおり", end=" ")
                if i == 5:
                    print("マーカーテスト", end=" ")
                if i == 6:
                    print("HOME", end=" ")
    if temp & 0x2:
        for i, key in enumerate(keys):
            if key & 0x2 == 0:
                if i == 0:
                    print("Q", end=" ")
                if i == 1:
                    print("W", end=" ")
                if i == 2:
                    print("E", end=" ")
                if i == 3:
                    print("T", end=" ")
                if i == 4:
                    print("Y", end=" ")
                if i == 5:
                    print("U", end=" ")
                if i == 6:
                    print("I", end=" ")
    if temp & 0x4:
        for i, key in enumerate(keys):
            if key & 0x4 == 0:
                if i == 0:
                    print("A", end=" ")
                if i == 1:
                    print("S", end=" ")
                if i == 2:
                    print("D", end=" ")
                if i == 3:
                    print("G", end=" ")
                if i == 4:
                    print("H", end=" ")
                if i == 5:
                    print("O", end=" ")
                if i == 6:
                    print("P", end=" ")
    if temp & 0x8:
        for i, key in enumerate(keys):
            if key & 0x8 == 0:
                if i == 0:
                    print("Z", end=" ")
                if i == 1:
                    print("X", end=" ")
                if i == 2:
                    print("C", end=" ")
                if i == 3:
                    print("B", end=" ")
                if i == 4:
                    print("J", end=" ")
                if i == 5:
                    print("K", end=" ")
                if i == 6:
                    print("L", end=" ")
    if temp & 0x10:
        for i, key in enumerate(keys):
            if key & 0x10 == 0:
                if i == 0:
                    print("Shift", end=" ")
                if i == 1:
                    print("R", end=" ")
                if i == 2:
                    print("F", end=" ")
                if i == 3:
                    print("記号", end=" ")
                if i == 4:
                    print("N", end=" ")
                if i == 5:
                    print("M", end=" ")
                if i == 6:
                    print("-", end=" ")
    if temp & 0x20:
        for i, key in enumerate(keys):
            if key & 0x20 == 0:
                if i == 0:
                    print("V", end=" ")
                if i == 1:
                    print("PageUp", end=" ")
                if i == 2:
                    print("Space", end=" ")
                if i == 3:
                    print("Left", end=" ")
                if i == 4:
                    print("Up", end=" ")
                if i == 5:
                    print("Down", end=" ")
                if i == 6:
                    print("Right", end=" ")
    if temp & 0x40:
        for i, key in enumerate(keys):
            if key & 0x40 == 0:
                if i == 0:
                    print("文字切替", end=" ")
                if i == 1:
                    print("PageDown", end=" ")
                if i == 2:
                    print("ツール", end=" ")
                if i == 3:
                    print("Esc", end=" ")
                if i == 4:
                    print("Enter", end=" ")
                if i == 5:
                    print("?", end=" ")
                if i == 6:
                    print("BackSpace", end=" ")
    keys = [f"{key:#04x}" for key in keys]

    if not temp == 0:
        print(f"\nKeys = {', '.join(keys)}")


setup()
while True:
    time.sleep(0.001)
    keyread()
