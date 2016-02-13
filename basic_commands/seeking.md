## Seeking

Current seek position is changed with `s` command. It accepts a math expression as argument. The expression can be composed of shift operations, basic math operations, or memory access operations.


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


If you want to inspect the result of a math expression, you can evaluate it using the `?` command. Simply pass the expression as an argument. The result can be displayed in hexadecimal, decimal, octal or binary formats.

    > ? 0x100+200
    0x1C8 ; 456d ; 710o ; 1100 1000  


In the visual mode you can press `u` (undo) or `U` (redo) inside the seek history to return back to previous or forward to the next location.

## Open test file

As test file lets use some simple hello_world.c compiled in Linux ELF format.
After we compiled it lets open it with radare2

    r2 ./hello_world

Now we have command prompt

    [0x00400410]>

Now we are ready to go deeper. 

## Seeking at any position

Seek to address 0x0

    [0x00400410]> s 0x0
    [0x00000000]>

Print current address 

    [0x00000000]> s
    0x0
    [0x00000000]>

Seek N positions forward

    [0x00000000]> s+ 128
    [0x00000080]>

Undo/Redo seek. Lets see with positions we where seeking

    [0x00400410] [0x00000000] [0x00000080]

Lets undo last two seeks to beginning

    [0x00000080]> s-
    [0x00000000]> s-
    [0x00400410]>

Now we are at same position as at beginning _0x00400410_.

Lets search in hellow_world ELF file 'Hello'. After search our position will set
to searched string position. But we always can go back with `s-`.

    [0x00400410]> s/ Hello
    Searching 5 bytes from 0x00400411 to 0x00600928: 48 65 6c 6c 6f 
    Searching 5 bytes in [0x400411-0x600928]
    hits: 1  hit0_0 .. hit0_0
    0x004005b4 hit0_0 "Hello"
    [0x004005b4]>s-
    [0x00400410]>
