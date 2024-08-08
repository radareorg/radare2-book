## Rasm2

The command-line assembler and disassembler that is part of the radare2 framework. It supports a wide range of architectures and can be used independently of the main radare2 tool. Key features include:

* Multi-architecture support: Can handle numerous architectures including x86, x86-64, ARM, MIPS, PowerPC, SPARC, and many others.
* Bi-directional operation: Functions as both an assembler (converting human-readable assembly code to machine code) and a disassembler (converting machine code back to assembly).
* Flexible input/output: Accepts input as hexadecimal strings, raw binary files, or text files containing assembly code.
* Shellcode generation: Useful for security research and exploit development.
* Inline assembly: Allows for quick assembly of individual instructions or small code snippets.
* Syntax highlighting: Provides colored output for better readability when disassembling.
* Plugins: Supports architecture-specific plugins for extended functionality.

### Help

```
$ rasm2 -h
Usage: rasm2 [-ACdDehLBvw] [-a arch] [-b bits] [-o addr] [-s syntax]
             [-f file] [-F fil:ter] [-i skip] [-l len] 'code'|hex|0101b|-
 -a [arch]    set architecture to assemble/disassemble (see -L)
 -A           show Analysis information from given hexpairs
 -b [bits]    set cpu register size (8, 16, 32, 64) (RASM2_BITS)
 -B           binary input/output (-l is mandatory for binary input)
 -c [cpu]     select specific CPU (depends on arch)
 -C           output in C format
 -d, -D       disassemble from hexpair bytes (-D show hexpairs)
 -e           use big endian instead of little endian
 -E           display ESIL expression (same input as in -d)
 -f [file]    read data from file
 -F [in:out]  specify input and/or output filters (att2intel, x86.pseudo, ...)
 -h, -hh      show this help, -hh for long
 -i [len]     ignore/skip N bytes of the input buffer
 -j           output in json format
 -k [kernel]  select operating system (linux, windows, darwin, ..)
 -l [len]     input/Output length
 -L           list RAsm plugins: (a=asm, d=disasm, A=analyze, e=ESIL)
 -LL          list RAnal plugins (see anal.arch=?) combines with -j
 -o,-@ [addr] set start address for code (default 0)
 -O [file]    output file name (rasm2 -Bf a.asm -O a)
 -N           same as r2 -N (or R2_NOPLUGINS) (not load any plugin)
 -p           run SPP over input for assembly
 -q           quiet mode
 -r           output in radare commands
 -s [syntax]  select syntax (intel, att)
 -v           show version information
 -x           use hex dwords instead of hex pairs when assembling.
 -w           what's this instruction for? describe opcode
 If '-l' value is greater than output length, output is padded with nops
 If the last argument is '-' reads from stdin
Environment:
 R2_NOPLUGINS   do not load shared plugins (speedup loading)
 R2_LOG_LEVEL=X    change the log level
 R2_DEBUG          if defined, show error messages and crash signal
 R2_DEBUG_ASSERT=1 lldb -- r2 to get proper backtrace of the runtime assert
 RASM2_ARCH        same as rasm2 -a
 RASM2_BITS        same as rasm2 -b
```

### Plugins

Plugins for supported target architectures can be listed with the `-L` option. Knowing a plugin name, you can use it by specifying its name to the `-a` option

```
$ rasm2 -L
_dAe  8 16       6502        LGPL3   6502/NES/C64/Tamagotchi/T-1000 CPU
_dAe  8          8051        PD      8051 Intel CPU
_dA_  16 32      arc         GPL3    Argonaut RISC Core
a___  16 32 64   arm.as      LGPL3   as ARM Assembler (use ARM_AS environment)
adAe  16 32 64   arm         BSD     Capstone ARM disassembler
_dA_  16 32 64   arm.gnu     GPL3    Acorn RISC Machine CPU
_d__  16 32      arm.winedbg LGPL2   WineDBG's ARM disassembler
adAe  8 16       avr         GPL     AVR Atmel
adAe  16 32 64   bf          LGPL3   Brainfuck (by pancake, nibble) v4.0.0
_dA_  32         chip8       LGPL3   Chip8 disassembler
_dA_  16         cr16        LGPL3   cr16 disassembly plugin
_dA_  32         cris        GPL3    Axis Communications 32-bit embedded processor
adA_  32 64      dalvik      LGPL3   AndroidVM Dalvik
ad__  16         dcpu16      PD      Mojang's DCPU-16
_dA_  32 64      ebc         LGPL3   EFI Bytecode
adAe  16         gb          LGPL3   GameBoy(TM) (z80-like)
_dAe  16         h8300       LGPL3   H8/300 disassembly plugin
_dAe  32         hexagon     LGPL3   Qualcomm Hexagon (QDSP6) V6
_d__  32         hppa        GPL3    HP PA-RISC
_dAe             i4004       LGPL3   Intel 4004 microprocessor
_dA_  8          i8080       BSD     Intel 8080 CPU
adA_  32         java        Apache  Java bytecode
_d__  32         lanai       GPL3    LANAI
_d__  8          lh5801      LGPL3   SHARP LH5801 disassembler
_d__  32         lm32        BSD     disassembly plugin for Lattice Micro 32 ISA
_dA_  16 32      m68k        BSD     Capstone M68K disassembler
_dA_  32         malbolge    LGPL3   Malbolge Ternary VM
_d__  16         mcs96       LGPL3   condrets car
adAe  16 32 64   mips        BSD     Capstone MIPS disassembler
adAe  32 64      mips.gnu    GPL3    MIPS CPU
_dA_  16         msp430      LGPL3   msp430 disassembly plugin
_dA_  32         nios2       GPL3    NIOS II Embedded Processor
_dAe  8          pic         LGPL3   PIC disassembler
_dAe  32 64      ppc         BSD     Capstone PowerPC disassembler
_dA_  32 64      ppc.gnu     GPL3    PowerPC
_d__  32         propeller   LGPL3   propeller disassembly plugin
_dA_  32 64      riscv       GPL     RISC-V
_dAe  32         rsp         LGPL3   Reality Signal Processor
_dAe  32         sh          GPL3    SuperH-4 CPU
_dA_  8 16       snes        LGPL3   SuperNES CPU
_dAe  32 64      sparc       BSD     Capstone SPARC disassembler
_dA_  32 64      sparc.gnu   GPL3    Scalable Processor Architecture
_d__  16         spc700      LGPL3   spc700, snes' sound-chip
_d__  32         sysz        BSD     SystemZ CPU disassembler
_dA_  32         tms320      LGPLv3  TMS320 DSP family (c54x,c55x,c55x+,c64x)
_d__  32         tricore     GPL3    Siemens TriCore CPU
_dAe  32         v810        LGPL3   v810 disassembly plugin
_dAe  32         v850        LGPL3   v850 disassembly plugin
_dAe  8 32       vax         GPL     VAX
adA_  32         wasm        MIT     WebAssembly (by cgvwzq) v0.1.0
_dA_  32         ws          LGPL3   Whitespace esotheric VM
a___  16 32 64   x86.as      LGPL3   Intel X86 GNU Assembler
_dAe  16 32 64   x86         BSD     Capstone X86 disassembler
a___  16 32 64   x86.nasm    LGPL3   X86 nasm assembler
a___  16 32 64   x86.nz      LGPL3   x86 handmade assembler
_dA_  16         xap         PD      XAP4 RISC (CSR)
_dA_  32         xcore       BSD     Capstone XCore disassembler
_dAe  32         xtensa      GPL3    XTensa CPU
adA_  8          z80         GPL     Zilog Z80
```

**NOTE** the "ad" in the first column means both assembler and disassembler are offered by a corresponding  plugin. "_d" indicates disassembler, "a_" means only assembler is available.
