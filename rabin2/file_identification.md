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