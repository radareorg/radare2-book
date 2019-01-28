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

Before the IO refactoring done by condret, the `S` command was used to manage what we now call maps. Currently the `S` command is deprecated because `iS` and `om` should be enough.

Firmware images, bootloaders and binary files usually place various sections of a binary at different addresses in memory. To represent this behavior, radare offers the `iS`. Use `iS?` to get the help message. To list all created sections use `iS` (or `iSj` to get the json format). The `iS=` will show the region bars in ascii-art.

You can also use `om` command to view information about mapped sections:

```
[0x00401000]> om
 6 fd: 3 +0x00000600 0x00403000 - 0x004031ff -rwx fmap..what
 5 fd: 6 +0x00000000 0x00403200 - 0x00403fff -rwx mem..what
 4 fd: 3 +0x00000200 0x00401000 - 0x004011ff -rwx fmap..text
 3 fd: 5 +0x00000000 0x00401200 - 0x00401fff mrwx mmap..text
 2 fd: 3 +0x00000400 0x00402000 - 0x004021ff -rw- fmap..rdata
 1 fd: 4 +0x00000000 0x00402200 - 0x00402fff mrw- mmap..rdata
 ```
Use `om?` to get all the possible subcommands. To list all the defined maps use `om` (or `omj` to get the json format or `om*` to get the r2 commands format). To get the ascii art view use `om=`. 

It is also possible to delete the mapped section using the `om-mapid` command.
