## Mapping Files

Radare IO system allows to map contents of files into the same IO space used to contain loaded binary. New contents can be placed at random offsets. This is useful to open multiple files in a single view or to 'emulate' a static environment similar to what you would have using a debugger where the program and all its libraries are loaded in memory and can be accessed.

Using the `S` (sections) command you can define base address for each library to be loaded.

Mapping files is done using the `o` (open) command. Let's read the help:

    [0x00000000]> o?
    Usage: o[com- ] [file] ([offset])
    o                  list opened files
    oc [file]          open core file, like relaunching r2
    oo                 reopen current file (kill+fork in debugger)
    oo+                reopen current file in read-write
    o 4                prioritize io on fd 4 (bring to front)
    o-1                close file index 1
    o /bin/ls          open /bin/ls file in read-only
    o+/bin/ls          open /bin/ls file in read-write mode
    o /bin/ls 0x4000   map file at 0x4000
    on /bin/ls 0x4000  map raw file at 0x4000 (no r_bin involved)
    om[?]              create, list, remove IO maps

To prepare a simple layout:

    $ rabin2 -l /bin/ls
        [Linked libraries]
        libselinux.so.1
        librt.so.1
        libacl.so.1
        libc.so.6
        
        4 libraries

To map a file:

    [0x00001190]> o /bin/zsh 0x499999

To list mapped files:

    [0x00000000]> o
    - 6 /bin/ls @ 0x0 ; r
    - 10 /lib/ld-linux.so.2 @ 0x100000000 ; r
    - 14 /bin/zsh @ 0x499999 ; r


To print hexadecimal values from /bin/zsh:

    [0x00000000]> px @ 0x499999


To unmap files use `o-` command. Pass required file descriptor to it as an argument:

    [0x00000000]> o-14
