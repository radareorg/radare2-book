## Basic usage

The learning curve for radare is usually somewhat steep at the beginning. Although after an hour of using it you will easily understand how most things work, and how to combine the various tools radare offers. I strongly encourage you to read the rest of this book to help you understand how everything works and to improve your skills.

Navigating a binary file is done using three simple actions: seek, print, and alterate.

The 'seek' command is abbreviated as `s` and accepts an expression as its argument. This expression can be something like `10`, `+0x25`, or `[0x100+ptr_table]`. If you are working with block-based files, you may prefer to set the block size to 4K or the size required with the `b` command, and move forward or backwards at seeks aligned to the block size using the `>` and `<` commands.

The 'print' command is abbreviated as `p` and accepts a letter to specify the print mode. The most common ones are `px` for printing in hexadecimal, and `pd` for disassembling.

To 'write', specify the `-w` option when opening the file, `radare -w`. The `w` command can be used to write strings (no letter specified), hexpairs (an `x` specified), or even assembly opcodes (an `a` specified):

    > w hello world         ; string
    > wx 90 90 90 90        ; hexpairs
    > wa jmp 0x8048140      ; assemble
    > wf inline.bin         ; write contents of file

Appending a `?` to the command will show its help message, for example, `p?`.

To enter visual mode, press `V<enter>`. Use `q` to quit visual mode and return to the prompt.

In visual mode you can use the hjkl keys to navigate (left, down, up, and right, respectively). You can use these keys in cursor mode (`c`). To select keys in cursor mode, hold down the shift key while using any of the hjkl keys.

While in visual mode you can also insert (alterate bytes) by pressing `i`, followed by `tab` to switch between the hex or string column. Pressing `q` inside the hex panel returns you to visual mode.
