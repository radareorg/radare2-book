#Chapter 3: Basic commands

The basic set of commands in radare can be mostly grouped by action, and they should be easy to remember and short. This is why they are grouped with a single character, subcommands or related commands are described with a second character. For example `/ foo` for searching plain strings or `/x 90 90` to look for hexpair strings.

The format of the commands (as explained in 'Command format' chapter) looks something like that:

    [[.][times][cmd][~grep][@[@iter]addr!size][|>pipe] ; ...

    > 3s +1024    ; seeks three times 1024 from the current seek
    
If the command starts with `!` the string is passed to the plugin hadling the current IO (the debugger for example), if no one handles it calls to posix_system() which is a shell escape, you can prefix the command with two `!!`.

    > !help       ; handled by the debugger or shell
    > !!ls        ; runs ls in the shell
The [arg] argument depends on the command, but most of them take a number as argument to specify the number of bytes to work on instead of block size. Other commands accept math expressions, or strings.

    > px 0x17     ; show 0x17 bytes in hexa at current seek
    > s base+0x33 ; seeks to flag 'base' plus 0x33
    > / lib       ; search for 'lib' string.
The `@` is used to specify a temporal seek where the command is executed. This is quite useful to not seeking all the time.

    > p8 10 @ 0x4010  ; show 10 bytes at offset 0x4010
    > f patata @ 0x10 ; set 'patata' flag at offset 0x10
Using `@@` you can execute a single command on a list of flags matching the glob:

    > s 0
    > / lib             ; search 'lib' string
    > p8 20 @@ hit0_*   ; show 20 hexpairs at each search hit
    
The `>` is used to pipe the output of the command to a file (truncating to 0 if exist)

    > pr > dump.bin   ; dump 'raw' bytes of current block to 'dump.bin' file
    > f  > flags.txt  ; dump flag list to 'flags.txt'
    
The `|` pipe is used to dump the output of the command to another program.

    [0x4A13B8C0]> f | grep section | grep text
    0x0805f3b0 512 section._text
    0x080d24b0 512 section._text_end
    
Using the `;` chars you can concatenate multiple commands in a single line:

    > px ; dr


##3.1 Seeking

The command `s` is used to seek. It accepts a math expression as argument that can be composed by shift operations, basic math ones and memory access.


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


To 'query' the math expression you can evaluate them using the `?` command and giving the math operation as argument. And getting the result in hexa, decimal, octal and binary.

    > ? 0x100+200
    0x1C8 ; 456d ; 710o ; 1100 1000  


In visual mode just press `u` or `U` to undo or redo inside the seek history.


##3.2 Block size

The block size is the default view size for radare. All commands will work with this constraint, but you can always temporarily change the block size just giving a numeric argument to the print commands for example (px 20)

    [0xB7F9D810]> b?
    Usage: b[f] [arg]
    b         display current block size
    b+3       increase blocksize by 3
    b-16      decrement blocksize by 3
    b 33      set block size to 33
    b eip+4   numeric argument can be an expression
    bf foo    set block size to flag size
    bm 1M     set max block size

The `b` command is used to change the block size:

    [0x00000000]> b 0x100   ; block size = 0x100
    [0x00000000]> b +16     ;  ... = 0x110
    [0x00000000]> b -32     ;  ... = 0xf0
    
The `bf` command is used to change the block size to the one specified by a flag. For example in symbols, the block size of the flag represents the size of the function.

    [0x00000000]> bf sym.main    ; block size = sizeof(sym.main)
    [0x00000000]> pd @ sym.main  ; disassemble sym.main
    ...

You can perform these two operations in a single one (pdf):

     [0x00000000]> pdf @ sym.main


##3.3 Sections

It is usually on firmware images, bootloaders and binary files to find sections that are loaded in memory at different addresses than the one in the disk.

To solve this issue, radare implements `S`.

Here's the help message:

    [0xB7EE8810]> S?
    Usage: S[?-.*=adlr] [...]
    S               ; list sections
    S.              ; show current section name
    S?              ; show this help message
    S*              ; list sections (in radare commands)
    S=              ; list sections (in nice ascii-art bars)
    Sa[-] [arch] [bits] [[off]] ; Specify arch and bits for given section
    Sd [file]       ; dump current section to a file (see dmd)
    Sl [file]       ; load contents of file into current section (see dml)
    Sr [name]       ; rename section on current seek
    S [off] [vaddr] [sz] [vsz] [name] [rwx] ; add new section
    S-[id|0xoff|*]  ; remove this section definition

We can specify a section in a single line in this way:

    S [off] [vaddr] [sz] [vsz] [name] [rwx] ; add new section

For example:

    [0x00404888]> S 0x00000100 0x00400000 0x0001ae08 0001ae08 test rwx

Displaying the sections information:

    [0x00404888]> S ; list sections
    
    [00] . 0x00000238 r-- va=0x00400238 sz=0x0000001c vsz=0000001c .interp
    [01] . 0x00000254 r-- va=0x00400254 sz=0x00000020 vsz=00000020 .note.ABI_tag
    [02] . 0x00000274 r-- va=0x00400274 sz=0x00000024 vsz=00000024 .note.gnu.build_id
    [03] . 0x00000298 r-- va=0x00400298 sz=0x00000068 vsz=00000068 .gnu.hash
    [04] . 0x00000300 r-- va=0x00400300 sz=0x00000c18 vsz=00000c18 .dynsym
    
    
    [0xB7EEA810]> S= ; list sections (in nice ascii-art bars)
    
    ...
    25  0x0001a600 |-----------------------------#| 0x0001a608 --- .gnu_debuglink
    26  0x0001a608 |-----------------------------#| 0x0001a706 --- .shstrtab
    27* 0x00000000 |##############################| 0x0001ae08 rwx ehdr
    =>  0x00004888 |-----^------------------------| 0x00004988
    
    
The first three lines are sections and the last one `=>` is the current seek representation based on the proportions over them.

To remove a section definition just prefix the name of the section with `-`:

    [0xB7EE8810]> S -.dynsym


##3.4 Mapping files

Radare IO allows to virtually map contents of files in the same IO space at random offsets. This is useful to open multiple files in a single view or just to 'emulate' an static environment like if it was in the debugger with the program and all its libraries mapped there.

Using the `S`ections command you'll be able to define different base address for each library loaded at different offsets.

Mapping files is done with the `o` (open) command. Let's read the help:

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


To unmap these files just use the `o-` command giving the file descriptor as argument:

    [0x00000000]> o-14


##3.5 Print modes

One of the efforts in radare is the way to show the information to the user. This is interpreting the bytes and giving an almost readable output format.

The bytes can be represented as integers, shorts, longs, floats, timestamps, hexpair strings, or things more complex like C structures, disassembly, decompilations, external processors, ..

This is a list of the available print modes listable with `p?`:

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
    
The date format printed can be configured with the `cfg.datefmt` variable following the strftime(3) format.

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
      e - temporarily swap endianness
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

###3.5.5 Strings

Strings are probably one of the most important entrypoints while starting to reverse engineer a program because they are usually referencing information about the functions actions ( asserts, debug or info messages, ...).

So it is important for radare to be able to print strings in multiple ways:

    [0x00404888]> ps?
    |Usage: ps[zpw] [N]
    | ps  = print string
    | psb = print strings in current block
    | psx = show string with scaped chars
    | psz = print zero terminated string
    | psp = print pascal string
    | psw = print wide string
    

Most common strings will be just zero-terminated ones. Here's an example by using the debugger to continue the execution of the program until it executes the 'open' syscall. When we recover the control over the process, we get the arguments passed to the syscall, pointed by %ebx. Which is obviously a zero terminated string.

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
    

###3.5.6 Print memory

It is also possible to print various packed data types in a single line using the `pf` command. 


    [0xB7F08810]> pf xxS @ rsp
    0x7fff0d29da30 = 0x00000001 
    0x7fff0d29da34 = 0x00000000 
    0x7fff0d29da38 = 0x7fff0d29da38 -> 0x0d29f7ee /bin/ls
    

This is sometimes useful for looking at the arguments passed to a function, by just giving the 'format memory string' as argument and temporarily changing the current seek with the `@` token.

It is also possible to define arrays of structures with `pf`. Just prefix the format string with a numeric value.

You can also define a name for each field of the structure by giving them as optional arguments after the format string splitted by spaces.
    
    
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

A practical example for using pf on a binary GStreamer plugin:

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
    
###3.5.7 Disassembly

The `pd` command is the one used to disassemble code, it accepts a numeric value to specify how many opcodes are wanted to be disassembled. The `pD` one acts in the same way, but using a number-of-bytes instead of counting instructions.

     d : disassembly N opcodes   count of opcodes
     D : asm.arch disassembler   bsize bytes
 
 
     [0x00404888]> pd 1
                ;-- entry0:
                0x00404888    31ed         xor ebp, ebp
    
###3.5.8 Selecting the architecture

The architecture flavour for the disassembly is defined by the `asm.arch` eval variable. You can use `e asm.arch = ?` to list all available architectures.

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


##3.7 Flags

The flags are bookmarks at a certain offset in the file that can be stored inside 'flag spaces'. A flag space is something like a namespace for flags. They are used to group flags with similar characteristics or of a certain type. Some example of flagspaces could be sections, registers, symbols.

To create a flag just type:

     [0x4A13B8C0]> f flag_name @ offset
    
You can remove this flag adding `-` at the begginging of the command. Most commands accept `-` as argument-prefix as a way to delete.

     [0x4A13B8C0]> f -flag_name 

To switch/create between flagspaces use the `fs` command:

     [0x4A13B8C0]> fs   ; list flag spaces

     00   symbols
     01   imports
     02   sections
     03   strings
     04   regs
     05   maps
     
     [0x4A13B8C0]> fs symbols ; select only flags in symbols flagspace
     [0x4A13B8C0]> f          ; list only flags in symbols flagspace

     [0x4A13B8C0]> fs *      ; select all flagspaces
     
You can rename flags with `fr`.


##3.8 Write

Radare can manipulate the file in multiple ways. You can resize the file, move bytes, copy/paste them, insert mode (shifting data to the end of the block or file) or just overwrite some bytes with an address, the contents of a file, a widestring or inline assembling an opcode.

To resize. Use the `r` command which accepts a numeric argument. Possitive valule sets the new size to the file. A negative one will strip N bytes from the current seek down-sizing the file.

    r 1024      ; resize the file to 1024 bytes
    r -10 @ 33  ; strip 10 bytes at offset 33
To write bytes just use the `w` command. It accepts multiple input formats like inline assembling, endian-friendly dwords, files, hexpair files, wide strings:

    [0x00404888]> w?
    |Usage: w[x] [str] [<file] [<<EOF] [@addr]
    | w foobar     write string 'foobar'
    | wh r2        whereis/which shell command
    | wr 10        write 10 random bytes
    | ww foobar    write wide string 'f\x00o\x00o\x00b\x00a\x00r\x00'
    | wa push ebp  write opcode, separated by ';' (use '"' around the command)
    | waf file     assemble file and write bytes
    | wA r 0       alter/modify opcode at current seek (see wA?)
    | wb 010203    fill current block with cyclic hexpairs
    | wc[ir*?]     write cache undo/commit/reset/list (io.cache)
    | wx 9090      write two intel nops
    | wv eip+34    write 32-64 bit value
    | wo? hex      write in block with operation. 'wo?' fmi
    | wm f0ff      set binary mask hexpair to be used as cyclic write mask
    | ws pstring   write 1 byte for length and then the string
    | wf -|file    write contents of file at current offset
    | wF -|file    write contents of hexpairs file here
    | wp -|file    apply radare patch file. See wp? fmi
    | wt file [sz] write to file (from current seek, blocksize or sz bytes)
    
Some examples:

     [0x00000000]> wx 123456 @ 0x8048300
     [0x00000000]> wv 0x8048123 @ 0x8049100
     [0x00000000]> wa jmp 0x8048320




###3.8.1 Write over with operation

The `wo` write command accepts multiple kinds of operations that can be applied on the curren block. This is for example a XOR, ADD, SUB, ...

    [0x4A13B8C0]> wo?
    |Usage: wo[asmdxoArl24] [hexpairs] @ addr[:bsize]
    |Example:
    |  wox 0x90   ; xor cur block with 0x90
    |  wox 90     ; xor cur block with 0x90
    |  wox 0x0203 ; xor cur block with 0203
    |  woa 02 03  ; add [0203][0203][...] to curblk
    |  woe 02 03  
    |Supported operations:
    |  wow  ==  write looped value (alias for 'wb')
    |  woa  +=  addition
    |  wos  -=  substraction
    |  wom  *=  multiply
    |  wod  /=  divide
    |  wox  ^=  xor
    |  woo  |=  or
    |  woA  &=  and
    |  woR  random bytes (alias for 'wr $b'
    |  wor  >>= shift right
    |  wol  <<= shift left
    |  wo2  2=  2 byte endian swap
    |  wo4  4=  4 byte endian swap


This way it is possible to implement ciphering algorithms using radare core primitives.

A sample session doing a xor(90) + addition(01 02)

    [0x7fcd6a891630]> px
    - offset -       0 1  2 3  4 5  6 7  8 9  A B  C D  E F  0123456789ABCDEF
    0x7fcd6a891630  4889 e7e8 6839 0000 4989 c48b 05ef 1622  H...h9..I......"
    0x7fcd6a891640  005a 488d 24c4 29c2 5248 89d6 4989 e548  .ZH.$.).RH..I..H
    0x7fcd6a891650  83e4 f048 8b3d 061a 2200 498d 4cd5 1049  ...H.=..".I.L..I
    0x7fcd6a891660  8d55 0831 ede8 06e2 0000 488d 15cf e600  .U.1......H.....
    
    
    [0x7fcd6a891630]> wox 90
    [0x7fcd6a891630]> px
    - offset -       0 1  2 3  4 5  6 7  8 9  A B  C D  E F  0123456789ABCDEF
    0x7fcd6a891630  d819 7778 d919 541b 90ca d81d c2d8 1946  ..wx..T........F
    0x7fcd6a891640  1374 60d8 b290 d91d 1dc5 98a1 9090 d81d  .t`.............
    0x7fcd6a891650  90dc 197c 9f8f 1490 d81d 95d9 9f8f 1490  ...|............
    0x7fcd6a891660  13d7 9491 9f8f 1490 13ff 9491 9f8f 1490  ................
    
    
    [0x7fcd6a891630]> woa 01 02
    [0x7fcd6a891630]> px
    - offset -       0 1  2 3  4 5  6 7  8 9  A B  C D  E F  0123456789ABCDEF
    0x7fcd6a891630  d91b 787a 91cc d91f 1476 61da 1ec7 99a3  ..xz.....va.....
    0x7fcd6a891640  91de 1a7e d91f 96db 14d9 9593 1401 9593  ...~............
    0x7fcd6a891650  c4da 1a6d e89a d959 9192 9159 1cb1 d959  ...m...Y...Y...Y
    0x7fcd6a891660  9192 79cb 81da 1652 81da 1456 a252 7c77  ..y....R...V.R|w

##3.9 Yank/Paste

You can yank/paste bytes in visual mode using the `y` and `Y` key bindings that are alias for the `y` and `yy` commands of the shell. There is an internal buffer that stores N bytes from the current seek. You can write-back to another seek using the `yy` one.

    [0x00000000]> y?
    |Usage: y[ptxy] [len] [[@]addr]
    | y                show yank buffer information (srcoff len bytes)
    | y 16             copy 16 bytes into clipboard
    | y 16 0x200       copy 16 bytes into clipboard from 0x200
    | y 16 @ 0x200     copy 16 bytes into clipboard from 0x200
    | yp               print contents of clipboard
    | yx               print contents of clipboard in hexadecimal
    | yt 64 0x200      copy 64 bytes from current seek to 0x200
    | yf 64 0x200 file copy 64 bytes from 0x200 from file (opens w/ io), use -1 for all bytes
    | yfa file copy    copy all bytes from from file (opens w/ io)
    | yy 0x3344        paste clipboard

Sample session:

    [0x00000000]> s 0x100    ; seek at 0x100
    [0x00000100]> y 100      ; yanks 100 bytes from here
    [0x00000200]> s 0x200    ; seek 0x200
    [0x00000200]> yy         ; pastes 100 bytes
    
    
You can perform a yank and paste in a single line by just using the `yt` command (yank-to). The syntax is the following:

    [0x4A13B8C0]> x
       offset   0 1  2 3  4 5  6 7  8 9  A B  0123456789AB
    0x4A13B8C0, 89e0 e839 0700 0089 c7e8 e2ff ...9........
    0x4A13B8CC, ffff 81c3 eea6 0100 8b83 08ff ............
    0x4A13B8D8, ffff 5a8d 2484 29c2           ..Z.$.).    
    
    [0x4A13B8C0]> yt 8 0x4A13B8CC @ 0x4A13B8C0
    
    [0x4A13B8C0]> x
       offset   0 1  2 3  4 5  6 7  8 9  A B  0123456789AB
    0x4A13B8C0, 89e0 e839 0700 0089 c7e8 e2ff ...9........
    0x4A13B8CC, 89e0 e839 0700 0089 8b83 08ff ...9........
    0x4A13B8D8, ffff 5a8d 2484 29c2           ..Z.$.).    


###3.10 Comparing bytes

You can compare data using the `c` command that accepts different input formats and compares the input against the bytes in the current seek.

    [0x00404888]> c?
    |Usage: c[?dfx] [argument]
    | c  [string]    Compares a plain with escaped chars string
    | cc [at] [(at)] Compares in two hexdump columns of block size
    | c4 [value]     Compare a doubleword from a math expression
    | c8 [value]     Compare a quadword from a math expression
    | cx [hexpair]   Compare hexpair string
    | cX [addr]      Like 'cc' but using hexdiff output
    | cf [file]      Compare contents of file at current seek
    | cg[o] [file]   Graphdiff current file and [file]
    | cu [addr] @at  Compare memory hexdumps of $$ and dst in unified diff
    | cw[us?] [...]  Compare memory watchers
    | cat  [file]    Show contents of file (see pwd, ls)
    | cl|cls|clear   Clear screen, (clear0 to goto 0, 0 only)
    
An example of memory comparision:

    [0x08048000]> p8 4
    7f 45 4c 46 
    
    [0x08048000]> cx 7f 45 90 46
    Compare 3/4 equal bytes
    0x00000002 (byte=03)   90 ' '  ->  4c 'L'
    [0x08048000]> 

Another subcommand of `c` (compare) command is the `cc` which stands for 'compare code'.

    [0x4A13B8C0]> cc 0x39e8e089 @ 0x4A13B8C0
    
    [0x08049A80]> cc sym.main2 @ sym.main

`c8` Compare a quadword from the current seek (0x00000000) from a math expression

    [0x00000000]> c8 4
    
    Compare 1/8 equal bytes (0%)
    0x00000000 (byte=01)   7f ' '  ->  04 ' '
    0x00000001 (byte=02)   45 'E'  ->  00 ' '
    0x00000002 (byte=03)   4c 'L'  ->  00 ' '


This number can be an math expressions using flag names and so:

    [0x00000000]> cx 7f469046
    
    Compare 2/4 equal bytes
    0x00000001 (byte=02)   45 'E'  ->  46 'F'
    0x00000002 (byte=03)   4c 'L'  ->  90 ' '



We can use the compare command against a file previously dumped to disk from the contents of the current block.

    r2 /bin/true
    [0x08049A80]> s 0
    [0x08048000]> cf /bin/true
    Compare 512/512 equal bytes

