#!/usr/bin/env python3

import smbus  # install python3-smbus with apt
import time


i2c = smbus.SMBus(1)
addr = 0x28


def read(reg):
    v = i2c.read_byte_data(addr, reg)
    return v


def readw(reg):
    v = i2c.read_word_data(addr, reg)
    return v & 0x00ff, (v & 0xff00) >> 8


def write(reg, v):
    i2c.write_byte_data(addr, reg, v)


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


def setup_simple():
    """Simplified sequence without redundant calls"""

    print('Waiting the MCU to get ready')

    st = read(0x00)
    while st & 0x40 != 0:
        print('.', end='')
        st = read(0x00)
        time.sleep(0.1)

    working = read(0x0a) & 0x01 > 0
    if working:
        print('MCU is working')
    else:
        print('MCU is suspended')

    event = read(0x01) & 0x20 > 0
    if event:
        print('Event is enabled')
    else:
        print('Event is disabled')

    v = read(0x40)
    print(f'Reg (R) 0x40 -> {v:#04x}')

    read(0x41)
    print(f'Reg (R) 0x41 -> {v:#04x}')

    read(0x48)
    print(f'Reg (R) 0x48 -> {v:#04x}')

    write(0x88, 0x08)
    print('Reg (W) 0x88 = 0x08')

    write(0x82, 0x05)
    print('Reg (W) 0x82 = 0x05')

    write(0x82, 0x02)
    print('Reg (W) 0x82 = 0x02')

    write(0x82, 0x03)
    print('Reg (W) 0x82 = 0x03')


def read_keys():
    keys = []
    n, key = readw(0x04)
    if key == 0x00:
        return

    keys.append(key)

    if n >= 1:
        k1, k2 = readw(0x04)
        keys = keys + [k1, k2]
        keys = keys[:n]

    print(f"N = {n:#04x}")

    keys = [f"{key:#04x}" for key in keys]
    print(f"Keys = {', '.join(keys)}")


setup_simple()
while True:
    time.sleep(0.1)
    read_keys()
