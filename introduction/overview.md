## Overview

Today, the radare project is a set of small utilities that can be used together or independently from the command line.

###radare2

The core of the hexadecimal editor and debugger. radare2 allows you to open a number of input/output sources as if they were simple, plain files, including disks, networks, kernel plug-ins, processes being debugged, and so on.

It implements an advanced command line interface for moving around the file, analyzing data, disassembling, binary patching, data comparison, searching, replacing, and scripting with a variety of languages, including Ruby, Python, Lua, and Perl.

###rabin2

Extracts information from executable binaries, such as ELF, PE, Java CLASS, and Mach-O. rabin2 is used from the core to get exported symbols, imports, file information, cross references (xrefs), library dependencies, sections, etc.

###rasm2

A command line assembler and disassembler for multiple architectures (including Intel x86 and x86-64, MIPS, ARM, PowerPC, Java, and MSIL):

    $ rasm2 -a java 'nop'
    00
    
    $ rasm2 -a x86 -d '90'
    nop
    
    $ rasm2 -a x86 -b 32 'mov eax, 33'
    b821000000
    
    $ echo 'push eax;nop;nop' | rasm2 -f -
    5090


###rahash2

An implementation of a block-based hash. From small text strings to large disks, rahash2 supports multiple algorithms, including MD4, MD5, CRC16, CRC32, SHA1, SHA256, SHA384, SHA512, par, xor, xorpair, mod255, hamdist, or entropy.

rahash2 can be used to check the integrity of, or track changes to, big files, memory dumps, and disks.

###radiff2

A binary diffing utility that implements multiple algorithms. It supports byte-level or delta diffing for binary files, and code-analysis diffing to find changes in basic code blocks from the radare code analysis, or from the IDA analysis using the rsc idc2rdb  script.

###rafind2

A program to find byte patterns in a file.

###ragg2

A frontend for r_egg. ragg2 compiles programs into tiny binaries for x86, x86-64, and ARM.

###rarun2

A launcher for running programs with a different environment, arguments, permissions, directories, and overridden, default file descriptors. rarun2 is useful for:

* Crackmes
* Fuzzing
* Test suites

#### Sample rarun2 script

       $ cat foo.rr2
       #!/usr/bin/rarun2
       program=./pp400
       arg0=10
       stdin=foo.txt
       chdir=/tmp
       #chroot=.
       ./foo.rr2


#### Connecting a program to a socket

       $ nc -l 9999
       $ rarun2 program=/bin/ls connect=localhost:9999


#### Debugging a program redirecting io to another terminal

1. open a new terminal and type 'tty' to get
       $ tty ; clear ; sleep 999999
       /dev/ttyS010
2. In another terminal run r2
        $ r2 -d rarun2 program=/bin/ls stdio=/dev/ttys010

###rax2

A utility that aims to be a minimalistic expression evaluator for the shell. It is useful for making base conversions between floating point values, hexadecimal representations, hexpair strings to ascii, octal to integer. It supports endianness and can be used as a shell if no arguments are given.


    $ rax2 1337
    0x539
    
    $ rax2 0x400000
    4194304
    
    $ rax2 -b 01111001
    y
    
    $ rax2 -S radare2
    72616461726532
    
    $ rax2 -s 617765736f6d65
    awesome
    
    