## Write

Radare can manipulate the file in multiple ways. You can resize the file, move bytes, copy/paste them, insert mode (shifting data to the end of the block or file) or just overwrite some bytes with an address, the contents of a file, a widestring or inline assembling an opcode.

To resize. Use the `r` command which accepts a numeric argument. Possitive valule sets the new size to the file. A negative one will strip N bytes from the current seek down-sizing the file.

    r 1024      ; resize the file to 1024 bytes
    r -10 @ 33  ; strip 10 bytes at offset 33
To write bytes just use the `w` command. It accepts multiple input formats like inline assembling, endian-friendly dwords, files, hexpair files, wide strings:

    [0x00404888]> w?
    |Usage: w[x] [str] [<file] [<<EOF] [@addr]
    | w foobar     write string 'foobar'
    | wh r2        whereis/which shell command
    | wr 10        write 10 random bytes
    | ww foobar    write wide string 'f\x00o\x00o\x00b\x00a\x00r\x00'
    | wa push ebp  write opcode, separated by ';' (use '"' around the command)
    | waf file     assemble file and write bytes
    | wA r 0       alter/modify opcode at current seek (see wA?)
    | wb 010203    fill current block with cyclic hexpairs
    | wc[ir*?]     write cache undo/commit/reset/list (io.cache)
    | wx 9090      write two intel nops
    | wv eip+34    write 32-64 bit value
    | wo? hex      write in block with operation. 'wo?' fmi
    | wm f0ff      set binary mask hexpair to be used as cyclic write mask
    | ws pstring   write 1 byte for length and then the string
    | wf -|file    write contents of file at current offset
    | wF -|file    write contents of hexpairs file here
    | wp -|file    apply radare patch file. See wp? fmi
    | wt file [sz] write to file (from current seek, blocksize or sz bytes)
    
Some examples:

     [0x00000000]> wx 123456 @ 0x8048300
     [0x00000000]> wv 0x8048123 @ 0x8049100
     [0x00000000]> wa jmp 0x8048320




###3.8.1 Write over with operation

The `wo` write command accepts multiple kinds of operations that can be applied on the curren block. This is for example a XOR, ADD, SUB, ...

    [0x4A13B8C0]> wo?
    |Usage: wo[asmdxoArl24] [hexpairs] @ addr[:bsize]
    |Example:
    |  wox 0x90   ; xor cur block with 0x90
    |  wox 90     ; xor cur block with 0x90
    |  wox 0x0203 ; xor cur block with 0203
    |  woa 02 03  ; add [0203][0203][...] to curblk
    |  woe 02 03  
    |Supported operations:
    |  wow  ==  write looped value (alias for 'wb')
    |  woa  +=  addition
    |  wos  -=  substraction
    |  wom  *=  multiply
    |  wod  /=  divide
    |  wox  ^=  xor
    |  woo  |=  or
    |  woA  &=  and
    |  woR  random bytes (alias for 'wr $b'
    |  wor  >>= shift right
    |  wol  <<= shift left
    |  wo2  2=  2 byte endian swap
    |  wo4  4=  4 byte endian swap


This way it is possible to implement ciphering algorithms using radare core primitives.

A sample session doing a xor(90) + addition(01 02)

    [0x7fcd6a891630]> px
    - offset -       0 1  2 3  4 5  6 7  8 9  A B  C D  E F  0123456789ABCDEF
    0x7fcd6a891630  4889 e7e8 6839 0000 4989 c48b 05ef 1622  H...h9..I......"
    0x7fcd6a891640  005a 488d 24c4 29c2 5248 89d6 4989 e548  .ZH.$.).RH..I..H
    0x7fcd6a891650  83e4 f048 8b3d 061a 2200 498d 4cd5 1049  ...H.=..".I.L..I
    0x7fcd6a891660  8d55 0831 ede8 06e2 0000 488d 15cf e600  .U.1......H.....
    
    
    [0x7fcd6a891630]> wox 90
    [0x7fcd6a891630]> px
    - offset -       0 1  2 3  4 5  6 7  8 9  A B  C D  E F  0123456789ABCDEF
    0x7fcd6a891630  d819 7778 d919 541b 90ca d81d c2d8 1946  ..wx..T........F
    0x7fcd6a891640  1374 60d8 b290 d91d 1dc5 98a1 9090 d81d  .t`.............
    0x7fcd6a891650  90dc 197c 9f8f 1490 d81d 95d9 9f8f 1490  ...|............
    0x7fcd6a891660  13d7 9491 9f8f 1490 13ff 9491 9f8f 1490  ................
    
    
    [0x7fcd6a891630]> woa 01 02
    [0x7fcd6a891630]> px
    - offset -       0 1  2 3  4 5  6 7  8 9  A B  C D  E F  0123456789ABCDEF
    0x7fcd6a891630  d91b 787a 91cc d91f 1476 61da 1ec7 99a3  ..xz.....va.....
    0x7fcd6a891640  91de 1a7e d91f 96db 14d9 9593 1401 9593  ...~............
    0x7fcd6a891650  c4da 1a6d e89a d959 9192 9159 1cb1 d959  ...m...Y...Y...Y
    0x7fcd6a891660  9192 79cb 81da 1652 81da 1456 a252 7c77  ..y....R...V.R|w
