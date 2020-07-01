# Visual Mode

The visual mode is a more user-friendly interface alternative to radare2's command-line prompt. It allows easy navigation, has a cursor mode for selecting bytes, and offers numerous key bindings to simplify debugger use.
To enter visual mode, use `V` command. To exit from it back to command line, press `q`.

## Navigation

Navigation can be done using HJKL or arrow keys and PgUp/PgDown keys. It also understands usual Home/End keys.
Like in Vim the movements can be repeated by preceding the navigation key with the number, for
example `5j` will move down for 5 lines, or `2l` will move 2 characters right.

![Visual Mode](visualmode.png)

## print modes aka panels

The Visual mode uses "print modes" which are basically different panel that you can rotate. By default those are:

↻ **Hexdump panel** -> **Disassembly panel** → **Debugger panel** → **Hexadecimal words dump panel** → **Hex-less hexdump panel** → **Op analysis color map panel** → **Annotated hexdump panel** ↺.

Notice that the top of the panel contains the command which is used, for example for the disassembly panel:

```
[0x00404890 16% 120 /bin/ls]> pd $r @ entry0
```

## Getting Help

To see help on all key bindings defined for visual mode, press `?`:
```
Visual mode help:
 ?        show this help
 ??       show the user-friendly hud
 %        in cursor mode finds matching pair, or toggle autoblocksz
 @        redraw screen every 1s (multi-user view)
 ^        seek to the begining of the function
 !        enter into the visual panels mode
 _        enter the flag/comment/functions/.. hud (same as VF_)
 =        set cmd.vprompt (top row)
 |        set cmd.cprompt (right column)
 .        seek to program counter
 \        toggle visual split mode
 "        toggle the column mode (uses pC..)
 /        in cursor mode search in current block
 :cmd     run radare command
 ;[-]cmt  add/remove comment
 0        seek to beginning of current function
 [1-9]    follow jmp/call identified by shortcut (like ;[1])
 ,file    add a link to the text file
 /*+-[]   change block size, [] = resize hex.cols
 </>      seek aligned to block size (seek cursor in cursor mode)
 a/A      (a)ssemble code, visual (A)ssembler
 b        browse symbols, flags, configurations, classes, ...
 B        toggle breakpoint
 c/C      toggle (c)ursor and (C)olors
 d[f?]    define function, data, code, ..
 D        enter visual diff mode (set diff.from/to
 e        edit eval configuration variables
 f/F      set/unset or browse flags. f- to unset, F to browse, ..
 gG       go seek to begin and end of file (0-$s)
 hjkl     move around (or HJKL) (left-down-up-right)
 i        insert hex or string (in hexdump) use tab to toggle
 mK/'K    mark/go to Key (any key)
 M        walk the mounted filesystems
 n/N      seek next/prev function/flag/hit (scr.nkey)
 g        go/seek to given offset
 O        toggle asm.pseudo and asm.esil
 p/P      rotate print modes (hex, disasm, debug, words, buf)
 q        back to radare shell
 r        refresh screen / in cursor mode browse comments
 R        randomize color palette (ecr)
 sS       step / step over
 t        browse types
 T        enter textlog chat console (TT)
 uU       undo/redo seek
 v        visual function/vars code analysis menu
 V        (V)iew graph using cmd.graph (agv?)
 wW       seek cursor to next/prev word
 xX       show xrefs/refs of current function from/to data/code
 yY       copy and paste selection
 z        fold/unfold comments in disassembly
 Z        toggle zoom mode
 Enter    follow address of jump/call
Function Keys: (See 'e key.'), defaults to:
  F2      toggle breakpoint
  F4      run to cursor
  F7      single step
  F8      step over
  F9      continue
```
