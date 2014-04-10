#Chapter 3: Basic commands

The basic set of commands in radare can be mostly grouped by action, and they should be easy to remember and short. This is why they are grouped with a single character, subcommands or related commands are described with a second character. For example '/ foo' for searching plain strings or '/x 90 90' to look for hexpair strings.

The format of the commands (as explained in 'Command format' chapter) looks something like that:

    [[.][times][cmd][~grep][@[@iter]addr!size][|>pipe] ; ...

    > 3s +1024    ; seeks three times 1024 from the current seek
    
If the command starts with '!' the string is passed to the plugin hadling the current IO (the debugger for example), if no one handles it calls to posix_system() which is a shell escape, you can prefix the command with two '!!'.

    > !help       ; handled by the debugger or shell
    > !!ls        ; runs ls in the shell
The [arg] argument depends on the command, but most of them take a number as argument to specify the number of bytes to work on instead of block size. Other commands accept math expressions, or strings.

    > px 0x17     ; show 0x17 bytes in hexa at current seek
    > s base+0x33 ; seeks to flag 'base' plus 0x33
    > / lib       ; search for 'lib' string.
The '@' is used to specify a temporal seek where the command is executed. This is quite useful to not seeking all the time.

    > p8 10 @ 0x4010  ; show 10 bytes at offset 0x4010
    > f patata @ 0x10 ; set 'patata' flag at offset 0x10
Using '@@' you can execute a single command on a list of flags matching the glob:

    > s 0
    > / lib             ; search 'lib' string
    > p8 20 @@ hit0_*   ; show 20 hexpairs at each search hit
    
The '>' is used to pipe the output of the command to a file (truncating to 0 if exist)

    > pr > dump.bin   ; dump 'raw' bytes of current block to 'dump.bin' file
    > f  > flags.txt  ; dump flag list to 'flags.txt'
    
The '|' pipe is used to dump the output of the command to another program.

    [0x4A13B8C0]> f | grep section | grep text
    0x0805f3b0 512 section._text
    0x080d24b0 512 section._text_end
    
Using the ';' chars you can concatenate multiple commands in a single line:

    > px ; dr


##3.1 Seeking

The command 's' is used to seek. It accepts a math expression as argument that can be composed by shift operations, basic math ones and memory access.


    [0x00000000]> s?
    Usage: s[+-] [addr]
    s                 print current address
    s 0x320           seek to this address
    s-                undo seek
    s+                redo seek
    s*                list undo seek history
    s++               seek blocksize bytes forward
    s--               seek blocksize bytes backward
    s+ 512            seek 512 bytes forward
    s- 512            seek 512 bytes backward
    sg/sG             seek begin (sg) or end (sG) of section or file
    s.hexoff          Seek honoring a base from core->offset
    sa [[+-]a] [asz]  seek asz (or bsize) aligned to addr
    sn/sp             seek next/prev scr.nkey
    s/ DATA           search for next occurrence of 'DATA'
    s/x 9091          search for next occurrence of \x90\x91
    sb                seek aligned to bb start
    so [num]          seek to N next opcode(s)
    sf                seek to next function (f->addr+f->size)
    sC str            seek to comment matching given string
    sr pc             seek to register

    > 3s++        ; 3 times block-seeking
    > s 10+0x80   ; seek at 0x80+10


To 'query' the math expression you can evaluate them using the '?' command and giving the math operation as argument. And getting the result in hexa, decimal, octal and binary.

    > ? 0x100+200
    0x1C8 ; 456d ; 710o ; 1100 1000  


In visual mode just press 'u' or 'U' to undo or redo inside the seek history.


##3.2 Block size

The block size is the default view size for radare. All commands will work with this constraint, but you can always temporally change the block size just giving a numeric argument to the print commands for example (px 20)

    [0xB7F9D810]> b?
    Usage: b[f] [arg]
    b         display current block size
    b+3       increase blocksize by 3
    b-16      decrement blocksize by 3
    b 33      set block size to 33
    b eip+4   numeric argument can be an expression
    bf foo    set block size to flag size
    bm 1M     set max block size

The 'b' command is used to change the block size:

    [0x00000000]> b 0x100   ; block size = 0x100
    [0x00000000]> b +16     ;  ... = 0x110
    [0x00000000]> b -32     ;  ... = 0xf0
    
The 'bf' command is used to change the block size to the one specified by a flag. For example in symbols, the block size of the flag represents the size of the function.

    [0x00000000]> bf sym.main    ; block size = sizeof(sym.main)
    [0x00000000]> pd @ sym.main  ; disassemble sym.main
    ...

You can perform these two operations in a single one (pdf):

     [0x00000000]> pdf @ sym.main


##3.4 Mapping files

Radare IO allows to virtually map contents of files in the same IO space at random offsets. This is useful to open multiple files in a single view or just to 'emulate' an static environment like if it was in the debugger with the program and all its libraries mapped there.

Using the 'S'ections command you'll be able to define different base address for each library loaded at different offsets.

Mapping files is done with the 'o' (open) command. Let's read the help:

    [0x00000000]> o?
    Usage: o[com- ] [file] ([offset])
    o                  list opened files
    oc [file]          open core file, like relaunching r2
    oo                 reopen current file (kill+fork in debugger)
    oo+                reopen current file in read-write
    o 4                priorize io on fd 4 (bring to front)
    o-1                close file index 1
    o /bin/ls          open /bin/ls file in read-only
    o+/bin/ls          open /bin/ls file in read-write mode
    o /bin/ls 0x4000   map file at 0x4000
    on /bin/ls 0x4000  map raw file at 0x4000 (no r_bin involved)
    om[?]              create, list, remove IO maps

Let's prepare a simple layout:

    $ rabin2 -l /bin/ls
        [Linked libraries]
        libselinux.so.1
        librt.so.1
        libacl.so.1
        libc.so.6
        
        4 libraries

Map a file:

    [0x00001190]> o /bin/zsh 0x499999

Listing mapped files:

    [0x00000000]> o
    - 6 /bin/ls @ 0x0 ; r
    - 10 /lib/ld-linux.so.2 @ 0x100000000 ; r
    - 14 /bin/zsh @ 0x499999 ; r


Print some hexadecimal from /bin/zsh

    [0x00000000]> px @ 0x499999


To unmap these files just use the 'o-' command giving the file descriptor as argument:

    [0x00000000]> o-14


##3.5 Print modes

One of the efforts in radare is the way to show the information to the user. This is interpreting the bytes and giving an almost readable output format.

The bytes can be represented as integers, shorts, longs, floats, timestamps, hexpair strings, or things more complex like C structures, disassembly, decompilations, external processors, ..

This is a list of the available print modes listable with 'p?':

    [0x08049AD0]> p?
    Usage: p[=68abcdDfiImrstuxz] [arg|len]
    p=[bep?] [blks]  show entropy/printable chars/chars bars
    p2 [len]         8x8 2bpp-tiles
    p6[de] [len]     base64 decode/encode
    p8 [len]         8bit hexpair list of bytes
    pa[ed] [hex asm] assemble (pa) or disasm (pad) or esil (pae) from hexpairs
    p[bB] [len]      bitstream of N bytes
    pc[p] [len]      output C (or python) format
    p[dD][lf] [l]    disassemble N opcodes/bytes (see pd?)
    pf[?|.nam] [fmt] print formatted data (pf.name, pf.name $<expr>) 
    p[iI][df] [len]  print N instructions/bytes (f=func) (see pi? and pdi)
    pm [magic]       print libmagic data (pm? for more information)
    pr [len]         print N raw bytes
    p[kK] [len]      print key in randomart (K is for mosaic)
    ps[pwz] [len]    print pascal/wide/zero-terminated strings
    pt[dn?] [len]    print different timestamps
    pu[w] [len]      print N url encoded bytes (w=wide)
    pv[jh] [mode]      bar|json|histogram blocks (mode: e?search.in)
    p[xX][owq] [len] hexdump of N bytes (o=octal, w=32bit, q=64bit)
    pz [len]         print zoom view (see pz? for help)
    pwd              display current working directory
    
    
###3.5.1 Hexadecimal

User-friendly way:

    [0x00404888]> px
    - offset -   0 1  2 3  4 5  6 7  8 9  A B  C D  E F  0123456789ABCDEF
    0x00404888  31ed 4989 d15e 4889 e248 83e4 f050 5449  1.I..^H..H...PTI
    0x00404898  c7c0 4024 4100 48c7 c1b0 2341 0048 c7c7  ..@$A.H...#A.H..
    0x004048a8  d028 4000 e83f dcff fff4 6690 662e 0f1f  .(@..?....f.f...

####Show hexadecimal words dump (32bit)

    [0x00404888]> pxw
    0x00404888  0x8949ed31 0x89485ed1 0xe48348e2 0x495450f0  1.I..^H..H...PTI
    0x00404898  0x2440c0c7 0xc7480041 0x4123b0c1 0xc7c74800  ..@$A.H...#A.H..
    0x004048a8  0x004028d0 0xffdc3fe8 0x9066f4ff 0x1f0f2e66  .(@..?....f.f...
    
    [0x00404888]> e cfg.bigendian 
    false
    
    [0x00404888]> e cfg.bigendian = true
    
    [0x00404888]> pxw
    0x00404888  0x31ed4989 0xd15e4889 0xe24883e4 0xf0505449  1.I..^H..H...PTI
    0x00404898  0xc7c04024 0x410048c7 0xc1b02341 0x0048c7c7  ..@$A.H...#A.H..
    0x004048a8  0xd0284000 0xe83fdcff 0xfff46690 0x662e0f1f  .(@..?....f.f...
    

####8bit hexpair list of bytes

    [0x00404888]> p8 16
    31ed4989d15e4889e24883e4f0505449

####Show hexadecimal quad-words dump (64bit)

    [0x08049A80]> pxq
    0x00001390  0x65625f6b63617473  0x646e6962006e6967   stack_begin.bind
    0x000013a0  0x616d6f6474786574  0x7469727766006e69   textdomain.fwrit
    0x000013b0  0x6b636f6c6e755f65  0x6d63727473006465   e_unlocked.strcm
    ...
    

###3.5.2 Date formats

The current supported timestamp print modes are:

    [0x00404888]> pt?
    |Usage: pt[dn?]
    | pt      print unix time (32 bit cfg.big_endian)
    | ptd     print dos time (32 bit cfg.big_endian)
    | ptn     print ntfs time (64 bit !cfg.big_endian)
    | pt?     show help message

For example, you can 'view' the current buffer as timestamps in ntfs time:

    [0x08048000]> eval cfg.bigendian = false
    [0x08048000]> pt 4
    29:04:32948 23:12:36 +0000
    [0x08048000]> eval cfg.bigendian = true
    [0x08048000]> pt 4
    20:05:13001 09:29:21 +0000

As you can see, the endianness affects to the print formats. Once printing these filetimes you can grep the results by the year for example:

    [0x08048000]> pt | grep 1974 | wc -l
    15
    [0x08048000]> pt | grep 2022
    27:04:2022 16:15:43 +0000
    
The date format printed can be configured with the 'cfg.datefmt' variable following the strftime(3) format.

Extracted from the strftime(3) manpage:

    %a  The abbreviated name of the day of the week according to the current locale.
    %A  The full name of the day of the week according to the current locale.
    %b  The abbreviated month name according to the current locale.
    %B  The full month name according to the current locale.
    %c  The preferred date and time representation for the current locale.
    %C  The century number (year/100) as a 2-digit integer. (SU)
    %d  The day of the month as a decimal number (range 01 to 31).
    %D  Equivalent to %m/%d/%y.  (Yecch—for Americans only.  Americans should note that in other countries %d/%m/%y is rather common. This means that in international context this format is ambiguous and should not be used.) (SU)
    %e  Like %d, the day of the month as a decimal number, but a leading zero is replaced by a space. (SU)
    %E  Modifier: use alternative format, see below. (SU)
    %F  Equivalent to %Y-%m-%d (the ISO 8601 date format). (C99)
    %G  The ISO 8601 week-based year (see NOTES) with century as a decimal number.  The 4-digit year corresponding to the ISO week number (see %V).  This has the same format and value as %Y, except that if the ISO week number belongs to the previous or next year, that year is used instead. (TZ)
    %g  Like %G, but without century, that is, with a 2-digit year (00-99). (TZ)
    %h  Equivalent to %b.  (SU)
    %H  The hour as a decimal number using a 24-hour clock (range 00 to 23).
    %I  The hour as a decimal number using a 12-hour clock (range 01 to 12).
    %j  The day of the year as a decimal number (range 001 to 366).
    %k  The hour (24-hour clock) as a decimal number (range 0 to 23); single digits are preceded by a blank.  (See also %H.)  (TZ)
    %l  The hour (12-hour clock) as a decimal number (range 1 to 12); single digits are preceded by a blank.  (See also %I.)  (TZ)
    %m  The month as a decimal number (range 01 to 12).
    %M  The minute as a decimal number (range 00 to 59).
    %n  A newline character. (SU)
    %O  Modifier: use alternative format, see below. (SU)
    %p  Either "AM" or "PM" according to the given time value, or the corresponding strings for the current locale.  Noon is treated as "PM" and midnight as "AM".
    %P  Like %p but in lowercase: "am" or "pm" or a corresponding string for the current locale. (GNU)
    %r  The time in a.m. or p.m. notation.  In the POSIX locale this is equivalent to %I:%M:%S %p.  (SU)
    %R  The time in 24-hour notation (%H:%M).  (SU) For a version including the seconds, see %T below.
    %s  The number of seconds since the Epoch, 1970-01-01 00:00:00  +0000 (UTC). (TZ)
    %S  The second as a decimal number (range 00 to 60).  (The range is up to 60 to allow for occasional leap seconds.)
    %t  A tab character. (SU)
    %T  The time in 24-hour notation (%H:%M:%S).  (SU)
    %u  The day of the week as a decimal, range 1 to 7, Monday being 1.  See also %w.  (SU)
    %U  The week number of the current year as a decimal number, range 00 to 53, starting with the first Sunday as the first day of week 01.  See also %V and %W.
    %V  The ISO 8601 week number (see NOTES) of the current year as a decimal number, range 01 to 53, where week 1 is the first week that has at least 4 days in the new year.  See also %U and %W.(U)
    %w  The day of the week as a decimal, range 0 to 6, Sunday being 0.  See also %u.
    %W  The week number of the current year as a decimal number, range 00 to 53, starting with the first Monday as the first day of week 01.
    %x  The preferred date representation for the current locale without the time.
    %X  The preferred time representation for the current locale without the date.
    %y  The year as a decimal number without a century (range 00 to 99).
    %Y  The year as a decimal number including the century.
    %z  The +hhmm or -hhmm numeric timezone (that is, the hour and minute offset from UTC). (SU)
    %Z  The timezone name or abbreviation.
    %+  The date and time in date(1) format. (TZ) (Not supported in glibc2.)
    %%  A literal '%' character.
    

###3.5.3 Basic types

All basic types are mapped as print modes. If you are interested in a more complex structure or just type : `pf?`

Here's the list of the print (pf?) modes for basic types:

    Usage: pf[.key[.field[=value]]|[ val]]|[times][format] [arg0 arg1 ...]
    Examples:
      pf 10xiz pointer length string
      pf {array_size}b @ array_base
      pf.             # list all formats
      pf.obj xxdz prev next size name
      pf.obj          # run stored format
      pf.obj.name     # show string inside object
      pf.obj.size=33  # set new size
     Format chars:
      e - temporally swap endian
      f - float value (4 bytes)
      c - char (signed byte)
      b - byte (unsigned)
      B - show 10 first bytes of buffer
      i - %i integer value (4 bytes)
      w - word (2 bytes unsigned short in hex)
      q - quadword (8 bytes)
      p - pointer reference (2, 4 or 8 bytes)
      d - 0x%08x hexadecimal value (4 bytes)
      D - disassemble one opcode
      x - 0x%08x hexadecimal value and flag (fd @ addr)
      z - \0 terminated string
      Z - \0 terminated wide string
      s - 32bit pointer to string (4 bytes)
      S - 64bit pointer to string (8 bytes)
      * - next char is pointer (honors asm.bits)
      + - toggle show flags for each offset
      : - skip 4 bytes
      . - skip 1 byte

Let's see some examples:

    [0x4A13B8C0]> pf i
    0x00404888 = 837634441
    
    [0x4A13B8C0]> pf
    0x00404888 = 837634432.000000







###3.5.4 Source (asm, C)

Valid print code formats are: JSON, C, Python, Cstring (pcj, pc, pcp, pcs) 
    pc     C
    pcs    string
    pcj    json
    pcJ    javascript
    pcp    python
    pcw    words (4 byte)
    pcd    dwords (8 byte)
 
    [0xB7F8E810]> pc 32
    #define _BUFFER_SIZE 32
    unsigned char buffer[_BUFFER_SIZE] = {
    0x89, 0xe0, 0xe8, 0x49, 0x02, 0x00, 0x00, 0x89, 0xc7, 0xe8, 0xe2, 0xff, 0xff, 0xff, 0x81, 0xc3, 0xd6, 0xa7, 0x01, 0x00, 0x8b, 0x83, 0x00, 0xff, 0xff, 0xff, 0x5a, 0x8d, 0x24, 0x84, 0x29, 0xc2 };


    [0x7fcd6a891630]> pcs
    "\x48\x89\xe7\xe8\x68\x39\x00\x00\x49\x89\xc4\x8b\x05\xef\x16\x22\x00\x5a\x48\x8d\x24\xc4\x29\xc2\x52\x48\x89\xd6\x49\x89\xe5\x48\x83\xe4\xf0\x48\x8b\x3d\x06\x1a
    
###3.5.8 Selecting the architecture

The architecture flavour for the disassembly is defined by the 'asm.arch' eval variable. You can use `e asm.arch = ?` to list all available architectures.

    [0xB7F08810]> e asm.arch = ?

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
    _d  16 32      m68k        BSD     Motorola 68000
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
    
    
###3.5.9 Configuring the disassembler

There are multiple options that can be used to configure the output of the disassembly, all these options are described using `e? asm.` 


                  asm.os: Select operating system (kernel) (linux, darwin, w32,..)
               asm.bytes: Display the bytes of each instruction
          asm.cmtflgrefs: Show comment flags associated to branch referece
            asm.cmtright: Show comments at right of disassembly if they fit in screen
            asm.comments: Show comments in disassembly view
              asm.decode: Use code analysis as a disassembler
               asm.dwarf: Show dwarf comment at disassembly
                asm.esil: Show ESIL instead of mnemonic
              asm.filter: Replace numbers in disassembly using flags containing a dot in the name in disassembly
               asm.flags: Show flags
              asm.lbytes: Align disasm bytes to left
               asm.lines: If enabled show ascii-art lines at disassembly
           asm.linescall: Enable call lines
            asm.linesout: If enabled show out of block lines
          asm.linesright: If enabled show lines before opcode instead of offset
          asm.linesstyle: If enabled iterate the jump list backwards
           asm.lineswide: If enabled put an space between lines
              asm.middle: Allow disassembling jumps in the middle of an instruction
              asm.offset: Show offsets at disassembly
              asm.pseudo: Enable pseudo syntax
                asm.size: Show size of opcodes in disassembly (pd)
            asm.stackptr: Show stack pointer at disassembly
              asm.cycles: Show cpu-cycles taken by instruction at disassembly
                asm.tabs: Use tabs in disassembly
               asm.trace: Show execution traces for each opcode
               asm.ucase: Use uppercase syntax at disassembly
              asm.varsub: Substitute variables in disassembly
                asm.arch: Set the arch to be usedd by asm
              asm.parser: Set the asm parser to use
              asm.segoff: Show segmented address in prompt (x86-16)
                 asm.cpu: Set the kind of asm.arch cpu
             asm.profile: configure disassembler (default, simple, gas, smart, debug, full)
               asm.xrefs: Show xrefs in disassembly
           asm.functions: Show functions in disassembly
              asm.syntax: Select assembly syntax
              asm.nbytes: Number of bytes for each opcode at disassembly
           asm.bytespace: Separate hex bytes with a whitespace
                asm.bits: Word size in bits at assembler
          asm.lineswidth: Number of columns for program flow arrows

###3.5.10 Disassembly syntax

The syntax is the flavour of disassembly syntax prefered to be used by the disasm engine.

    e asm.syntax = intel
    e asm.syntax = att

You can also check asm.pseudo which is an experimental pseudocode view and asm.esil, ESIL stands for 'Evaluable Strings Intermedate Language'. It aims to describe a human readable representation for every opcode. Those representations can be evaluated in order to emulate code.
