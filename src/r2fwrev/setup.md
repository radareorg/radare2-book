## Project Organization

The most common way to get ready with radare2 to analyze a firmware is usually to create a directory containing the firmware file, a makefile or shellscript and a directory to hold few companion files that will help us get all the stuff in place everytime we start the shell.

The reason for not using projects is because usually these targets require some special setups, custom scripts, manual tries and errors and obviously not using the default autoanalysis.

This way we have full control on how r2 will behave and we can always change any line of the scripts to get the right behaviour.

```console
$ cat Makefile
BADDR=0x8000000
all:
    r2 -i script.r2 -m $BADDR dump.bin
$ cat script.r2
f entry0=0x8005320
s entry0
CC evertyhing starts here
afr
```

## Cpu model

First of all we need to know or guess which is the target architecture the blob is suposed to be executed.

As long as this file is just a raw dump there is no metadata that can be retrieved from RBin to auto configure the asm.arch and asm.cpu variables.

In some cases the chip is covered with black plastic, hiding the actual model or architecture details. If looking for documentation online doesn't rings any bell we can always try for the most common options until we get a meaningful disassembly listing.

The most common architectures we will find when analysing firmwares or flash dumps are: arm, thumb, tricore, v850, mips, riscv, stm8, tms320 or mcs64.

You can change any of the following options inside the r2 shell after loading the file to see what's in the output of `pd` for us.

```console
e asm.arch=?
e asm.bits=?
e asm.cpu=?
e cfg.bigendian=true|false
```

For example a big endian ARM-Thumb environment can be configured like this:

`$ r2 -a arm -b 16 -e cfg.bigendian=true`
