# Architectures

This chapter covers architecture specific topics.

Even though most examples in the radare2 book are showing Intel x86 code, radare2 supports an extensive list of computer architectures. The concepts of radare2 apply to all architectures, but there are a few differences in the configuration and usage.

Here's a list of the list of some of the currently supported architectures by radare2, you can get this list by running `rasm2 -L`. But from inside radare2 it's also possible to list the arch plugins using the `La` command, or the `-a?` one.

```
_de 8           6502        LGPL3   6502/NES analysis plugin
_de 8           6502.cs     LGPL3   Capstone mos65xx analysis plugin
ade 8 16        8051        LGPL3   8051 CPU code analysis plugin
_de 64          alpha       GPL     ALPHA architecture plugin
_de 32          amd29k      BSD     AMD 29k decoder
a__ 16 32 64    any.as      LGPL3   Uses system gnu/clang 'as' assembler (by pancake)
a__ 8 16 32 64  any.vasm    MIT     Use -a arm.vasm, 6502.vasm, 6809, c16x, jagrisc, m68k, pdp11, ppc, qnice, tr3200, vidcore, x86, z80 (by http://sun.hasenbraten.de/vasm/ (r2pm -ci vasm))
_de 48          arc         LGPL3   ARC code analysis plugin (by pancake)
a__ 16 32 64    arm.nz      LGPL3   custom thumb, arm32 and arm64 assembler (by pancake)
_de 16 32 64    arm         BSD     Capstone ARM analyzer
_de 16 32 64    arm.gnu     LGPL3   ARM code analysis plugin (asm.cpu=wd for winedbg disassembler)
_de 64          arm.v35     BSD     Vector35 ARM analyzer
ade 8 16        avr         LGPL3   AVR code analysis plugin
ade 32          bf          LGPL3   brainfuck code analysis plugin
ade 32          bpf.mr      LGPLv3  Classic BPF analysis plugin
_de 32 64       bpf         BSD     Capstone BPF plugin (by terorie, aemmitt)
_de 32          chip8       LGPL3   CHIP8 analysis plugin (by maijin)
_de 16          cr16        LGPL3   Compact RISC processor
_de 32          cris        LGPL3   Axis Communications 32-bit embedded processor
_de 32          dalvik      LGPL3   Dalvik (Android VM) bytecode analysis plugin
_de 32          dis         MIT     Inferno Dis VM disassembler
_de 32 64       ebc         LGPL3   EFI Bytecode architecture (by Fedor Sakharov)
ade 32          evm         BSD     EthereumVM plugin
ade 16          gb          LGPL3   Gameboy CPU code analysis plugin
_de 16          h8300       LGPL3   H8300 code analysis plugin
_de 16          hppa        GPL3    HP PA-RISC
ade 4           i4004       LGPL3   i4004 decoder plugin (by pancake, condret)
_de 16          i8080       LGPL3   I8080 CPU code arch plugin
ade 32          java        Apache  Java bytecode analysis plugin
_de 16          jdh8        LGPL3   jdh-8 toy architecture
_de 64          kvx         unknown Kalray VLIW core
_de 32          lanai       GPL3    Myricom's LANAI based on GNU binutils
_de 8           lh5801      LGPL3   SHARP LH5801 disassembler
_de 32          lm32        BSD     disassembly plugin for Lattice Micro 32 ISA (by Felix Held)
_de 64          loongarch   LGPL3   loongson loongarch code analysis plugin
ade 32          lua         MIT     LUA Bytecode arch plugin (by pancake)
_de 16 32       m680x       BSD     Capstone M680X analysis plugin
_de 32          m68k        BSD     Capstone M68K analyzer
_de 32          m68k.gnu    GPL3    Binutils 2.36 based m68k disassembler (by pancake)
_de 32          mcore       LGPL3   MCore analysis plugin
_de 16 32 64    mcs96       LGPL3   Intel MCS96 microcontroller, also known as 8xC196 or 80196 (by condret)
ade 16 32 64    mips        BSD     Capstone MIPS analyzer
_de 32          mips.gnu    LGPL3   MIPS code analysis plugin
_de 16          msp430      LGPL3   TI MSP430 code analysis plugin
_de 32          nds32       GPL3    Binutils based nds32 disassembler (by Edoardo Mantovani)
_de 32          nios2       LGPL3   NIOS II code analysis plugin
_de 8 16 32 64  null        LGPL3   Fallback/Null arch plugin
_de 32          or1k        LGPL3   OpenRISC 1000
_de 16          pdp11       GPL3    PDP-11 (by pancake)
_de 8           pic         LGPL3   PIC analysis plugin
ade 8           pickle      BSD     Python Pickle Machine Disassembler
_de 32 64       ppc         BSD     Capstone (+vle+ps) PowerPC disassembler
_de 32 64       ppc.gnu     LGPL3   PowerPC analysis plugin
_de 32          propeller   LGPL3   Parallax propeller code analysis plugin
_de 32          pyc         LGPL3   Python bytecode analysis plugin
ade 32 64       riscv       GPL     RISC-V analysis plugin (by pancake)
_de 32 64       riscv.cs    BSD     RISC-V analysis plugin
_de 32          rsp         LGPL3   RSP code analysis plugin
_de 32 64       s390        BSD     Capstone SystemZ microanalysis (by pancake)
_de 32 64       s390.gnu    BSD     SystemZ S390 from binutils
_de 32          sh          LGPL3   SH-4 code analysis plugin
_de 32          sh.cs       BSD     Capstone SH analysis
_de 4           sm5xx       BSD     Sharp SM 5XX family MCUs
_de 8 16        snes        LGPL3   SNES analysis plugin (by pancake)
_de 32 64       sparc       BSD     Capstone SPARC analysis
_de 32 64       sparc.gnu   GPL3    Scalable Processor Architecture
_de 32          tms320      LGPLv3  TMS320 DSP family (c54x,c55x,c55x+,c64x)
_de 32          tricore     LGPL3   TRICORE analysis plugin
_de 32          tricore.cs  BSD     Capstone TriCore analysis
_de 32          v810        LGPL3   V810 code analysis plugin
ade 32          v850        MIT     V850 code analysis plugin
_de 32          vax         GPL     VAX code analysis plugin
ade 64          wasm        LGPL3   WebAssembly analysis plugin
_de 32          ws          LGPL3   Space, tab and linefeed analysis plugin
_de 16 32 64    x86         BSD     Capstone X86 analysis
a__ 16 32 64    x86.nasm    LGPL3   X86 nasm assembler
a__ 16 32 64    x86.nz      LGPL3   x86 handmade assembler (by pancake)
_de 16          xap         LGPL3   XAP code analysis plugin (by pancake)
_de 32          xcore       BSD     Capstone XCORE analysis
_de 32          xtensa      LGPL3   Xtensa disassembler
ade 16          z80         GPL     Z80 CPU code analysis plugin
```
