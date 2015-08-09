## Visual Cursor Mode

Pressing lowercase `c` toggles the cursor mode. When this mode is active, currently selected byte (or byte range) is highlighted by having a highlighted background.

![Cursor at 0x00404896](cursor.png)

The cursor is used to select a range of bytes or simply to point to a byte. You can use the latter to create a named flag. Seek to required position, then press `f` and enter a name for a flag.
If you select a range of bytes (with HJKL and SHIFT key pressed), and file write mode has been enabled with `-w` radare2 option, you can press `i` and then enter a byte array to overwrite selected range with new values, used as circular buffer. For example:

    <select 10 bytes in visual mode using SHIFT+HJKL>
    <press 'i' and then enter '12 34'>
    
10 bytes you have selected will be changed to "12 34" repeated in a circular manner: 12 34 12 34 12 34 12 34 12 34.
A byte range selection can be used together with the `d` key to set associated data type: a string, code or, word, or to perform other actions as indicated in the menu presented on the key press:

    B    set as short word (2 bytes)
    c    set as code
    C    define flag color (fc)
    d    set as data
    e    end of function
    f    analyze function
    F    format
    j    merge down (join this and next functions)
    k    merge up (join this and previous function)
    h    highlight word
    m    manpage for current call
    q    quit/cancel operation
    r    rename function
    R    find references /r
    s    set string
    S    set strings in current block
    u    undefine metadata here
    w    set as 32bit word
    W    set as 64bit word
    q    quit this menu

This can be used to enhance disassembly view, to add metadata or to set code boundaries for cases when instructions are intermixed with data.
In cursor mode, you can set a block size by simply moving cursor to position you want and then pressing `_` to invoke HUD menu. Then change block size.
