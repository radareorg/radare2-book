## Идентификация свойств файла

Идентификация типа файла осуществляется с помощью `-I`. С помощью этого параметра rabin2 выводит информацию о двоичном типе, такую как кодировка, порядок байтов, класс, операционная система:
```
$ rabin2 -I /bin/ls
arch     x86
binsz    128456
bintype  elf
bits     64
canary   true
class    ELF64
crypto   false
endian   little
havecode true
intrp    /lib64/ld-linux-x86-64.so.2
lang     c
linenum  false
lsyms    false
machine  AMD x86-64 architecture
maxopsz  16
minopsz  1
nx       true
os       linux
pcalign  0
pic      true
relocs   false
relro    partial
rpath    NONE
static   false
stripped true
subsys   linux
va       true
```

Чтобы заставить rabin2 выдавать информацию в формате, понятном основная программе radare2, добавте флаг `-Ir`:
```
$ rabin2 -Ir /bin/ls
e cfg.bigendian=false
e asm.bits=64
e asm.dwarf=true
e bin.lang=c
e file.type=elf
e asm.os=linux
e asm.arch=x86
e asm.pcalign=0
```

