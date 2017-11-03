## Mapping Files

Radare IO system allows you to map contents of files into the same IO space used to contain loaded binary. New contents can be placed at random offsets. This lets you create a static environment which emulate
the view you would have when using a debugger, where the program and all its libraries are loaded in memory and can be accessed.

Using the `S` (sections) command you can define base address for each library to be loaded.

Mapping files is done using the `o` (open) command. Let's read the help:

    [0x00000000]> o?
    |Usage: o [com- ] [file] ([offset])
    | o                    list opened files
    | oq                   list all open files
    | o*                   list opened files in r2 commands
    | o=                   list opened files (ascii-art bars)
    | ob[?] [lbdos] [...]  list opened binary files backed by fd
    | oc [file]            open core file, like relaunching r2
    | oi[-|idx]            alias for o, but using index instead of fd
    | oj[?]                list opened files in JSON format
    | oL                   list all IO plugins registered
    | om[?]                create, list, remove IO maps
    | on [file] 0x4000     map raw file at 0x4000 (no r_bin involved)
    | oo[?]                reopen current file (kill+fork in debugger)
    | oo+                  reopen current file in read-write
    | ood [args]           reopen in debugger mode (with args)
    | oo[bnm] [...]        see oo? for help
    | op [fd]              priorize given fd (see also ob)
    | o 4                  Switch to open file on fd 4
    | o-1                  close file descriptor 1
    | o-*                  close all opened files
    | o--                  close all files, analysis, binfiles, flags, same as !r2 --
    | o [file]             open [file] file in read-only
    | o+ [file]            open file in read-write mode
    | o [file] 0x4000      map file at 0x4000
    | ox fd fdx            exchange the descs of fd and fdx and keep the mapping

Prepare a simple layout:

    $ rabin2 -l /bin/ls
    [Linked libraries]
    libselinux.so.1
    librt.so.1
    libacl.so.1
    libc.so.6

    4 libraries

Map a file:

    [0x00001190]> o /bin/zsh 0x499999

List mapped files:

    [0x00000000]> o
    - 6 /bin/ls @ 0x0 ; r
    - 10 /lib/ld-linux.so.2 @ 0x100000000 ; r
    - 14 /bin/zsh @ 0x499999 ; r


Print hexadecimal values from /bin/zsh:

    [0x00000000]> px @ 0x499999


Unmap files using the `o-` command. Pass required file descriptor to it as an argument:

    [0x00000000]> o-14
