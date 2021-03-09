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

GPLv2
