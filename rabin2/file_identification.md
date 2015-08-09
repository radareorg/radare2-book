## File Properties Identification

File type identification is done using `-I`. With this option, rabin2 prints information on a binary's type, its encoding, endianness, class, operating system, etc.:

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
    
To make rabin2 output information in format that the main program, radare2, can understand, pass `-Ir` option to it:

    $ rabin2 -Ir /bin/ls
    e file.type=elf
    e cfg.bigendian=false
    e asm.os=linux
    e asm.arch=x86
    e anal.arch=x86
    e asm.bits=64
    e asm.dwarf=true
