## Overview

Nowadays the project is composed of a set of small utilities that can be used together or independently from the command line:

###radare2

The core of the hexadecimal editor and debugger. Allows to open any kind of file from different IO access like disk, network, kernel plugins, remote devices, debugged processes, ... and handle any of them as if they were a simple plain file.

Implements an advanced command line interface for moving around the file, analyzing data, disassembling, binary patching, data comparision, searching, replacing, scripting with ruby, python, lua and perl, ...

###rabin2

Extracts information from executable binaries like ELF, PE, Java CLASS, MACH-O. It's used from the core to get exported symbols, imports, file information, xrefs, library dependencies, sections, ...

###rasm2

Commandline assembler and disassembler for multiple architectures (intel[32,64], mips, arm, powerpc, java, msil, ...)

    $ rasm2 -a java 'nop'
    00
    
    $ rasm2 -a x86 -d '90'
    nop
    
    $ rasm2 -a x86 -b 32 'mov eax, 33'
    b821000000
    
    $ echo 'push eax;nop;nop' | rasm2 -f -
    5090


###rahash2

Implementation of a block-based rahash for small text strings or large disks, supporting multiple algorithms like md4, md5, crc16, crc32, sha1, sha256, sha384, sha512, par, xor, xorpair, mod255, hamdist or entropy.

It can be used to check the integrity or track changes between big files, memory dumps or disks.

###radiff2

Binary diffing utility with multiple algorithms implemented inside. Supports byte-level or delta diffing for binary files and code-analysis diffing to find changes in basic code blocks from radare code analysis or IDA ones using the idc2rdb rsc script.

###rafind2

rafind2 is a program to find byte patterns into files

###ragg2

Ragg2 is a frontend for r_egg, compile programs into tiny binaries for x86-32/64 and arm.

###rarun2

Rarun2 is used as a launcher for running programs with different environment, arguments, permissions, directories and overridden default file descriptors. It can be useful for :

* Crackme
* Fuzzing
* Test suite
