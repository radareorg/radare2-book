## Print Modes

One of the key features of radare is displaying information in many formats. The goal is to offer a selection of displaying choices to best interpret binary data.

Binary data can be represented as integers, shorts, longs, floats, timestamps, hexpair strings, or more complex formats like C structures, disassembly listings, decompilations, be a result of an external processing...

Below is a list of available print modes listed by `p?`:

    [0x08049AD0]> p?
    |Usage: p[=68abcdDfiImrstuxz] [arg|len] [@addr]
    | p=[?][bep] [blks] [len] [blk]  show entropy/printable chars/chars bars
    | p2 [len]                       8x8 2bpp-tiles
    | p3 [file]                      print stereogram (3D)
    | p6[de] [len]                   base64 decode/encode
    | p8[?][j] [len]                 8bit hexpair list of bytes
    | pa[edD] [arg]                  pa:assemble  pa[dD]:disasm or pae: esil from hexpairs
    | pA[n_ops]                      show n_ops address and type
    | p[b|B|xb] [len] ([skip])       bindump N bits skipping M
    | pb[?] [n]                      bitstream of N bits
    | pB[?] [n]                      bitstream of N bytes
    | pc[?][p] [len]                 output C (or python) format
    | pC[d] [rows]                   print disassembly in columns (see hex.cols and pdi)
    | pd[?] [sz] [a] [b]             disassemble N opcodes (pd) or N bytes (pD)
    | pf[?][.nam] [fmt]              print formatted data (pf.name, pf.name $<expr>)
    | ph[?][=|hash] ([len])          calculate hash for a block
    | p[iI][df] [len]                print N ops/bytes (f=func) (see pi? and pdi)
    | pm[?] [magic]                  print libmagic data (see pm? and /m?)
    | pr[?][glx] [len]               print N raw bytes (in lines or hexblocks, 'g'unzip)
    | p[kK] [len]                    print key in randomart (K is for mosaic)
    | ps[?][pwz] [len]               print pascal/wide/zero-terminated strings
    | pt[?][dn] [len]                print different timestamps
    | pu[?][w] [len]                 print N url encoded bytes (w=wide)
    | pv[?][jh] [mode]               show variable/pointer/value in memory
    | p-[?][jh] [mode]               bar|json|histogram blocks (mode: e?search.in)
    | px[?][owq] [len]               hexdump of N bytes (o=octal, w=32bit, q=64bit)
    | pz[?] [len]                    print zoom view (see pz? for help)
    | pwd                            display current working directory

Tip: when using json output, you can append the `~{}` to the command to get a pretty-printed version of the output:

    [0x00000000]> oj
    [{"raised":false,"fd":563280,"uri":"malloc://512","from":0,"writable":true,"size":512,"overlaps":false}]
    [0x00000000]> oj~{}
    [
        {
            "raised": false,
            "fd": 563280,
            "uri": "malloc://512",
            "from": 0,
            "writable": true,
            "size": 512,
            "overlaps": false
        }
    ]

For more on the magical powers of `~` see the help in `?@?`, and the "Command Format" chapter earlier in the book.

### Hexadecimal View

`px` gives a user-friendly output showing 16 pairs of numbers per row with offsets and raw representations:

    [0x00404888]> px
    - offset -   0 1  2 3  4 5  6 7  8 9  A B  C D  E F  0123456789ABCDEF
    0x00404888  31ed 4989 d15e 4889 e248 83e4 f050 5449  1.I..^H..H...PTI
    0x00404898  c7c0 4024 4100 48c7 c1b0 2341 0048 c7c7  ..@$A.H...#A.H..
    0x004048a8  d028 4000 e83f dcff fff4 6690 662e 0f1f  .(@..?....f.f...

#### Show Hexadecimal Words Dump (32 bits)

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


#### 8 bits Hexpair List of Bytes

    [0x00404888]> p8 16
    31ed4989d15e4889e24883e4f0505449

#### Show Hexadecimal Quad-words Dump (64 bits)

    [0x08049A80]> pxq
    0x00001390  0x65625f6b63617473  0x646e6962006e6967   stack_begin.bind
    0x000013a0  0x616d6f6474786574  0x7469727766006e69   textdomain.fwrit
    0x000013b0  0x6b636f6c6e755f65  0x6d63727473006465   e_unlocked.strcm
    ...


### Date/Time Formats

Currently supported timestamp output modes are:

    [0x00404888]> pt?
    |Usage: pt[dn?]
    | pt      print unix time (32 bit cfg.big_endian)
    | ptd     print dos time (32 bit cfg.big_endian)
    | ptn     print ntfs time (64 bit !cfg.big_endian)
    | pt?     show help message

For example, you can 'view' the current buffer as timestamps in the ntfs time:

    [0x08048000]> eval cfg.bigendian = false
    [0x08048000]> pt 4
    29:04:32948 23:12:36 +0000
    [0x08048000]> eval cfg.bigendian = true
    [0x08048000]> pt 4
    20:05:13001 09:29:21 +0000

As you can see, the endianness affects the result. Once you have printed a timestamp, you can grep output, for example, by year value:

    [0x08048000]> pt | grep 1974 | wc -l
    15
    [0x08048000]> pt | grep 2022
    27:04:2022 16:15:43 +0000

The default date format can be configured using the `cfg.datefmt` variable. Formatting rules for it follow the well known strftime(3) format. An excerpt from the strftime(3) manpage:

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


### Basic Types

There are print modes available for all basic types. If you are interested in a more complex structure, type `pf??` for format characters and `pf???` for examples:

    [0x00499999]> pf??
    |pf: pf[.k[.f[=v]]|[v]]|[n]|[0|cnt][fmt] [a0 a1 ...]
    | Format:
    |  b       byte (unsigned)
    |  B       resolve enum bitfield (see t?)
    |  c       char (signed byte)
    |  d       0x%%08x hexadecimal value (4 bytes) (see %%i and %%x)
    |  D       disassemble one opcode
    |  e       temporally swap endian
    |  E       resolve enum name (see t?)
    |  f       float value (4 bytes)
    |  i       %%i signed integer value (4 bytes) (see %%d and %%x)
    |  n       next char specifies size of signed value (1, 2, 4 or 8 byte(s))
    |  N       next char specifies size of unsigned value (1, 2, 4 or 8 byte(s))
    |  o       0x%%08o octal value (4 byte)
    |  p       pointer reference (2, 4 or 8 bytes)
    |  q       quadword (8 bytes)
    |  r       CPU register `pf r (eax)plop`
    |  s       32bit pointer to string (4 bytes)
    |  S       64bit pointer to string (8 bytes)
    |  t       UNIX timestamp (4 bytes)
    |  T       show Ten first bytes of buffer
    |  u       uleb128 (variable length)
    |  w       word (2 bytes unsigned short in hex)
    |  x       0x%%08x hex value and flag (fd @ addr) (see %%d and %%i)
    |  X       show formatted hexpairs
    |  z       \0 terminated string
    |  Z       \0 terminated wide string
    |  ?       data structure `pf ? (struct_name)example_name`
    |  *       next char is pointer (honors asm.bits)
    |  +       toggle show flags for each offset
    |  :       skip 4 bytes
    |  .       skip 1 byte
    [0x00499999]> pf???
    |pf: pf[.k[.f[=v]]|[v]]|[n]|[0|cnt][fmt] [a0 a1 ...]
    | Examples:
    | pf 3xi foo bar                               3-array of struct, each with named fields: 'foo' as hex, and 'bar' as int
    | pf B (BitFldType)arg_name`                   bitfield type
    | pf E (EnumType)arg_name`                     enum type
    | pf.obj xxdz prev next size name              Define the obj format as xxdz
    | pf obj=xxdz prev next size name              Same as above
    | pf iwq foo bar troll                         Print the iwq format with foo, bar, troll as the respective names for the fields
    | pf 0iwq foo bar troll                        Same as above, but considered as a union (all fields at offset 0)
    | pf.plop ? (troll)mystruct                    Use structure troll previously defined
    | pf 10xiz pointer length string               Print a size 10 array of the xiz struct with its field names
    | pf {integer}? (bifc)                         Print integer times the following format (bifc)
    | pf [4]w[7]i                                  Print an array of 4 words and then an array of 7 integers
    | pf ic...?i foo bar "(pf xw yo foo)troll" yo  Print nested anonymous structres
    | pfn2                                         print signed short (2 bytes) value. Use N insted of n for printing unsigned values

Some examples are below:

    [0x4A13B8C0]> pf i
    0x00404888 = 837634441

    [0x4A13B8C0]> pf
    0x00404888 = 837634432.000000

### High-level Languages Views

Valid print code formats for human-readable languages are:

    pc     C
    pc*    print 'wx' r2 commands
    pch    C half-words (2 byte)
    pcw    C words (4 byte)
    pcd    C dwords (8 byte)
    pca    GAS .byte blob
    pcA    .bytes with instructions in comments
    pcs    string
    pcS    shellscript that reconstructs the bin
    pcj    json
    pcJ    javascript
    pcp    python

    [0xB7F8E810]> pc 32
    #define _BUFFER_SIZE 32
    unsigned char buffer[_BUFFER_SIZE] = {
    0x89, 0xe0, 0xe8, 0x49, 0x02, 0x00, 0x00, 0x89, 0xc7, 0xe8, 0xe2, 0xff, 0xff, 0xff, 0x81, 0xc3, 0xd6, 0xa7, 0x01, 0x00, 0x8b, 0x83, 0x00, 0xff, 0xff, 0xff, 0x5a, 0x8d, 0x24, 0x84, 0x29, 0xc2 };


    [0x7fcd6a891630]> pcs
    "\x48\x89\xe7\xe8\x68\x39\x00\x00\x49\x89\xc4\x8b\x05\xef\x16\x22\x00\x5a\x48\x8d\x24\xc4\x29\xc2\x52\x48\x89\xd6\x49\x89\xe5\x48\x83\xe4\xf0\x48\x8b\x3d\x06\x1a

### Strings

Strings are probably one of the most important entrypoints when starting to reverse engineer a program, because they usually reference information about functions' actions (asserts, debug or info messages...) Therefore radare supports various string formats:

    [0x00000000]> ps?
    |Usage: ps[zpw] [N]Print String
    | ps   print string
    | pss  print string in screen (wrap width)
    | psi  print string inside curseek
    | psb  print strings in current block
    | psx  show string with escaped chars
    | psz  print zero terminated string
    | psp  print pascal string
    | psu  print utf16 unicode (json)
    | psw  print 16bit wide string
    | psW  print 32bit wide string
    | psj  print string in JSON format

Most strings are zero-terminated. Here is an example by using the debugger to continue the execution of a program until it executes the 'open' syscall. When we recover the control over the process, we get the arguments passed to the syscall, pointed by %ebx. In the case of the 'open' call, it is a zero terminated string which we can inspect using `psz`.

    [0x4A13B8C0]> dcs open
    0x4a14fc24 syscall(5) open ( 0x4a151c91 0x00000000 0x00000000 ) = 0xffffffda
    [0x4A13B8C0]> dr
      eax  0xffffffda    esi  0xffffffff    eip    0x4a14fc24
      ebx  0x4a151c91    edi  0x4a151be1    oeax   0x00000005
      ecx  0x00000000    esp  0xbfbedb1c    eflags 0x200246  
      edx  0x00000000    ebp  0xbfbedbb0    cPaZstIdor0 (PZI)
    [0x4A13B8C0]>
    [0x4A13B8C0]> psz @ 0x4a151c91
    /etc/ld.so.cache


### Print Memory Contents

It is also possible to print various packed data types using the `pf` command:

    [0xB7F08810]> pf xxS @ rsp
    0x7fff0d29da30 = 0x00000001
    0x7fff0d29da34 = 0x00000000
    0x7fff0d29da38 = 0x7fff0d29da38 -> 0x0d29f7ee /bin/ls

This can be used to look at the arguments passed to a function. To achieve this, simply pass a 'format memory string' as an argument to `pf`, and temporally change current seek position / offset using `@`. It is also possible to define arrays of structures with `pf`. To do this, prefix the format string with a numeric value. You can also define a name for each field of the structure by appending them as a space-separated arguments list.

    [0x4A13B8C0]> pf 2*xw pointer type @ esp
    0x00404888 [0] {
       pointer :
    (*0xffffffff8949ed31)      type : 0x00404888 = 0x8949ed31
       0x00404890 = 0x48e2
    }
    0x00404892 [1] {
    (*0x50f0e483)    pointer : 0x00404892 = 0x50f0e483
         type : 0x0040489a = 0x2440
    }

A practical example for using `pf` on a binary of a GStreamer plugin:

    $ radare ~/.gstreamer-0.10/plugins/libgstflumms.so
    [0x000028A0]> seek sym.gst_plugin_desc
    [0x000185E0]> pf iissxsssss major minor name desc _init version \
     license source package origin
         major : 0x000185e0 = 0
         minor : 0x000185e4 = 10
          name : 0x000185e8 = 0x000185e8 flumms
          desc : 0x000185ec = 0x000185ec Fluendo MMS source
         _init : 0x000185f0 = 0x00002940
       version : 0x000185f4 = 0x000185f4 0.10.15.1
       license : 0x000185f8 = 0x000185f8 unknown
        source : 0x000185fc = 0x000185fc gst-fluendo-mms
       package : 0x00018600 = 0x00018600 Fluendo MMS source
        origin : 0x00018604 = 0x00018604 http://www.fluendo.com

### Disassembly

The `pd` command is used to disassemble code. It accepts a numeric value to specify how many instructions should be disassembled. The `pD` command is similar but instead of a number of instructions, it decompiles a given number of bytes.

     d : disassembly N opcodes   count of opcodes
     D : asm.arch disassembler   bsize bytes

     [0x00404888]> pd 1
                ;-- entry0:
                0x00404888    31ed         xor ebp, ebp

### Selecting Target Architecture

The architecture flavor for disassembler is defined by the `asm.arch` eval variable. You can use `e asm.arch = ?` to list all available architectures.

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


### Configuring the Disassembler

There are multiple options which can be used to configure the output of disassembler. All these options are described in `e? asm.`

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

### Disassembly Syntax

The `asm.syntax` variable is used to change flavor of assembly syntax used by a disassembler engine. To switch between Intel and AT&T representations:

    e asm.syntax = intel
    e asm.syntax = att

You can also check `asm.pseudo`, which is an experimental pseudocode view, and `asm.esil` which outputs ESIL ('Evaluable Strings Intermedate Language'). ESIL's goal is to have a human-readable representation of every opcode semantics. Such representations can be evaluated (interpreted) to emulate effects of individual instructions.
