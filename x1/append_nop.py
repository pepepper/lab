#!/usr/bin/env python3

import sys


def main():
    if len(sys.argv) < 5:
        print(f'Usage: {sys.argv[0]} N top.bin bottom.bin out.bin', file=sys.stderr)
        sys.exit(1)

    with open(sys.argv[2], 'rb') as topf, open(sys.argv[3], 'rb') as bottomf, open(sys.argv[4], 'wb') as out:
        top, bottom = topf.read(), bottomf.read()
        out.write(top)
        for i in range((1024 * 64 - len(top) - len(bottom)) // 4):
            out.write(b'\x00\x00\xa0\xe1')
        out.write(bottom)

        for i in range(1024 * 1024 * 15 // (1024 * 64) - 1):
            for i in range((1024 * 64 - len(bottom)) // 4):
                out.write(b'\x00\x00\xa0\xe1')
            out.write(bottom)


main()
