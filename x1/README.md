#### Code
 - `mrc.S`  Read SCTLR (MMU etc.) and return
 - `return.S`  Return immediately
 - `extract.py`  Extract .text


#### Build
```sh
apt install gcc-arm-linux-gnueabihf
pip3 install pyelftools
make all
```

#### Run
 - Create a directory `/path/to/sd/APP/foo`
 - Create index.din `touch /path/to/sd/APP/foo/index.din`
 - Copy and rename the raw executable `cp foo.bin /path/to/sd/APP/foo/AppMain.bin`

#### License

See each directory and SPDX license identifier for further information.

// Portions from OP-TEE projects \
// https://github.com/OP-TEE/optee_os

Unless it has its own copyright/license embedded in its body, each file
is subject to the following license terms:

Copyright (c) 2015, Linaro Limited
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
