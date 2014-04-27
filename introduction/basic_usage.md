## Basic usage

Many people requested a sample session of using radare to help in understanding how the shell works and how to perform the most common tasks like disassembling, seeking, binary patching and debugging.

I strongly encourage you to read the rest of this book to help you understand better how everything works and enhace your skills, the learning curve of radare is usually a bit harder at the beggining, but after an hour of using it you will easily understand how most of the things work and how to get them cooperate together :)

For walking thru the binary file you will use three different kind of basic actions: seek, print and alterate.

To 'seek' there's an specific command abreviated as `s` than accepts an expression as argument that can be something like `10`, `+0x25` or `[0x100+ptr_table]`. If you are working with block-based files you may prefer to set up the block size to 4K or the size required with the command `b` and move forward or backward at seeks aligned to the block size using the `>` and `<` commands.

The 'print' command aliased as `p`, accepts a second letter to specify the print mode selected. The most common ones are `px` for printing in hexadecimal, `pd` for disassembling.

To 'write' open the file with `radare -w`. This should be specified while opening the file.
You can use the `w` command to write strings or `wx` for hexpair strings:

    > w hello world         ; string
    > wx 90 90 90 90        ; hexpairs
    > wa jmp 0x8048140      ; assemble
    > wf inline.bin         ; write contents of file

Appending a `?` to the command you will get the help message of it. (p? for example)

Enter the visual mode pressing `V<enter>`, and return to the prompt using the `q` key.

In the visual mode you should use hjkl keys which are the default ones for scrolling (like left,down,up,right). So entering in cursor mode (`c`) you will be able select bytes if using the shift together with HJKL.

In the visual mode you can insert (alterate bytes) pressing `i` and then <tab> to switch between the hex or string column. Pressing `q` in hex panel to return into the visual mode.
