#!/usr/bin/env python3

import sys
from elftools.elf.elffile import ELFFile  # pip install pyelftools


def main():
    if len(sys.argv) < 3:
        print(f'Usage: {sys.argv[0]} in.elf out.bin')
        sys.exit(1)

    with open(sys.argv[1], 'rb') as f:
        extract(ELFFile(f))


def extract(elf):
    text = elf.get_section_by_name('.text')
    if text is None:
        print('Input ELF has no .text section', file=sys.stderr)

    with open(sys.argv[2], 'wb') as f:
        f.write(text.data())

    print(f'Successfully extracted the .text section to "{sys.argv[2]}"')


main()
