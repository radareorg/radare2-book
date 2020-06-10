# Visual Panels

## Concept

Visual Panels is characterized by the following core functionalities:

1. Split Screen
2. Display multiple screens such as Symbols, Registers, Stack, as well as custom panels
3. Menu will cover all those commonly used commands for you so that you don't have to memorize any of them

CUI met some useful GUI as the menu, that is Visual Panels.

Panels can be accessed from visual mode by using `!`.

## Overview

![Panels Overview](panels_overview.png)

## Commands
```
|Visual Ascii Art Panels:
| |      split the current panel vertically
| -      split the current panel horizontally
| :      run r2 command in prompt
| _      start the hud input mode
| ?      show this help
| ??     show the user-friendly hud
| !      run r2048 game
| .      seek to PC or entrypoint
| *      show pseudo code/r2dec in the current panel
| /      highlight the keyword
| (      toggle snow
| &      toggle cache
| [1-9]  follow jmp/call identified by shortcut (like ;[1])
| ' '    (space) toggle graph / panels
| tab    go to the next panel
| a      toggle auto update for decompiler
| b      browse symbols, flags, configurations, classes, ...
| c      toggle cursor
| C      toggle color
| d      define in the current address. Same as Vd
| D      show disassembly in the current panel
| e      change title and command of current panel
| g      go/seek to given offset
| G      show graph in the current panel
| i      insert hex
| hjkl   move around (left-down-up-right)
| J      scroll panels down by page
| K      scroll panels up by page
| H      scroll panels left by page
| L      scroll panels right by page
| m      select the menu panel
| M      open new custom frame
| nN     create new panel with given command
| pP     seek to next or previous scr.nkey
| q      quit, back to visual mode
| r      toggle jmphints/leahints
| sS     step in / step over
| t      rotate related commands in a panel
| uU     undo / redo seek
| w      start Window mode
| V      go to the graph mode
| X      close current panel
| z      swap current panel with the first one
```

## Basic Usage

Use `tab` to move around the panels until you get to the targeted panel. Then, use `hjkl`, just like in vim, to scroll the panel you are currently on.
Use `S` and `s` to step over/in, and all the panels should be updated dynamically while you are debugging.
Either in the Registers or Stack panels, you can edit the values by inserting hex. This will be explained later.
While hitting `tab` can help you moving between panels, it is highly recommended to use `m` to open the menu.
As usual, you can use `hjkl` to move around the menu and will find tons of useful stuff there.

## Split Screen

`|` is for the vertical and `-` is for the horizontal split. You can delete any panel by pressing `X`.

Split panels can be resized from Window Mode, which is accessed with `w`.

## Window Mode Commands
```
|Panels Window mode help:
| ?      show this help
| ??     show the user-friendly hud
| Enter  start Zoom mode
| c      toggle cursor
| hjkl   move around (left-down-up-right)
| JK     resize panels vertically
| HL     resize panels horizontally
| q      quit Window mode

```

## Edit Values

Either in the Register or Stack panel, you can edit the values. Use `c` to activate cursor mode and you can move the cursor by pressing `hjkl`, as usual. Then, hit `i`, just like the insert mode of vim, to insert a value.
