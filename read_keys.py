#!/usr/bin/env python3

import smbus  # install python3-smbus with apt
import time


i2c = smbus.SMBus(1)
addr = 0x28


def read(reg):
    v = i2c.read_byte_data(addr, reg)
    print(f"Read b {reg:#04x} = {v:#04x}")
    return v


def readw(reg):
    v = i2c.read_word_data(addr, reg)
    #print(f"Read w {reg:#04x} = {v:#06x}")
    return v & 0x00ff, (v & 0xff00) >> 8


def write(reg, v):
    i2c.write_byte_data(addr, reg, v)
    print(f"Write b {reg:#04x} = {v:#06x}")


def setup():
    read(0x00)
    read(0x0a)
    read(0x01)

    read(0x40)
    read(0x41)
    read(0x48)

    read(0x00)
    readw(0x04)
    read(0x01)
    read(0x0a)
    read(0x00)
    read(0x0a)
    read(0x01)
    write(0x88, 0x08)
    write(0x82, 0x05)

    read(0x00)
    read(0x0a)

    write(0x82, 0x02)
    write(0x82, 0x03)


def read_keys():
    keys = []
    n, key = readw(0x04)
    if key == 0x00:
        return

    keys.append(key)

    additional = n // 2
    for i in range(additional):
        k1, k2 = readw(0x04)
        keys.append(k1)
        if i == additional-1 and n % 2 == 1:
            keys.append(k2)

    print(f"N = {n:#04x}")

    keys = [f"{key:#04x}" for key in keys]
    print(f"Keys = {', '.join(keys)}")


setup()
while True:
    time.sleep(0.1)
    read_keys()
