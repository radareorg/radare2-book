#Chapter 14: Rasm2

The inline assembler/disassembler. Initially 'rasm' was designed to be used for binary patching, just to get the bytes of a certain opcode. Here's the help

    $ rasm2 -h
    Usage: rasm2 [-CdDehLBvw] [-a arch] [-b bits] [-o addr] [-s syntax]
                 [-f file] [-F fil:ter] [-i skip] [-l len] 'code'|hex|-
     -a [arch]    Set architecture to assemble/disassemble (see -L)
     -b [bits]    Set cpu register size (8, 16, 32, 64) (RASM2_BITS)
     -c [cpu]     Select specific CPU (depends on arch)
     -C           Output in C format
     -d, -D       Disassemble from hexpair bytes (-D show hexpairs)
     -e           Use big endian instead of little endian
     -f [file]    Read data from file
     -F [in:out]  Specify input and/or output filters (att2intel, x86.pseudo, ...)
     -h           Show this help
     -i [len]     ignore/skip N bytes of the input buffer
     -k [kernel]  Select operating system (linux, windows, darwin, ..)
     -l [len]     Input/Output length
     -L           List supported asm plugins
     -o [offset]  Set start address for code (default 0)
     -O [file]    Output file name (rasm2 -Bf a.asm -O a)
     -s [syntax]  Select syntax (intel, att)
     -B           Binary input/output (-l is mandatory for binary input)
     -v           Show version information
     -w           What's this instruction for? describe opcode
     If '-l' value is greater than output length, output is padded with nops
     If the last argument is '-' reads from stdin


Asm supported plugins can be listed with "-L"

    $ rasm2 -L
    _d  16         8051        PD      8051 Intel CPU
    _d  16 32      arc         GPL3    Argonaut RISC Core
    ad  16 32 64   arm         GPL3    Acorn RISC Machine CPU
    _d  16 32 64   arm.cs      BSD     Capstone ARM disassembler
    _d  16 32      arm.winedbg LGPL2   WineDBG's ARM disassembler
    _d  16 32      avr         GPL     AVR Atmel
    ad  32         bf          LGPL3   Brainfuck
    _d  16         cr16        LGPL3   cr16 disassembly plugin
    _d  16         csr         PD      Cambridge Silicon Radio (CSR)
    ad  32 64      dalvik      LGPL3   AndroidVM Dalvik
    ad  16         dcpu16      PD      Mojang's DCPU-16
    _d  32 64      ebc         LGPL3   EFI Bytecode
    _d  8          gb          LGPL3   GameBoy(TM) (z80-like)
    _d  16         h8300       LGPL3   H8/300 disassembly plugin
    _d  8          i8080       BSD     Intel 8080 CPU
    ad  32         java        Apache  Java bytecode
    _d  32         m68k        BSD     Motorola 68000
    _d  32         malbolge    LGPL3   Malbolge Ternary VM
    ad  32 64      mips        GPL3    MIPS CPU
    _d  16 32 64   mips.cs     BSD     Capstone MIPS disassembler
    _d  16 32 64   msil        PD      .NET Microsoft Intermediate Language
    _d  32         nios2       GPL3    NIOS II Embedded Processor
    _d  32 64      ppc         GPL3    PowerPC
    _d  32 64      ppc.cs      BSD     Capstone PowerPC disassembler
    ad             rar         LGPL3   RAR VM
    _d  32         sh          GPL3    SuperH-4 CPU
    _d  32 64      sparc       GPL3    Scalable Processor Architecture
    _d  32         tms320      LGPLv3  TMS320 DSP family
    _d  32         ws          LGPL3   Whitespace esotheric VM
    _d  16 32 64   x86         BSD     udis86 x86-16,32,64
    _d  16 32 64   x86.cs      BSD     Capstone X86 disassembler
    a_  32 64      x86.nz      LGPL3   x86 handmade assembler
    ad  32         x86.olly    GPL2    OllyDBG X86 disassembler
    ad  8          z80         NC-GPL2 Zilog Z80

 
##14.1 Assemble

It is quite common to use 'rasm2' from the shell. It is a nice utility for copypasting the hexpairs that represent the opcode.

    $ rasm2 -a x86 -b 32 'mov eax, 33'
    b821000000

    $ echo 'push eax;nop;nop' | rasm2 -f -
    5090

Rasm2 is used from radare core to write bytes using 'wa' command. 

It is possible to assemble for x86 (intel syntax), olly (olly syntax), powerpc, arm and java. For the intel syntax, rasm tries to use NASM or GAS. You can use the SYNTAX environment variable to choose your favorite syntax: intel or att.

There are some examples in rasm's source directory to assemble a raw file using rasm from a file describing these opcodes.

    $ cat selfstop.rasm
    ;
    ; Self-Stop shellcode written in rasm for x86
    ;
    ; --pancake
    ;
    
    .arch x86
    .equ base 0x8048000
    .org 0x8048000  ; the offset where we inject the 5 byte jmp
    
    selfstop:
      push 0x8048000
      pusha
      mov eax, 20
      int 0x80
    
      mov ebx, eax 
      mov ecx, 19
      mov eax, 37
      int 0x80
      popa
      ret
    ;
    ; The call injection
    ;
    
      ret
      
      
     
    [0x00000000]> e asm.bits = 32
    [0x00000000]> wx `!rasm2 -f a.rasm`
    [0x00000000]> pd 20
           0x00000000    6800800408   push 0x8048000 ;  0x08048000 
           0x00000005    60           pushad
           0x00000006    b814000000   mov eax, 0x14 ;  0x00000014 
           0x0000000b    cd80         int 0x80
              syscall[0x80][0]=?
           0x0000000d    89c3         mov ebx, eax
           0x0000000f    b913000000   mov ecx, 0x13 ;  0x00000013 
           0x00000014    b825000000   mov eax, 0x25 ;  0x00000025 
           0x00000019    cd80         int 0x80
              syscall[0x80][0]=?
           0x0000001b    61           popad
           0x0000001c    c3           ret
           0x0000001d    c3           ret

  
##14.2 Disassemble

In the same way as rasm assembler works, giving the '-d' flag you can disassemble an hexpair string:

    $ rasm2 -a x86 -b 32 -d '90'
    nop