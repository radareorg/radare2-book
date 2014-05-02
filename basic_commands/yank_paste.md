## Yank/Paste

You can yank/paste bytes in visual mode using the `y` and `Y` key bindings which are alias for the `y` and `yy` commands of the shell. These commands operate on an internal buffer which stores N bytes counted from the current seek. You can write-back to another seek using the `yy` command.

    [0x00000000]> y?
    |Usage: y[ptxy] [len] [[@]addr]
    | y                show yank buffer information (srcoff len bytes)
    | y 16             copy 16 bytes into clipboard
    | y 16 0x200       copy 16 bytes into clipboard from 0x200
    | y 16 @ 0x200     copy 16 bytes into clipboard from 0x200
    | yp               print contents of clipboard
    | yx               print contents of clipboard in hexadecimal
    | yt 64 0x200      copy 64 bytes from current seek to 0x200
    | yf 64 0x200 file copy 64 bytes from 0x200 from file (opens w/ io), use -1 for all bytes
    | yfa file copy    copy all bytes from from file (opens w/ io)
    | yy 0x3344        paste clipboard

Sample session:

    [0x00000000]> s 0x100    ; seek at 0x100
    [0x00000100]> y 100      ; yanks 100 bytes from here
    [0x00000200]> s 0x200    ; seek 0x200
    [0x00000200]> yy         ; pastes 100 bytes
    
    
You can perform a yank and paste in a single line by just using the `yt` command (yank-to). The syntax is as follows:

    [0x4A13B8C0]> x
       offset   0 1  2 3  4 5  6 7  8 9  A B  0123456789AB
    0x4A13B8C0, 89e0 e839 0700 0089 c7e8 e2ff ...9........
    0x4A13B8CC, ffff 81c3 eea6 0100 8b83 08ff ............
    0x4A13B8D8, ffff 5a8d 2484 29c2           ..Z.$.).    
    
    [0x4A13B8C0]> yt 8 0x4A13B8CC @ 0x4A13B8C0
    
    [0x4A13B8C0]> x
       offset   0 1  2 3  4 5  6 7  8 9  A B  0123456789AB
    0x4A13B8C0, 89e0 e839 0700 0089 c7e8 e2ff ...9........
    0x4A13B8CC, 89e0 e839 0700 0089 8b83 08ff ...9........
    0x4A13B8D8, ffff 5a8d 2484 29c2           ..Z.$.).    
