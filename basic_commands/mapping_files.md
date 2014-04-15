## Mapping files

Radare IO allows to virtually map contents of files in the same IO space at random offsets. This is useful to open multiple files in a single view or just to 'emulate' an static environment like if it was in the debugger with the program and all its libraries mapped there.

Using the `S`ections command you'll be able to define different base address for each library loaded at different offsets.

Mapping files is done with the `o` (open) command. Let's read the help:

    [0x00000000]> o?
    Usage: o[com- ] [file] ([offset])
    o                  list opened files
    oc [file]          open core file, like relaunching r2
    oo                 reopen current file (kill+fork in debugger)
    oo+                reopen current file in read-write
    o 4                priorize io on fd 4 (bring to front)
    o-1                close file index 1
    o /bin/ls          open /bin/ls file in read-only
    o+/bin/ls          open /bin/ls file in read-write mode
    o /bin/ls 0x4000   map file at 0x4000
    on /bin/ls 0x4000  map raw file at 0x4000 (no r_bin involved)
    om[?]              create, list, remove IO maps

Let's prepare a simple layout:

    $ rabin2 -l /bin/ls
        [Linked libraries]
        libselinux.so.1
        librt.so.1
        libacl.so.1
        libc.so.6
        
        4 libraries

Map a file:

    [0x00001190]> o /bin/zsh 0x499999

Listing mapped files:

    [0x00000000]> o
    - 6 /bin/ls @ 0x0 ; r
    - 10 /lib/ld-linux.so.2 @ 0x100000000 ; r
    - 14 /bin/zsh @ 0x499999 ; r


Print some hexadecimal from /bin/zsh

    [0x00000000]> px @ 0x499999


To unmap these files just use the `o-` command giving the file descriptor as argument:

    [0x00000000]> o-14
