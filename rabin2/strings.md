## Strings

The `-z` option is used to list readable strings found in the .rodata section of ELF binaries, or the .text section of PE files. Example:

    $ rabin2 -z /bin/ls |head
    addr=0x00012487 off=0x00012487 ordinal=000 sz=9 len=9 section=.rodata type=A string=src/ls.c
    addr=0x00012490 off=0x00012490 ordinal=001 sz=26 len=26 section=.rodata type=A string=sort_type != sort_version
    addr=0x000124aa off=0x000124aa ordinal=002 sz=5 len=5 section=.rodata type=A string= %lu
    addr=0x000124b0 off=0x000124b0 ordinal=003 sz=7 len=14 section=.rodata type=W string=%*lu ?
    addr=0x000124ba off=0x000124ba ordinal=004 sz=8 len=8 section=.rodata type=A string=%s %*s
    addr=0x000124c5 off=0x000124c5 ordinal=005 sz=10 len=10 section=.rodata type=A string=%*s, %*s
    addr=0x000124cf off=0x000124cf ordinal=006 sz=5 len=5 section=.rodata type=A string= ->
    addr=0x000124d4 off=0x000124d4 ordinal=007 sz=17 len=17 section=.rodata type=A string=cannot access %s
    addr=0x000124e5 off=0x000124e5 ordinal=008 sz=29 len=29 section=.rodata type=A string=cannot read symbolic link %s
    addr=0x00012502 off=0x00012502 ordinal=009 sz=10 len=10 section=.rodata type=A string=unlabeled


With the `-zr` option, this information is represented as a radare2 commands list. It can be used in a radare2 session to automatically create a flag space called "strings" pre-populated with flags for all strings found by rabin2.
Furthermore, this script will mark corresponding byte ranges as strings instead of code.

    $ rabin2 -zr /bin/ls |head
    fs strings
    f str.src_ls.c 9 @ 0x00012487
    Cs 9 @ 0x00012487
    f str.sort_type__sort_version 26 @ 0x00012490
    Cs 26 @ 0x00012490
    f str._lu 5 @ 0x000124aa
    Cs 5 @ 0x000124aa
    f str.__lu_ 14 @ 0x000124b0
    Cs 7 @ 0x000124b0
    f str._s__s 8 @ 0x000124ba
    (...)
