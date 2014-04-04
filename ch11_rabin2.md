#Chapter 11: Rabin2

Under this bunny-arabic-like name, radare hides the power of a wonderful tool to handle binary files and get information to show it in the command line or import it into the core.

Rabin2 is able to handle multiple file formats like Java CLASS, ELF, PE, MACH-O, etc.. and it is able to get symbol import/exports, library dependencies, strings of data sections, xrefs, address of entrypoint, sections, architecture type, etc.

    $ rabin2 -h

    Usage: rabin2 [-ACdehHiIjlLMqrRsSvVxzZ] [-@ addr] [-a arch] [-b bits]
                  [-B addr] [-c F:C:D] [-f str] [-m addr] [-n str] [-N len]
                  [-o str] [-O str] file
     -@ [addr]       show section, symbol or import at addr
     -A              list archs
     -a [arch]       set arch (x86, arm, .. or <arch>_<bits>)
     -b [bits]       set bits (32, 64 ...)
     -B [addr]       override base address (pie bins)
     -c [fmt:C:D]    create [elf,mach0,pe] with Code and Data hexpairs (see -a)
     -C              list classes
     -d              show debug/dwarf information
     -e              entrypoint
     -f [str]        select sub-bin named str
     -g              same as -SMRevsiz (show all info)
     -h              this help
     -H              header fields
     -i              imports (symbols imported from libraries)
     -I              binary info
     -j              output in json
     -l              linked libraries
     -L              list supported bin plugins
     -m [addr]       show source line at addr
     -M              main (show address of main symbol)
     -n [str]        show section, symbol or import named str
     -N [minlen]     force minimum number of chars per string (see -z)
     -o [str]        output file/folder for write operations (out by default)
     -O [str]        write/extract operations (-O help)
     -q              be quiet, just show fewer data
     -r              radare output
     -R              relocations
     -s              symbols (exports)
     -S              sections
     -v              use vaddr in radare output (or show version if no file)
     -x              extract bins contained in file
     -z              strings (from data section)
     -zz             strings (from raw bins [e bin.rawstr=1])
     -Z              guess size of binary program


##11.1 File identification

The file identification is done through the -I flag, it will output information regarding binary class, encoding, OS, type, etc.

    $ rabin2 -I /bin/ls
    file    /bin/ls
    type    EXEC (Executable file)
    pic false
    has_va  true
    root    elf
    class   ELF64
    lang    c
    arch    x86
    bits    64
    machine AMD x86-64 architecture
    os  linux
    subsys  linux
    endian  little
    strip   true
    static  false
    linenum false
    lsyms   false
    relocs  false
    rpath   NONE
    
As it was said we can add the -r flag to use all this information in radare:

    $ rabin2 -Ir /bin/ls
    e file.type=elf
    e cfg.bigendian=false
    e asm.os=linux
    e asm.arch=x86
    e anal.arch=x86
    e asm.bits=64
    e asm.dwarf=true



##11.2 Entrypoint

The flag "-e" lets us know the program entrypoint.

    $ rabin2 -e /bin/ls
    [Entrypoints]
    addr=0x00004888 off=0x00004888 baddr=0x00000000

    1 entrypoints

    $ rabin2 -er /bin/ls
    fs symbols
    f entry0 @ 0x00004888
    s entry0


##11.3 Imports

Rabin2 is able to get all the imported objects, as well as their offset at the PLT, this information is quite useful, for example, to recognize wich function is called by a call instruction.

    $ rabin2 -i /bin/ls |head
    [Imports]
    ordinal=001 plt=0x000021b0 bind=GLOBAL type=FUNC name=__ctype_toupper_loc
    ordinal=002 plt=0x000021c0 bind=GLOBAL type=FUNC name=__uflow
    ordinal=003 plt=0x000021d0 bind=GLOBAL type=FUNC name=getenv
    ordinal=004 plt=0x000021e0 bind=GLOBAL type=FUNC name=sigprocmask
    ordinal=005 plt=0x000021f0 bind=GLOBAL type=FUNC name=raise
    ordinal=006 plt=0x00002210 bind=GLOBAL type=FUNC name=localtime
    ordinal=007 plt=0x00002220 bind=GLOBAL type=FUNC name=__mempcpy_chk
    ordinal=008 plt=0x00002230 bind=GLOBAL type=FUNC name=abort
    ordinal=009 plt=0x00002240 bind=GLOBAL type=FUNC name=__errno_location
    (...)


##11.4 Symbols (exports)

In rabin, symbols list works in a very similar way as exports do.

    $ rabin2 -s /bin/ls | head
    [Symbols]
    addr=0x0021a610 off=0x0021a610 ord=114 fwd=NONE sz=8 bind=GLOBAL type=OBJECT name=stdout
    addr=0x0021a600 off=0x0021a600 ord=115 fwd=NONE sz=0 bind=GLOBAL type=NOTYPE name=_edata
    addr=0x0021b388 off=0x0021b388 ord=116 fwd=NONE sz=0 bind=GLOBAL type=NOTYPE name=_end
    addr=0x0021a600 off=0x0021a600 ord=117 fwd=NONE sz=8 bind=GLOBAL type=OBJECT name=__progname
    addr=0x0021a630 off=0x0021a630 ord=119 fwd=NONE sz=8 bind=UNKNOWN type=OBJECT name=program_invocation_name
    addr=0x0021a600 off=0x0021a600 ord=121 fwd=NONE sz=0 bind=GLOBAL type=NOTYPE name=__bss_start
    addr=0x0021a630 off=0x0021a630 ord=122 fwd=NONE sz=8 bind=GLOBAL type=OBJECT name=__progname_full
    addr=0x0021a600 off=0x0021a600 ord=123 fwd=NONE sz=8 bind=UNKNOWN type=OBJECT name=program_invocation_short_name
    addr=0x00002178 off=0x00002178 ord=124 fwd=NONE sz=0 bind=GLOBAL type=FUNC name=_init

With -r radare core can flag automatically all these symbols and define function and data blocks.
    
    $ rabin2 -sr /bin/ls
    
    fs symbols
    Cd 8 @ 0x0021a610
    f sym.stdout 8 0x0021a610
    f sym._edata 0 0x0021a600
    f sym._end 0 0x0021b388
    Cd 8 @ 0x0021a600
    f sym.__progname 8 0x0021a600
    Cd 8 @ 0x0021a630
    f sym.program_invocation_name 8 0x0021a630
    f sym.__bss_start 0 0x0021a600


##11.5 Libraries

Rabin2 can list the libraries used by a binary with the flag -l.

    $ rabin2 -l /bin/ls
    [Linked libraries]
    libselinux.so.1
    librt.so.1
    libacl.so.1
    libc.so.6
    
    4 libraries

    
If you compare the output of 'rabin2 -l' and 'ldd' you will notice that rabin will list less libraries than 'ldd'. The reason is that rabin will not follow the dependencies of the listed libraries, it will just display the ones listed in the binary itself.


##11.6 Strings

The -z flag is used to list all the strings located in the section .rodata for ELF binaries, and .text for PE ones.

    $ rabin2 -z /bin/ls |head
    addr=0x00012487 off=0x00012487 ordinal=000 sz=9 len=9 section=.rodata type=A string=src/ls.c
    addr=0x00012490 off=0x00012490 ordinal=001 sz=26 len=26 section=.rodata type=A string=sort_type != sort_version
    addr=0x000124aa off=0x000124aa ordinal=002 sz=5 len=5 section=.rodata type=A string= %lu
    addr=0x000124b0 off=0x000124b0 ordinal=003 sz=7 len=14 section=.rodata type=W string=%*lu ?
    addr=0x000124ba off=0x000124ba ordinal=004 sz=8 len=8 section=.rodata type=A string=%s %*s 
    addr=0x000124c5 off=0x000124c5 ordinal=005 sz=10 len=10 section=.rodata type=A string=%*s, %*s 
    addr=0x000124cf off=0x000124cf ordinal=006 sz=5 len=5 section=.rodata type=A string= -> 
    addr=0x000124d4 off=0x000124d4 ordinal=007 sz=17 len=17 section=.rodata type=A string=cannot access %s
    addr=0x000124e5 off=0x000124e5 ordinal=008 sz=29 len=29 section=.rodata type=A string=cannot read symbolic link %s
    addr=0x00012502 off=0x00012502 ordinal=009 sz=10 len=10 section=.rodata type=A string=unlabeled
    

With -r all this information is converted to radare2 commands, which will create a flag space called "strings" filled with flags for all those strings. 
Furtheremore, it will redefine them as strings insted of code.

    $ rabin2 -zr /bin/ls |head
    fs strings
    f str.src_ls.c 9 @ 0x00012487
    Cs 9 @ 0x00012487
    f str.sort_type__sort_version 26 @ 0x00012490
    Cs 26 @ 0x00012490
    f str._lu 5 @ 0x000124aa
    Cs 5 @ 0x000124aa
    f str.__lu_ 14 @ 0x000124b0
    Cs 7 @ 0x000124b0
    f str._s__s 8 @ 0x000124ba
    (...)

##11.7 Program sections

Rabin2 give us complete information about the program sections. We can know their index, offset, size, align, type and permissions, as we can see in the next example.

    $ rabin2 -S /bin/ls
 [Sections]
idx=00 addr=0x00000238 off=0x00000238 sz=28 vsz=28 perm=-r-- name=.interp
idx=01 addr=0x00000254 off=0x00000254 sz=32 vsz=32 perm=-r-- name=.note.ABI_tag
idx=02 addr=0x00000274 off=0x00000274 sz=36 vsz=36 perm=-r-- name=.note.gnu.build_id
idx=03 addr=0x00000298 off=0x00000298 sz=104 vsz=104 perm=-r-- name=.gnu.hash
idx=04 addr=0x00000300 off=0x00000300 sz=3096 vsz=3096 perm=-r-- name=.dynsym
idx=05 addr=0x00000f18 off=0x00000f18 sz=1427 vsz=1427 perm=-r-- name=.dynstr
idx=06 addr=0x000014ac off=0x000014ac sz=258 vsz=258 perm=-r-- name=.gnu.version
idx=07 addr=0x000015b0 off=0x000015b0 sz=160 vsz=160 perm=-r-- name=.gnu.version_r
idx=08 addr=0x00001650 off=0x00001650 sz=168 vsz=168 perm=-r-- name=.rela.dyn
idx=09 addr=0x000016f8 off=0x000016f8 sz=2688 vsz=2688 perm=-r-- name=.rela.plt
idx=10 addr=0x00002178 off=0x00002178 sz=26 vsz=26 perm=-r-x name=.init
idx=11 addr=0x000021a0 off=0x000021a0 sz=1808 vsz=1808 perm=-r-x name=.plt
idx=12 addr=0x000028b0 off=0x000028b0 sz=64444 vsz=64444 perm=-r-x name=.text
idx=13 addr=0x0001246c off=0x0001246c sz=9 vsz=9 perm=-r-x name=.fini
idx=14 addr=0x00012480 off=0x00012480 sz=20764 vsz=20764 perm=-r-- name=.rodata
idx=15 addr=0x0001759c off=0x0001759c sz=1820 vsz=1820 perm=-r-- name=.eh_frame_hdr
idx=16 addr=0x00017cb8 off=0x00017cb8 sz=8460 vsz=8460 perm=-r-- name=.eh_frame
idx=17 addr=0x00019dd8 off=0x00019dd8 sz=8 vsz=8 perm=-rw- name=.init_array
idx=18 addr=0x00019de0 off=0x00019de0 sz=8 vsz=8 perm=-rw- name=.fini_array
idx=19 addr=0x00019de8 off=0x00019de8 sz=8 vsz=8 perm=-rw- name=.jcr
idx=20 addr=0x00019df0 off=0x00019df0 sz=512 vsz=512 perm=-rw- name=.dynamic
idx=21 addr=0x00019ff0 off=0x00019ff0 sz=16 vsz=16 perm=-rw- name=.got
idx=22 addr=0x0001a000 off=0x0001a000 sz=920 vsz=920 perm=-rw- name=.got.plt
idx=23 addr=0x0001a3a0 off=0x0001a3a0 sz=608 vsz=608 perm=-rw- name=.data
idx=24 addr=0x0001a600 off=0x0001a600 sz=3464 vsz=3464 perm=-rw- name=.bss
idx=25 addr=0x0001a600 off=0x0001a600 sz=8 vsz=8 perm=---- name=.gnu_debuglink
idx=26 addr=0x0001a608 off=0x0001a608 sz=254 vsz=254 perm=---- name=.shstrtab

27 sections

Also, using -r, radare will flag the beginning and end of each section, as well as comment each one with the previous information.

    $ rabin2 -Sr /bin/ls
    fs sections
    S 0x00000238 0x00000238 0x0000001c 0x0000001c .interp 4
    f section..interp 28 0x00000238
    f section_end..interp 0 0x00000254
    CC [00] va=0x00000238 pa=0x00000238 sz=28 vsz=28 rwx=-r-- .interp @ 0x00000238
    S 0x00000254 0x00000254 0x00000020 0x00000020 .note.ABI_tag 4
    f section..note.ABI_tag 32 0x00000254
    f section_end..note.ABI_tag 0 0x00000274
    CC [01] va=0x00000254 pa=0x00000254 sz=32 vsz=32 rwx=-r-- .note.ABI_tag @ 0x00000254
    S 0x00000274 0x00000274 0x00000024 0x00000024 .note.gnu.build_id 4
    f section..note.gnu.build_id 36 0x00000274
    f section_end..note.gnu.build_id 0 0x00000298
    CC [02] va=0x00000274 pa=0x00000274 sz=36 vsz=36 rwx=-r-- .note.gnu.build_id @ 0x00000274
    S 0x00000298 0x00000298 0x00000068 0x00000068 .gnu.hash 4
    f section..gnu.hash 104 0x00000298
    f section_end..gnu.hash 0 0x00000300
    CC [03] va=0x00000298 pa=0x00000298 sz=104 vsz=104 rwx=-r-- .gnu.hash @ 0x00000298
    S 0x00000300 0x00000300 0x00000c18 0x00000c18 .dynsym 4
    f section..dynsym 3096 0x00000300
    f section_end..dynsym 0 0x00000f18
    CC [04] va=0x00000300 pa=0x00000300 sz=3096 vsz=3096 rwx=-r-- .dynsym @ 0x00000300
    S 0x00000f18 0x00000f18 0x00000593 0x00000593 .dynstr 4
    f section..dynstr 1427 0x00000f18
    f section_end..dynstr 0 0x000014ab
    CC [05] va=0x00000f18 pa=0x00000f18 sz=1427 vsz=1427 rwx=-r-- .dynstr @ 0x00000f18
    S 0x000014ac 0x000014ac 0x00000102 0x00000102 .gnu.version 4
    f section..gnu.version 258 0x000014ac
    f section_end..gnu.version 0 0x000015ae
    (...)
    
