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