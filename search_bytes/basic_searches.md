## Basic searches

A basic search for a plain string in a whole file would be something like:

    $ r2 -c "/ lib" -q /bin/ls
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

`r2 -q` // quiet mode (no prompt) and quit after -i

As you can see, radare generates a `hit` flag for each search result found. You you can just use the `ps` command to visualize the strings at these offsets in this way:

    [0x00404888]> / ls
    ...
    [0x00404888]> ps @ hit0_0
    lseek
    
We can also search wide-char strings (the ones containing zeros between each letter) using the `/w` in this way:

    [0x00000000]> /w Hello
    0 results found.

Search for strings ignoring case

    [0x0040488f]> /i Stallman
    Searching 8 bytes from 0x00400238 to 0x0040488f: 53 74 61 6c 6c 6d 61 6e 
    [# ]hits: 004138 < 0x0040488f  hits = 0   
   
It is also possible to mix hexadecimal scape sequences in the search string:

    [0x00000000]> / \x7FELF
    
But if you want to perform an hexadecimal search you will probably prefer an hexpair input with `/x`:

    [0x00000000]> /x 7F454C46
    
Once the search is done, the results are stored in the `search` flag space.

    [0x00000000]> f
    0x00000135 512 hit0_0
    0x00000b71 512 hit0_1
    0x00000bad 512 hit0_2
    0x00000bdd 512 hit0_3
    0x00000bfb 512 hit0_4
    0x00000f2a 512 hit0_5
    
To remove these flags, you can just use the `f@-hit*` command.

Sometimes while working long time in the same file you will need to launch the last search more than once and you will probably prefer to use the `//` command instead of typing all the string again.

    [0x00000f2a]> //     ; repeat last search
