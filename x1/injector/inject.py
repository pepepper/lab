#!/usr/bin/env python3

import sys


def main():
    if len(sys.argv) < 6:
        print(f"Usage: {sys.argv[0]} total page_offset disable_mmu.bin injected.bin out.bin")
        sys.exit(1)

    total, offset, dismmu, injected, out = sys.argv[1:6]
    total = int(total, base=16 if total.startswith('0x') else 10)
    offset = int(offset, base=16 if offset.startswith('0x') else 10)

    if total % 4 != 0:
        print(f'Total is not aligned', file=sys.stderr)
        sys.exit(1)
    elif offset % (1024 * 64) != 0:
        print(f'Page offset is not aligned', file=sys.stderr)
        sys.exit(1)

    with open(dismmu, 'rb') as dmf, open(injected, 'rb') as injf, open(out, 'wb') as out:
        nop = b'\x00\x00\xa0\xe1'
        dm = dmf.read()
        inj = injf.read()

        out.write(dm)
        out.write(nop * ((offset - len(dm)) // 4))
        out.write(nop * ((1024 * 64 - len(inj)) // 4))
        out.write(inj)
        out.write(nop * ((total - offset - 1024 * 64) // 4))


main()
