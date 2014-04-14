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