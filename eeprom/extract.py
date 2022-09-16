#!/usr/bin/env python3

"""
EEPROM dump parser
Supports Saleae Logic 2 format that has following columns:
    - name
    - type
    - start_time
    - duration
    - ack
    - address
    - read
    - data
"""

import sys
from collections import namedtuple
from dataclasses import dataclass

from IPython import embed

struct = namedtuple(
    'struct',
    [
        'name',
        'typ',
        'start_time',
        'duration',
        'ack',
        'address',
        'read',
        'data',
    ],
)


def load(fn) -> list[struct]:
    l = []
    with open(fn, 'r') as f:
        f.readline()
        for line in f.readlines():
            values = line.replace('"', '').strip().split(',')
            s = struct(*values)
            l.append(s)

    return l


def main():
    @dataclass
    class Chunk:
        word_address: int
        length: int
        body: bytes

    data = load(sys.argv[1])
    chunks = []

    di = iter(data)

    while True:
        try:
            d = next(di)
            if d.typ != 'start':
                raise ValueError(f'unexpected type: {d.typ}, expected = start')
        except StopIteration:
            break

        d = next(di)
        if d.typ != 'address':
            raise ValueError(f'unexpected type: {d.typ}, expected = address')

        d1, d2 = next(di), next(di)
        if d1.typ != 'data' or d2.typ != 'data':
            raise ValueError(f'unexpected type: {d1.typ} and {d2.typ}, expected = data and data')

        hi, lo = eval(d1.data), eval(d2.data)

        d = next(di)
        if d.typ != 'start':
            raise ValueError(f'unexpected type: {d.typ}, expected = start')

        d = next(di)
        if d.typ != 'address':
            raise ValueError(f'unexpected type: {d.typ}, expected = address')

        body = []
        while True:
            d = next(di)
            if d.typ == 'stop':
                break
            body.append(eval(d.data))

        chunks.append(Chunk((hi << 8) + lo, len(body), bytes(body)))

    with open(sys.argv[1] + '.bin', 'wb') as f:
        for chunk in chunks:
            f.write(chunk.body)

    print('* Parse summary *')
    for i, chunk in enumerate(chunks):
        print(f'Chunk {i+1}: address={chunk.word_address}, length={chunk.length}')
    print('Total length = {}'.format(sum(c.length for c in chunks)))


main()
