#!/usr/bin/env python3

import sys


def main():
    if len(sys.argv) < 5:
        print(f'Usage: {sys.argv[0]} top.bin bottom_loop.bin bottom_reset.bin out.bin reset_fill_from reset_fill_to', file=sys.stderr)
        sys.exit(1)

    with open(sys.argv[1], 'rb') as tf, open(sys.argv[2], 'rb') as btmlf, open(sys.argv[3], 'rb') as btmrf, open(sys.argv[4], 'wb') as out:
        top, btml, btmr = tf.read(), btmlf.read(), btmrf.read()
        fill_from, fill_to = int(sys.argv[5]), int(sys.argv[6])

        out.write(top)
        if fill_from == 0:
            blen, b = len(btmr), btmr
        else:
            blen, b = len(btml), btml
        for i in range((1024 * 64 - len(top) - blen) // 4):
            out.write(b'\x00\x00\xa0\xe1')
        out.write(b)

        for i in range(1024 * 1024 * 15 // (1024 * 64) - 1):
            if fill_from <= i+1 < fill_to:
                blen, b = len(btmr), btmr
            else:
                blen, b = len(btml), btml

            for j in range((1024 * 64 - blen) // 4):
                out.write(b'\x00\x00\xa0\xe1')
            out.write(b)


main()
