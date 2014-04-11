#Chapter 4: Visual mode

The visual mode is a user-friendlier interface for the commandline prompt of radare which accepts hjkl movement keys, a cursor for selecting bytes and some keybindings to ease the use of the debugger.

In this mode you can change the configuration in a easy way using the `e` (eval) key. Or just track the flags and walk thru the flagspaces pressing `t`.

To get a help of all the keybindings hooked in visual mode you can press `?`:

    Visual mode help:
    ?        show this help or manpage in cursor mode
    _        enter the hud
    .        seek to program counter
    /        in cursor mode search in current block
    :cmd     run radare command
    ;[-]cmt  add/remove comment
    /*+-[]   change block size, [] = resize hex.cols
    >||<     seek aligned to block size
    i/a/A    (i)nsert hex, (a)ssemble code, visual (A)ssembler
    b/B      toggle breakpoint / automatic block size
    c/C      toggle (c)ursor and (C)olors
    d[f?]    define function, data, code, ..
    D        enter visual diff mode (set diff.from/to)
    e        edit eval configuration variables
    f/F      set/unset flag
    gG       go seek to begin and end of file (0-$s)
    hjkl     move around (or HJKL) (left-down-up-right)
    mK/'K    mark/go to Key (any key)
    M        walk the mounted filesystems
    n/N      seek next/prev function/flag/hit (scr.nkey)
    o        go/seek to given offset
    p/P      rotate print modes (hex, disasm, debug, words, buf)
    q        back to radare shell
    R        randomize color palette (ecr)
    sS       step / step over
    t        track flags (browse symbols, functions..)
    T        browse anal info and comments
    v        visual code analysis menu
    V/W      (V)iew graph using cmd.graph (agv?), open (W)ebUI
    uU       undo/redo seek
    x        show xrefs to seek between them
    yY       copy and paste selection
    z        toggle zoom mode
    Enter    follow address of jump/call
    Function Keys: (See 'e key.'), defaults to:
      F2      toggle breakpoint
      F7      single step
      F8      step over
      F9      continue


From the visual mode you can toggle the insert and cursor modes with the `i` and `c` keys.



##4.1 Visual cursor

Pressing lowercase `c` makes the cursor appear or disappear. The cursor is used to select a range of bytes or just point to a byte to flag it (press `f` to create a new flag where the cursor points to)

If you select a range of bytes press `i` and then a byte array to overwrite the selected bytes with the ones you choose in a circular copy way. For example:

    <select 10 bytes in visual mode using upper hjkl>
    <press 'i' and then '12 34'>
    
The 10 bytes selected will become: 12 34 12 34 12 34 12 34 12 34
The byte range selection can be used together with the `d` key to change the data type of the selected bytes into a string, code or a byte array.

That's useful to enhace the disassembly, add metadata or just align the code if there are bytes mixed with code.

In cursor mode you can set the block size by simply moving it to the position you want and pressing `_`. Then change block size.


##4.2 Visual insert

The insert mode allows you to write bytes at nibble-level like most common hexadecimal editors. In this mode you can press `<tab>` to switch between the hexa and ascii columns of the hexadecimal dump.

To get back to the normal mode, just press `<tab>` to switch to the hexadecimal view and press `q`. (NOTE: if you press `q` in the ascii view...it will insert a `q` instead of quit this mode)

There are other keys for inserting and writing data in visual mode. Basically by pressing `i` key you'll be prompted for an hexpair string or use `a` for writing assembly where the cursor points.


##4.3 Visual xrefs

radare implements many user-friendly features for the visual interface to walk thru the assembly code. One of them is the `x` key that popups a menu for selecting the xref (data or code) against the current seek and then jump there. 
For example when pressing x when looking at those XREF:


    |  ....--> ; CODE (CALL) XREF from 0x00402b98 (fcn.004028d0)
    |  ....--> ; CODE (CALL) XREF from 0x00402ba0 (fcn.004028d0)                  
    |  ....--> ; CODE (CALL) XREF from 0x00402ba9 (fcn.004028d0)       
    |  ....--> ; CODE (CALL) XREF from 0x00402bd5 (fcn.004028d0)        
    |  ....--> ; CODE (CALL) XREF from 0x00402beb (fcn.004028d0)   
    |  ....--> ; CODE (CALL) XREF from 0x00402c25 (fcn.004028d0)     
    |  ....--> ; CODE (CALL) XREF from 0x00402c31 (fcn.004028d0)              
    |  ....--> ; CODE (CALL) XREF from 0x00402c40 (fcn.004028d0)            
    |  ....--> ; CODE (CALL) XREF from 0x00402c51 (fcn.004028d0)
    
After pressing `x`

    [GOTO XREF]>                                                                       
    [0] CODE (CALL) XREF 0x00402b98 (loc.00402b38)
    [1] CODE (CALL) XREF 0x00402ba0 (loc.00402b38)
    [2] CODE (CALL) XREF 0x00402ba9 (loc.00402b38)
    [3] CODE (CALL) XREF 0x00402bd5 (loc.00402b38)
    [4] CODE (CALL) XREF 0x00402beb (loc.00402b38)
    [5] CODE (CALL) XREF 0x00402c25 (loc.00402b38)
    [6] CODE (CALL) XREF 0x00402c31 (loc.00402b38)
    [7] CODE (CALL) XREF 0x00402c40 (loc.00402b38)
    [8] CODE (CALL) XREF 0x00402c51 (loc.00402b38)
    [9] CODE (CALL) XREF 0x00402c60 (loc.00402b38)    

All the calls and jumps are numbered (1, 2, 3...) these numbers are the keybindings for seeking there from the visual mode.
All the seek history is stored, by pressing `u` key you will go back in the seek history time :)
