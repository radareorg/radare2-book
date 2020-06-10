## Basic Radare2 Usage

The learning curve is usually somewhat steep at the beginning. Although after an hour of using it you should easily understand how most things work, and how to combine the various tools radare offers. You are encouraged to read the rest of this book to understand how some non-trivial things work, and to ultimately improve your skills.

![learning_curve](learning_curve.png)

Navigation, inspection and modification of a loaded binary file is performed using three simple actions: seek (to position), print (buffer), and alternate (write, append).

The 'seek' command is abbreviated as `s` and accepts an expression as its argument. The expression can be something like `10`, `+0x25`, or `[0x100+ptr_table]`. If you are working with block-based files, you may prefer to set the block size to a required value with `b` command, and seek forward or backwards with positions aligned to it. Use `s++` and `s--` commands to navigate this way.

If radare2 opens an executable file, by default it will open the file in Virtual Addressing (VA) mode and the sections will be mapped to their virtual addresses. In VA mode, seeking is based on the virtual address and the starting position is set to the entry point of the executable. Using `-n` option you can suppress this default behavior and ask radare2 to open the file in non-VA mode for you. In non-VA mode, seeking is based on the offset from the beginning of the file.

The 'print' command is abbreviated as `p` and has a number of submodes — the second letter specifying a desired print mode. Frequent variants include `px` to print in hexadecimal, and `pd` for disassembling.

To be allowed to write files, specify the `-w` option to radare2 when opening a file. The `w` command can be used to write strings, hexpairs (`x` subcommand), or even assembly opcodes (`a` subcommand). Examples:

```
> w hello world         ; string
> wx 90 90 90 90        ; hexpairs
> wa jmp 0x8048140      ; assemble
> wf inline.bin         ; write contents of file
```

Appending a `?` to a command will show its help message, for example, `p?`.
Appending `?*` will show commands starting with the given string, e.g. `p?*`.

To enter visual mode, press `V<enter>`. Use `q` to quit visual mode and return to the prompt.

In visual mode you can use HJKL keys to navigate (left, down, up, and right, respectively). You can use these keys in cursor mode toggled by `c` key. To select a byte range in cursor mode, hold down `SHIFT` key, and press navigation keys HJKL to mark your selection.

While in visual mode, you can also overwrite bytes by pressing `i`. You can press `TAB` to switch between the hex (middle) and string (right) columns. Pressing `q` inside the hex panel returns you to visual mode. By pressing `p` or `P` you can scroll different visual mode representations. There is a second most important visual mode - curses-like panels interface, accessible with `V!` command.

