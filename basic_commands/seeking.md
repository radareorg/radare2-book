##3.1 Seeking

The command `s` is used to seek. It accepts a math expression as argument that can be composed by shift operations, basic math ones and memory access.


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


To 'query' the math expression you can evaluate them using the `?` command and giving the math operation as argument. And getting the result in hexa, decimal, octal and binary.

    > ? 0x100+200
    0x1C8 ; 456d ; 710o ; 1100 1000  


In visual mode just press `u` or `U` to undo or redo inside the seek history.