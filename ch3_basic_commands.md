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
