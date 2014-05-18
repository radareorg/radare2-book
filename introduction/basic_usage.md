## Basic usage

Many people requested a sample session of using radare to help in understanding how the shell works and how to perform the most common tasks like disassembling, seeking, binary patching and debugging.

I strongly encourage you to read the rest of this book to help you understand better how everything works and to improve your skills. The learning curve for radare is usually a bit steep at the beggining. However, after an hour of using it you will easily understand how most of the things work and how to combine the various tools radare offers :)

Navigating a binary file is done using three simple actions: seek, print and alterate.

The 'seek' command is abbreviated as `s` and accepts an expression as its argument. This expression can be something like `10`, `+0x25` or `[0x100+ptr_table]`. If you are working with block-based files you may prefer to set up the block size to 4K or the size required with the command `b` and move forward or backward at seeks aligned to the block size using the `>` and `<` commands.

The 'print' command (short: `p`), accepts a second letter to specify the print mode. The most common ones are `px` for printing in hexadecimal, `pd` for disassembling.

To 'write' first open the file with `radare -w`. This should be specified while opening the file.
You can then use the `w` command to write strings or `wx` for hexpair strings:

    > w hello world         ; string
    > wx 90 90 90 90        ; hexpairs
    > wa jmp 0x8048140      ; assemble
    > wf inline.bin         ; write contents of file

Appending a `?` to the command will show its help message (example: p?).

To enter visual mode press `V<enter>`. To quit visual mode and return to the prompt use the `q` key.

In visual mode you can use the hjkl keys to navigate (left, down, up, right respectively). You can use these keys in cursor mode (`c`). To select keys in cursor mode, simply hold down the shift key while using any of the hjkl keys.

While in visual mode you can also insert (alterate bytes) pressing `i` followed by `<tab>` to switch between the hex or string column. Pressing `q` inside the hex panel returns you to visual mode.
