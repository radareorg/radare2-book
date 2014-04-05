#Chapter 3: Basic commands

The basic set of commands in radare can be mostly grouped by action, and they should be easy to remember and short. This is why they are grouped with a single character, subcommands or related commands are described with a second character. For example '/ foo' for searching plain strings or '/x 90 90' to look for hexpair strings.

The format of the commands (as explained in 'Command format' chapter) looks something like that:

    [[.][times][cmd][~grep][@[@iter]addr!size][|>pipe] ; ...

    > 3s +1024    ; seeks three times 1024 from the current seek
    
If the command starts with '!' the string is passed to the plugin hadling the current IO (the debugger for example), if no one handles it calls to posix_system() which is a shell escape, you can prefix the command with two '!!'.

    > !help       ; handled by the debugger or shell
    > !!ls        ; runs ls in the shell
The [arg] argument depends on the command, but most of them take a number as argument to specify the number of bytes to work on instead of block size. Other commands accept math expressions, or strings.

    > px 0x17     ; show 0x17 bytes in hexa at current seek
    > s base+0x33 ; seeks to flag 'base' plus 0x33
    > / lib       ; search for 'lib' string.
The '@' is used to specify a temporal seek where the command is executed. This is quite useful to not seeking all the time.

    > p8 10 @ 0x4010  ; show 10 bytes at offset 0x4010
    > f patata @ 0x10 ; set 'patata' flag at offset 0x10
Using '@@' you can execute a single command on a list of flags matching the glob:

    > s 0
    > / lib             ; search 'lib' string
    > p8 20 @@ hit0_*   ; show 20 hexpairs at each search hit
    
The '>' is used to pipe the output of the command to a file (truncating to 0 if exist)

    > pr > dump.bin   ; dump 'raw' bytes of current block to 'dump.bin' file
    > f  > flags.txt  ; dump flag list to 'flags.txt'
    
The '|' pipe is used to dump the output of the command to another program.

    [0x4A13B8C0]> f | grep section | grep text
    0x0805f3b0 512 section._text
    0x080d24b0 512 section._text_end
    
Using the ';' chars you can concatenate multiple commands in a single line:

    > px ; dr


##3.1 Seeking

The command 's' is used to seek. It accepts a math expression as argument that can be composed by shift operations, basic math ones and memory access.


    [0x00000000]> s?
    Usage: s[+-] [addr]
    s                 print current address
    s 0x320           seek to this address
    s-                undo seek
    s+                redo seek
    s*                list undo seek history
    s++               seek blocksize bytes forward
    s--               seek blocksize bytes backward
    s+ 512            seek 512 bytes forward
    s- 512            seek 512 bytes backward
    sg/sG             seek begin (sg) or end (sG) of section or file
    s.hexoff          Seek honoring a base from core->offset
    sa [[+-]a] [asz]  seek asz (or bsize) aligned to addr
    sn/sp             seek next/prev scr.nkey
    s/ DATA           search for next occurrence of 'DATA'
    s/x 9091          search for next occurrence of \x90\x91
    sb                seek aligned to bb start
    so [num]          seek to N next opcode(s)
    sf                seek to next function (f->addr+f->size)
    sC str            seek to comment matching given string
    sr pc             seek to register

    > 3s++        ; 3 times block-seeking
    > s 10+0x80   ; seek at 0x80+10


To 'query' the math expression you can evaluate them using the '?' command and giving the math operation as argument. And getting the result in hexa, decimal, octal and binary.

    > ? 0x100+200
    0x1C8 ; 456d ; 710o ; 1100 1000  


In visual mode just press 'u' or 'U' to undo or redo inside the seek history.


##3.2 Block size

The block size is the default view size for radare. All commands will work with this constraint, but you can always temporally change the block size just giving a numeric argument to the print commands for example (px 20)

    [0xB7F9D810]> b?
    Usage: b[f] [arg]
    b         display current block size
    b+3       increase blocksize by 3
    b-16      decrement blocksize by 3
    b 33      set block size to 33
    b eip+4   numeric argument can be an expression
    bf foo    set block size to flag size
    bm 1M     set max block size

The 'b' command is used to change the block size:

    [0x00000000]> b 0x100   ; block size = 0x100
    [0x00000000]> b +16     ;  ... = 0x110
    [0x00000000]> b -32     ;  ... = 0xf0
    
The 'bf' command is used to change the block size to the one specified by a flag. For example in symbols, the block size of the flag represents the size of the function.

    [0x00000000]> bf sym.main    ; block size = sizeof(sym.main)
    [0x00000000]> pd @ sym.main  ; disassemble sym.main
    ...

You can perform these two operations in a single one (pdf):

     [0x00000000]> pdf @ sym.main