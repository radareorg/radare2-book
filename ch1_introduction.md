#Chapter 1: Introduction

This book aims to cover most usage aspects of radare2. A framework for reverse engineering and analyzing binaries.

--pancake


##1.1 History

The radare project started in February of 2006 aiming to provide a Free and simple command line interface for an hexadecimal editor supporting 64 bit offsets to make searches and recovering data from hard-disks.

Since then, the project has grown with the aim changed to provide a complete framework for analyzing binaries with some basic *NIX concepts in mind like 'everything is a file', 'small programs that interact together using stdin/out' or 'keep it simple'.

It's mostly a single-person project, but some contributions (in source, patches, ideas or species) have been made and are really appreciated.

The project is composed by an hexadecimal editor as the central point of the project with assembler/disassembler, code analysis, scripting features, analysis and graphs of code and data, easy unix integration, ..


##1.2 Overview

Nowadays the project is composed by a set of small utilities that can be used together or independently from the command line:

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


##1.3 Getting radare

You can get radare from the website http://radare.org/ or Github repo https://github.com/radare/radare2

There are binary packages for multiple operating systems and GNU/Linux distributions (ubuntu, maemo, gentoo, windows, iphone, etc..) But I hardly encourage you to get the sources and compile them yourself to better understand the dependencies and have the source code and examples more accessible.

I try to publish a new stable release every month and sometimes publish nightly tarballs.

But as always the best way to use a software is to go upstream and pull the development repository which in radare is commonly more stable than the 'stable' releases O:)

To do this you will need Git and type:

    $ git clone https://github.com/radare/radare2.git

This will probably take a while, so take a coffee and continue reading this paper.

To update your local copy of the repository you will have to type this command in the root of the recently created 'radare' directory.

    $ git pull

If you have local modifications of the source, you can revert them with:

    $ git reset --hard HEAD

Or just feed me with a patch

    $ git diff > radare-foo.patch

##1.4 Compilation and portability

Currently the core of radare can be compiled in many systems, and architectures, but the main development is done on GNU/Linux and GCC. But it is known to compile with TCC and SunStudio.

People usually wants to use radare as a debugger for reverse engineering, and this is a bit more restrictive portability issue, so if the debugger is not ported to your favorite platform, please, notify it to me or just disable the debugger layer with --without-debugger in the ./configure stage.

Nowadays the debugger layer can be used on Windows, GNU/Linux (intel32, intel64, mips, arm), FreeBSD, NetBSD, OpenBSD (intel32, intel64) and there are plans for Solaris and OSX. And there are some IO plugins to use gdb, gdbremote or wine as backends.

The build system based on ACR/GMAKE.

    $ ./configure --prefix=/usr
    $ gmake
    $ sudo gmake install
    
But there is a simple script to do that automatically:

    $ sys/install.sh

##1.5 Windows compilation

The easy way to compile things for windows is using mingw32. The w32 builds distributed in the radare homepage are generated from a GNU/Linux box using mingw32 and they are tested with wine.

To compile type:

    $ CC=i486-mingw32-gcc ./configure --enable-w32 --without-gui
    $ make
    $ make w32dist
    $ zip -r w32-build.zip w32-build

The 'i486-mingw32-gcc' compiler is the one I have in my box, you will probably need to change this. Mingw32 will generate a native console application for windows.

Another possible way to compile radare on w32 is using cygwin, which I dont really recommend at all because of the problems related to the cygwin librarires makes the program quite hard to be debugged in case of problems.


##1.6 Commandline flags

The core accepts multiple flags from the command line to change some configuration or start with different options.

Here's the help message:

    $ radare2 -h
    Usage: r2 [-dDwntLqv] [-P patch] [-p prj] [-a arch] [-b bits] [-i file] [-s addr] [-B blocksize] [-c cmd] [-e k=v] file|-

       -a [arch]    set asm.arch
       -A           run 'aa' command to analyze all referenced code
       -b [bits]    set asm.bits
       -B [baddr]   set base address for PIE binaries
       -c 'cmd..'   execute radare command
       -C           file is host:port (alias for -c+=http://%s/cmd/)
       -d           use 'file' as a program for debug
       -D [backend] enable debug mode (e cfg.debug=true)
       -e k=v       evaluate config var
       -f           block size = file size
       -h, -hh      show help message, -hh for long
       -i [file]    run script file
       -k [kernel]  set asm.os variable for asm and anal
       -l [lib]     load plugin file
       -L           list supported IO plugins
       -m [addr]    map file at given address
       -n           disable analysis
       -N           disable user settings
       -q           quiet mode (no promt) and quit after -i
       -p [prj]     set project file
       -P [file]    apply rapatch file and quit
       -s [addr]    initial seek
       -S           start r2 in sandbox mode
       -t           load rabin2 info in thread
       -v, -V       show radare2 version (-V show lib versions)
       -w           open file in write mode


##1.7 Basic usage

Lot of people ping me some times for a sample usage session of radare to help to understand how the shell works and how to perform the most common tasks like disassembling, seeking, binary patching or debugging.

I hardly encourage you to read the rest of this book to help you understand better how everything works and enhace your skills, the learning curve of radare is usually a bit harder at the beggining, but after an hour of using it you will easily understand how most of the things work and how to get them cooperate together :)

For walking thru the binary file you will use three different kind of basic actions: seek, print and alterate.

To 'seek' there's an specific command abreviated as `s` than accepts an expression as argument that can be something like `10`, `+0x25` or `[0x100+ptr_table]`. If you are working with block-based files you may prefer to set up the block size to 4K or the size required with the command `b` and move forward or backward at seeks aligned to the block size using the `>` and `<` commands.

The 'print' command aliased as `p`, accepts a second letter to specify the print mode selected. The most common ones are `px` for printing in hexadecimal, `pd` for disassembling.

To 'write' open the file with `radare -w`. This should be specified while opening the file.
You can use the `w` command to write strings or `wx` for hexpair strings:

    > w hello world         ; string
    > wx 90 90 90 90        ; hexpairs
    > wa jmp 0x8048140      ; assemble
    > wf inline.bin         ; write contents of file

Appending a `?` to the command you will get the help message of it. (p? for example)

Enter the visual mode pressing `V<enter>`, and return to the prompt using the `q` key.

In the visual mode you should use hjkl keys which are the default ones for scrolling (like left,down,up,right). So entering in cursor mode (`c`) you will be able select bytes if using the shift together with HJKL.

In the visual mode you can insert (alterate bytes) pressing `i` and then <tab> to switch between the hex or string column. Pressing `q` in hex panel to return into the visual mode.


##1.8 Command format

The format of the commands looks something like that:

    [.][times][cmd][~grep][@[@iter]addr!size][|>pipe] ; ...

Commands are named with single chars [a-zA-Z]. So, if we prefix the command with a number. The following command will be executed as many times as we specify.

    px    # run px
    3px   # run 3 times 'px'

The `!` prefix is used to scape to the shell. If a single exclamation is used then commands will be send to the system() hook defined in the loaded IO plugin. This is used, for example in the ptrace IO plugin which accepts debugger commands from this interface.

Some examples:

    ds                    ; call debugger 'step' command
    px 200 @ esp          ; show 200 hex bytes at esp
    pc > file.c           ; dump buffer as a C byte array to file
    wx 90 @@ sym.*        ; write a nop on every symbol
    pd 2000 | grep eax    ; grep opcodes using 'eax' register
    px 20 ; pd 3 ; px 40  ; multiple commands in a single line

The `@` character is used to specify a temporary offset where the command at the left will be executed. 

The `~` character enables the internal grep which can be used to filter the output of any command. The usage is quite simple:

    pd 20~call            ; disassemble 20 instructions and grep for 'call'

We can either grep for columns or rows:

    pd 20~call:0          ; get first row
    pd 20~call:1          ; get second row

Or even combine them:

    pd 20~call[0]:0       ; grep first column of the first row matching 'call'

The use of internal grep is a key feature for scripting radare, because is used to iterate over list of offsets or data processed from disassembly, ranges, or any other command. Here's an example of usage. See macros section (iterators) for more information.


##1.9 Expressions

The expressions are mathematical representations of a 64 bit numeric value which can be displayed in different formats, compared or used at any command as a numeric argument. They support multiple basic arithmetic operations and some binary and boolean ones. The command used to evaluate these math expressions is the `?`. Here there are some examples:

    [0xB7F9D810]> ? 0x8048000
    134512640 0x8048000 01001100000 128.0M 804000:0000 134512640 00000000 134512640.0 0.000000  
    [0xB7F9D810]> ? 0x8048000+34
    134512674 0x8048022 01001100042 128.0M 804000:0022 134512674 00100010 134512674.0 0.000000  
    [0xB7F9D810]> ? 0x8048000+0x34
    134512692 0x8048034 01001100064 128.0M 804000:0034 134512692 00110100 134512692.0 0.000000  
    [0xB7F9D810]> ? 1+2+3-4*3
    -6 0xfffffffffffffffa 01777777777777777777772 17179869183.0G fffff000:0ffa -6   

The supported arithmetic expressions supported are:

    + : addition
    - : substraction
    * : multiply
    / : division
    % : modulus
    > : shift right
    < : shift left

The binary expressions should be scapped:

    \| : logical OR // ("? 0001010 | 0101001")
    \& : logical AND

The values can be numbers in many formats:

    0x033   : hexadecimal
    3334    : decimal
    sym.fo  : resolve flag offset
    10K     : KBytes  10*1024
    10M     : MBytes  10*1024*1024

There are other special syntaxes for the expressions. Here's for example some of them:

    ?@?    or stype @@?      ; misc help for '@' (seek), '~' (grep) (see ~??)
    ?$?           ; show available '$' variables
    $$            ; here (current virtual seek)
    $l            ; opcode length
    $s            ; file size
    $j            ; jump address (e.g. jmp 0x10, jz 0x10 => 0x10)
    $f            ; jump fail address (e.g. jz 0x10 => next instruction)
    $m            ; opcode memory reference (e.g. mov eax,[0x10] => 0x10)

For example:

    [0x4A13B8C0]> :? $m + $l
    140293837812900 0x7f98b45df4a4 03771426427372244 130658.0G 8b45d000:04a4 140293837812900 10100100 140293837812900.0 -0.000000


    [0x4A13B8C0]> :pd 1 @ +$l
    0x4A13B8C2   call 0x4a13c000   


##1.10 Rax

The `rax` utility comes with the radare framework and aims to be a minimalistic expression evaluator for the shell useful for making base conversions easily between floating point values, hexadecimal representations, hexpair strings to ascii, octal to integer. It supports endianness and can be used as a shell if no arguments given.

    $ rax2 -h
    
    Usage: rax2 [options] [expr ...]
    int   ->  hex           ;  rax2 10
    hex   ->  int           ;  rax2 0xa
    -int  ->  hex           ;  rax2 -77
    -hex  ->  int           ;  rax2 0xffffffb3
    int   ->  bin           ;  rax2 b30
    bin   ->  int           ;  rax2 1010d
    float ->  hex           ;  rax2 3.33f
    hex   ->  float         ;  rax2 Fx40551ed8
    oct   ->  hex           ;  rax2 35o
    hex   ->  oct           ;  rax2 Ox12 (O is a letter)
    bin   ->  hex           ;  rax2 1100011b
    hex   ->  bin           ;  rax2 Bx63
    raw   ->  hex           ;  rax2 -S < /binfile
    hex   ->  raw           ;  rax2 -s 414141
    -b    binstr -> bin     ;  rax2 -b 01000101 01110110
    -B    keep base         ;  rax2 -B 33+3 -> 36
    -d    force integer     ;  rax2 -d 3 -> 3 instead of 0x3
    -e    swap endianness   ;  rax2 -e 0x33
    -f    floating point    ;  rax2 -f 6.3+2.1
    -h    help              ;  rax2 -h
    -k    randomart         ;  rax2 -k 0x34 1020304050
    -n    binary number     ;  rax2 -e 0x1234   # 34120000
    -s    hexstr -> raw     ;  rax2 -s 43 4a 50
    -S    raw -> hexstr     ;  rax2 -S < /bin/ls > ls.hex
    -t    tstamp -> str     ;  rax2 -t 1234567890
    -x    hash string       ;  rax2 -x linux osx
    -u    units             ;  rax2 -u 389289238 # 317.0M
    -v    version           ;  rax2 -V
    

Some examples:

    $ rax2 3+0x80
    0x83

    $ rax2 0x80+3 
    131

    $ echo 0x80+3 | rax2
    131

    $ rax2 -s 4142
    AB

    $ rax2 -S AB 
    4142
    
    $ rax2 -S < bin.foo
    ...

    $ rax2 -e 33 
    0x21000000

    $ rax2 -e 0x21000000 
    33

    $ rax2 -k 90203010
    +--[0x10302090]---+
    |Eo. .            |
    | . . . .         |
    |      o          |
    |       .         |
    |        S        |
    |                 |
    |                 |
    |                 |
    |                 |
    +-----------------+

##1.11 Basic debugger session

To start debugging a program use the `-d` flag and append the PID or the program path with arguments.

    $ r2 -d /bin/ls

The debugger will fork and load the `ls` program in memory stopping the execution in the `ld.so`, so don't expect to see the entrypoint or the mapped libraries at this point. To change this you can define a new 'break entry point' adding `e dbg.bep=entry` or `dbg.bep=main` to your .radarerc.

But take care on this, because some malware or programs can execute code before the main.

Now the debugger prompt should appear and if you press `enter` ( null command ) the basic view of the process will be displayed with the stack dump, general purpose registers and disassembly from current program counter (eip on intel).


Here's a list of the most common commands for the debugger:

    > d?          ; get help on debugger commands
    > ds 3        ; step 3 times
    > db 0x8048920  ; setup a breakpoint
    > db -0x8048920 ; remove a breakpoint
    > dc          ; continue process execution
    > dcs        ; continue until syscall
    > dd            ; manipulate file descriptors
    > dm          ; show process maps
    > dmp A S rwx  ; change page at A with size S protection permissions
    > dr eax=33 ; set register value. eax = 33

The easiest way to use the debugger is from the Visual mode, so, you will no need to remember much commands or keep states in your mind.

    [0xB7F0C8C0]> V

After entering this command an hexdump of the current eip will be showed. Now press `p` one time to get into the debugger view. You can press `p` and `P` to rotate thru the most commonly used print modes.

Use F7 or `s` to step into and F8 or `S` to step over.

With the `c` key you will toggle the cursor mode and being able to select range of bytes to nop them or set breakpoints using the `F2` key.

In the visual mode you can enter commands with `:` to dump buffer contents like

    x @ esi

To get the help in the visual mode press `?`.

At this point the most common commands are !reg that can be used to get or set values for the general purpose registers. You can also manipulate the hardware and extended/floating registers. 
