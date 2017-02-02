## Program Sections

Rabin2 called with the `-S` option gives complete information about the sections of an executable. For each section the index, offset, size, alignment, type and permissions, are shown. The next example demonstrates this:

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

With the `-Sr` option, rabin2 will flag the start/end of every section, and will pass the rest of information as a comment.

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
