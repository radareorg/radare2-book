## Sections

The concept of sections is tied to the information extracted from the binary. We can display this information by using the `i` command.

Displaying information about sections:

```
[0x00005310]> iS
[Sections]
00 0x00000000     0 0x00000000     0 ----
01 0x00000238    28 0x00000238    28 -r-- .interp
02 0x00000254    32 0x00000254    32 -r-- .note.ABI_tag
03 0x00000278   176 0x00000278   176 -r-- .gnu.hash
04 0x00000328  3000 0x00000328  3000 -r-- .dynsym
05 0x00000ee0  1412 0x00000ee0  1412 -r-- .dynstr
06 0x00001464   250 0x00001464   250 -r-- .gnu.version
07 0x00001560   112 0x00001560   112 -r-- .gnu.version_r
08 0x000015d0  4944 0x000015d0  4944 -r-- .rela.dyn
09 0x00002920  2448 0x00002920  2448 -r-- .rela.plt
10 0x000032b0    23 0x000032b0    23 -r-x .init
...
```

As you may know, binaries have sections and maps. The sections define the contents of a portion of the file that can be mapped in memory (or not). What is mapped is defined by the segments.

Before the IO refactoring done by condret, the `S` command was used to manage what we now call maps. Currently the `S` command is supposed to be deprecated because `iS` and `om` should be enough.

Firmware images, bootloaders and binary files usually place various sections of
a binary at different addresses in memory. To represent this behavior, radare
offers the `S` command. Use `S?` to get the help message. But we would recommend
you to better check `om` and `iS` and try to avoid `S` as much as possible
since it will be deprecated soon. To list all created sections use `S` (or `Sj` to get the json format). The `S=` will show the region bars in ascii-art.

You can create a new section with a line like this:

```
S [off] [va] [sz] [vsz] [name] [mrwx]
```

For example:

```
[0x00404888]> S 0x00000100 0x00400000 0x0001ae08 0001ae08 test rwx
```

It is possible to remove a section definition using the `S-` command. Pass the section id to it as an argument:

```
[0xB7EE8810]> S-4
```
