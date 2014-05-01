## Comparing bytes

You can compare data using the `c` command. It accepts an input in various formats and compares the input against the bytes in the current seek.

    [0x00404888]> c?
    |Usage: c[?dfx] [argument]
    | c  [string]    Compares a plain with escaped chars string
    | cc [at] [(at)] Compares in two hexdump columns of block size
    | c4 [value]     Compare a doubleword from a math expression
    | c8 [value]     Compare a quadword from a math expression
    | cx [hexpair]   Compare hexpair string
    | cX [addr]      Like 'cc' but using hexdiff output
    | cf [file]      Compare contents of file at current seek
    | cg[o] [file]   Graphdiff current file and [file]
    | cu [addr] @at  Compare memory hexdumps of $$ and dst in unified diff
    | cw[us?] [...]  Compare memory watchers
    | cat  [file]    Show contents of file (see pwd, ls)
    | cl|cls|clear   Clear screen, (clear0 to goto 0, 0 only)
    
An example of memory comparision:

    [0x08048000]> p8 4
    7f 45 4c 46 
    
    [0x08048000]> cx 7f 45 90 46
    Compare 3/4 equal bytes
    0x00000002 (byte=03)   90 ' '  ->  4c 'L'
    [0x08048000]> 

Another subcommand of `c` (compare) command is `cc` which stands for 'compare code'.

    [0x4A13B8C0]> cc 0x39e8e089 @ 0x4A13B8C0
    
    [0x08049A80]> cc sym.main2 @ sym.main

`c8` compares a quadword from the current seek (0x00000000) from a math expression

    [0x00000000]> c8 4
    
    Compare 1/8 equal bytes (0%)
    0x00000000 (byte=01)   7f ' '  ->  04 ' '
    0x00000001 (byte=02)   45 'E'  ->  00 ' '
    0x00000002 (byte=03)   4c 'L'  ->  00 ' '


The number paramater can of course also be a math expressions using flag names and so on:

    [0x00000000]> cx 7f469046
    
    Compare 2/4 equal bytes
    0x00000001 (byte=02)   45 'E'  ->  46 'F'
    0x00000002 (byte=03)   4c 'L'  ->  90 ' '



We can use the compare command to compare the current block to a file previously dumped to disk.

    r2 /bin/true
    [0x08049A80]> s 0
    [0x08048000]> cf /bin/true
    Compare 512/512 equal bytes
