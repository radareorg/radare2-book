## Basic Search 

A basic search for a plain text string in a file would be something like:

    $ r2 -q -c "/ lib" /bin/ls
    Searching 3 bytes from 0x00400000 to 0x0041ae08: 6c 69 62 
    hits: 9
    0x00400239 hit0_0 "lib64/ld-linux-x86-64.so.2"
    0x00400f19 hit0_1 "libselinux.so.1"
    0x00400fae hit0_2 "librt.so.1"
    0x00400fc7 hit0_3 "libacl.so.1"
    0x00401004 hit0_4 "libc.so.6"
    0x004013ce hit0_5 "libc_start_main"
    0x00416542 hit0_6 "libs/"
    0x00417160 hit0_7 "lib/xstrtol.c"
    0x00417578 hit0_8 "lib"

As can be seen from the output above, radare2 generates a "hit" flag for every entry found. You can then use the `ps` command to see the strings stored at the offsets marked by the flags in this group, and they will have names of the form `hit0_<index>`:

    [0x00404888]> / ls
    ...
    [0x00404888]> ps @ hit0_0
    lseek

You can search for wide-char strings (e.g., unicode letters) using the `/w` command:

    [0x00000000]> /w Hello
    0 results found.

To perform a case-insensitive search for strings use `/i`:

    [0x0040488f]> /i Stallman
    Searching 8 bytes from 0x00400238 to 0x0040488f: 53 74 61 6c 6c 6d 61 6e
    [# ]hits: 004138 < 0x0040488f  hits = 0

It is possible to specify hexadecimal escape sequences in the search string by prepending them with "\x":

    [0x00000000]> / \x7FELF

if, instead, you are searching for a string of hexadecimal values, you're probably better of using the `/x` command:

    [0x00000000]> /x 7F454C46

Once the search is done, the results are stored in the `searches` flag space.

    [0x00000000]> fs
    0    0 . strings
    1    0 . symbols
    2    6 . searches

    [0x00000000]> f
    0x00000135 512 hit0_0
    0x00000b71 512 hit0_1
    0x00000bad 512 hit0_2
    0x00000bdd 512 hit0_3
    0x00000bfb 512 hit0_4
    0x00000f2a 512 hit0_5

To remove "hit" flags after you do not need them anymore, use the `f- hit*` command.

Often, during long search sessions, you will need to launch the latest search more than once. You can use the `//` command to repeat the last search.

    [0x00000f2a]> //     ; repeat last search
