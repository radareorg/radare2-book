## Strings

The `-z` option is used to list readable strings found in the .rodata section of ELF binaries, or the .text section of PE files. Example:

```
$ rabin2 -z /bin/ls | head
000 0x000160f8 0x000160f8  11  12 (.rodata) ascii dev_ino_pop
001 0x00016188 0x00016188  10  11 (.rodata) ascii sort_files
002 0x00016193 0x00016193   6   7 (.rodata) ascii posix-
003 0x0001619a 0x0001619a   4   5 (.rodata) ascii main
004 0x00016250 0x00016250  10  11 (.rodata) ascii ?pcdb-lswd
005 0x00016260 0x00016260  65  66 (.rodata) ascii # Configuration file for dircolors, a utility to help you set the
006 0x000162a2 0x000162a2  72  73 (.rodata) ascii # LS_COLORS environment variable used by GNU ls with the --color option.
007 0x000162eb 0x000162eb  56  57 (.rodata) ascii # Copyright (C) 1996-2018 Free Software Foundation, Inc.
008 0x00016324 0x00016324  70  71 (.rodata) ascii # Copying and distribution of this file, with or without modification,
009 0x0001636b 0x0001636b  76  77 (.rodata) ascii # are permitted provided the copyright notice and this notice are preserved.
```

With the `-zr` option, this information is represented as a radare2 commands list. It can be used in a radare2 session to automatically create a flag space called "strings" pre-populated with flags for all strings found by rabin2.
Furthermore, this script will mark corresponding byte ranges as strings instead of code.
```
$ rabin2 -zr /bin/ls | head
fs stringsf str.dev_ino_pop 12 @ 0x000160f8
Cs 12 @ 0x000160f8
f str.sort_files 11 @ 0x00016188
Cs 11 @ 0x00016188
f str.posix 7 @ 0x00016193
Cs 7 @ 0x00016193
f str.main 5 @ 0x0001619a
Cs 5 @ 0x0001619a
f str.pcdb_lswd 11 @ 0x00016250
Cs 11 @ 0x00016250
```

