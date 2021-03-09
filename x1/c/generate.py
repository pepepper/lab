#!/usr/bin/env python3
import sys
import textwrap


def main():
    if len(sys.argv) < 3:
        print(f'Usage: {sys.argv[0]} u-boot.bin uboot_bin.h.tpl uboot_bin.h')

    with open(sys.argv[2]) as f:
        tpl = f.read()

    with open(sys.argv[1], 'rb') as ui, open(sys.argv[3], 'w') as header:
        uboot = ui.read()

        out = ''
        for i, b in enumerate(uboot):
            out += f'{b:#04x}, '
            if (i+1) % 16 == 0:
                out = out.rstrip()
                out += '\n'

        formatted = tpl.format(uboot_size=len(uboot), uboot_bin=textwrap.indent(out, '    '))
        header.write(formatted)


main()
