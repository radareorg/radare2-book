#Chapter 5: Searching bytes

The search engine of radare is based on the work done by esteve plus multiple features on top of it that allows multiple keyword searching with binary masks and automatic flagging of results.

This powerful command is `/`.

    [0x00000000]> /
    Usage: /[amx/] [arg]
    / foo\x00       search for string `foo\0`
    /w foo          search for wide string `f\0o\0o\0`
    /wi foo         search for wide string ignoring case `f\0o\0o\0`
    /! ff           search for first occurrence not matching
    /i foo          search for string `foo` ignoring case
    /e /E.F/i       match regular expression
    /x ff0033       search for hex string
    /x ff..33       search for hex string ignoring some nibbles
    /x ff43 ffd0    search for hexpair with mask
    /d 101112       search for a deltified sequence of bytes
    /!x 00          inverse hexa search (find first byte != 0x00)
    /c jmp [esp]    search for asm code (see search.asmstr)
    /a jmp eax      assemble opcode and search its bytes
    /A              search for AES expanded keys
    /r sym.printf   analyze opcode reference an offset
    /R              search for ROP gadgets
    /P              show offset of previous instruction
    /m magicfile    search for matching magic file (use blocksize)
    /p patternsize  search for pattern of given size
    /z min max      search for strings of given size
    /v[?248] num    look for a asm.bigendian 32bit value
    //              repeat last search
    /b              search backwards


With radare everything is handled as a file, it doesn`t matters if it is a socket, a remote device, the process memory, etc..



## 5.1 Basic searchs

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


## 5.2 Configurating the searchs

The search engine can be configured by the `e` interface:

    Configuration:
    e cmd.hit = x         ; command to execute on every search hit
    e search.distance = 0 ; search string distance
    e search.in = [foo]   ; boundaries to raw, block, file, section)
    e search.align = 4    ; only catch aligned search hits
    e search.from = 0     ; start address
    e search.to = 0       ; end address
    e search.asmstr = 0   ; search string instead of assembly
    e search.flags = true ; if enabled store flags on keyword hits

`search.align` variable is used to determine that the only `valid` search hits must have to fit in this alignement. For example. you can use `e search.align=4` to get only the hits found in 4-byte aligned addresses.

The `search.flag` boolean variable makes the engine setup flags when finding hits. If the search is stopped by the user with a ^C then a `search_stop` flag will be added.

## 5.3 Pattern search

The search command allows you to throw repeated pattern searchs against the IO backend to be able to identify repeated sequences of bytes without specifying them. The only property to perform this search is to manually define the minimum length of these patterns.

Here`s an example:

    [0x00000000]> /p 10
The output of the command will show the different patterns found and how many times they are repeated.

## 5.4 Automatization

The cmd.hit eval variable is used to define a command that will be executed when a hit is reached by the search engine. If you want to run more than one command use `;` or `. script-file-name` for including a file as a script.

For example:

    [0x00404888]> e cmd.hit = p8 8
    [0x00404888]> / lib
    Searching 3 bytes from 0x00400000 to 0x0041ae08: 6c 69 62 
    hits: 9
    0x00400239 hit4_0 "lib64/ld-linux-x86-64.so.2"
    31ed4989d15e4889
    0x00400f19 hit4_1 "libselinux.so.1"
    31ed4989d15e4889
    0x00400fae hit4_2 "librt.so.1"
    31ed4989d15e4889
    0x00400fc7 hit4_3 "libacl.so.1"
    31ed4989d15e4889
    0x00401004 hit4_4 "libc.so.6"
    31ed4989d15e4889
    0x004013ce hit4_5 "libc_start_main"
    31ed4989d15e4889
    0x00416542 hit4_6 "libs/"
    31ed4989d15e4889
    0x00417160 hit4_7 "lib/xstrtol.c"
    31ed4989d15e4889
    0x00417578 hit4_8 "lib"
    31ed4989d15e4889


## 5.5 Backward search

To search backward just use `\b`


## 5.6 Search in assembly

If you want to search for a certain type of opcodes you can either use `/c` or `/a`:

    /c jmp [esp]    search for asm code
    
    
    [0x00404888]> /c jmp qword [rdx]
    f hit_0 @ 0x0040e50d   # 2: jmp qword [rdx]
    f hit_1 @ 0x00418dbb   # 2: jmp qword [rdx]
    f hit_2 @ 0x00418fcb   # 3: jmp qword [rdx]
    f hit_3 @ 0x004196ab   # 6: jmp qword [rdx]
    f hit_4 @ 0x00419bf3   # 3: jmp qword [rdx]
    f hit_5 @ 0x00419c1b   # 3: jmp qword [rdx]
    f hit_6 @ 0x00419c43   # 3: jmp qword [rdx]
    
    
    /a jmp eax      assemble opcode and search its bytes
    
    
    [0x00404888]> /a jmp eax
    hits: 1
    0x004048e7 hit3_0 ffe00f1f8000000000b8



    
## 5.7 Searching AES keys
 
Thanks to Victor Mu<here be dragonz>oz i have added support to the algorithm he developed to find expanded AES keys. It runs the search from the current seek to the cfg.limit or the end of the file. You can always stop the search pressing `^C`.

    $ sudo r2 /dev/mem
    [0x00000000]> /A
    0 AES keys found
