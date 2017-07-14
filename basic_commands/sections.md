## Sections

Firmware images, bootloaders and binary files usually place various sections of a binary at different addresses in memory. To represent this behavior, radare offers the `S` command.

Here's the help message:

    [0xB7EE8810]> S?
    |Usage: S[?-.*=adlr] [...]
    | S                          list sections
    | S.                         show current section name
    | S*                         list sections (in radare commands)
    | S=                         list sections (ascii-art bars) (io.va to display paddr or vaddr)
    | Sa[-] [A] [B] [[off]]      Specify arch and bits for given section
    | Sd[a] [file]               dump current (all) section to a file (see dmd)
    | Sl [file]                  load contents of file into current section (see dml)
    | Sf [baddr]                 Alias for S 0 0 $s $s foo mrwx
    | Sj                         list sections in JSON (alias for iSj)
    | Sr [name]                  rename section on current seek
    | S off va sz vsz name mrwx  add new section (if(!vsz)vsz=sz)
    | S-[id]                     remove section identified by id
    | S-.                        remove section at core->offset (can be changed with @)
    | S.-*                       remove all sections in current offset

You can specify a section in a single line:

    # Add new section
    S [off] [va] [sz] [vsz] [name] [mrwx]

For example:

    [0x00404888]> S 0x00000100 0x00400000 0x0001ae08 0001ae08 test rwx

Displaying information about sections:

    # List sections
    [0x00404888]> S

    [00] . 0x00000238 -r-- va=0x00400238 sz=0x0000001c vsz=0000001c .interp
    [01] . 0x00000254 -r-- va=0x00400254 sz=0x00000020 vsz=00000020 .note.ABI_tag
    [02] . 0x00000274 -r-- va=0x00400274 sz=0x00000024 vsz=00000024 .note.gnu.build_id
    [03] . 0x00000298 -r-- va=0x00400298 sz=0x00000068 vsz=00000068 .gnu.hash
    [04] . 0x00000300 -r-- va=0x00400300 sz=0x00000c18 vsz=00000c18 .dynsym
    ...


    # List sections (in nice ascii-art bars)
    [0xB7EEA810]> S=

    ...
    25  0x0001a600 |-----------------------------#| 0x0001a608 ---- .gnu_debuglink
    26  0x0001a608 |-----------------------------#| 0x0001a706 ---- .shstrtab
    27* 0x00000000 |##############################| 0x0001ae08 -rwx ehdr
    =>  0x00004888 |-----^------------------------| 0x00004988


The first three lines are sections and the last one (prefixed by `=>`) is the current seek location.

Remove a section definition using the `S-` command. Pass the section id to it as an argument.

    [0xB7EE8810]> S-4
