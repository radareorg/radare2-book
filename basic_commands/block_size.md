## Block size

The block size is the default view size for radare. All commands will work with this constraint, but you can always temporally change the block size just giving a numeric argument to the print commands for example (px 20)

    [0xB7F9D810]> b?
    Usage: b[f] [arg]
    b         display current block size
    b+3       increase blocksize by 3
    b-16      decrement blocksize by 16
    b 33      set block size to 33
    b eip+4   numeric argument can be an expression
    bf foo    set block size to flag size
    bm 1M     set max block size

The `b` command is used to change the block size:

    [0x00000000]> b 0x100   ; block size = 0x100
    [0x00000000]> b +16     ;  ... = 0x110
    [0x00000000]> b -32     ;  ... = 0xf0
    
The `bf` command is used to change the block size to the one specified by a flag. For example in symbols, the block size of the flag represents the size of the function.

    [0x00000000]> bf sym.main    ; block size = sizeof(sym.main)
    [0x00000000]> pd @ sym.main  ; disassemble sym.main
    ...

You can perform these two operations in a single one (pdf):

     [0x00000000]> pdf @ sym.main
