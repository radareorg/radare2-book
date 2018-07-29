## Sections

Firmware images, bootloaders and binary files usually place various sections of a binary at different addresses in memory. To represent this behavior, radare offers the `S` command.

Here's the help message:
```
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
```
You can specify a section in a single line:
```
# Add new section
S [off] [va] [sz] [vsz] [name] [mrwx]
```
For example:
```
[0x00404888]> S 0x00000100 0x00400000 0x0001ae08 0001ae08 test rwx
```
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

It is possible to remove a section definition using the `S-` command. Pass the section id to it as an argument:
```
[0xB7EE8810]> S-4
```
