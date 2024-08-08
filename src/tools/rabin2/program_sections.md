### Sections

Rabin2 called with the `-S` option gives complete information about the sections of an executable. For each section the index, offset, size, alignment, type and permissions, are shown. The next example demonstrates this:

```console
$ rabin2 -S /bin/ls
[Sections]

nth paddr          size vaddr         vsize perm name
-----------------------------------------------------
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
11 0x000032d0  1648 0x000032d0  1648 -r-x .plt
12 0x00003940    24 0x00003940    24 -r-x .plt.got
13 0x00003960 73931 0x00003960 73931 -r-x .text
14 0x00015a2c     9 0x00015a2c     9 -r-x .fini
15 0x00015a40 20201 0x00015a40 20201 -r-- .rodata
16 0x0001a92c  2164 0x0001a92c  2164 -r-- .eh_frame_hdr
17 0x0001b1a0 11384 0x0001b1a0 11384 -r-- .eh_frame
18 0x0001e390     8 0x0021e390     8 -rw- .init_array
19 0x0001e398     8 0x0021e398     8 -rw- .fini_array
20 0x0001e3a0  2616 0x0021e3a0  2616 -rw- .data.rel.ro
21 0x0001edd8   480 0x0021edd8   480 -rw- .dynamic
22 0x0001efb8    56 0x0021efb8    56 -rw- .got
23 0x0001f000   840 0x0021f000   840 -rw- .got.plt
24 0x0001f360   616 0x0021f360   616 -rw- .data
25 0x0001f5c8     0 0x0021f5e0  4824 -rw- .bss
26 0x0001f5c8   232 0x00000000   232 ---- .shstrtab
```

With the `-Sr` option, rabin2 will flag the start/end of every section, and will pass the rest of information as a comment.

```console
$ rabin2 -Sr /bin/ls | head
fs sections
"f section. 1 0x00000000"
"f section..interp 1 0x000002a8"
"f section..note.gnu.build_id 1 0x000002c4"
"f section..note.ABI_tag 1 0x000002e8"
"f section..gnu.hash 1 0x00000308"
"f section..dynsym 1 0x000003b8"
"f section..dynstr 1 0x00000fb8"
"f section..gnu.version 1 0x00001574"
"f section..gnu.version_r 1 0x00001678"
```
