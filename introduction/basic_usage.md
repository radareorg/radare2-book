## Basic Radare Usage

The learning curve for radare is usually somewhat steep at the beginning. Although after an hour of using it you should easily understand how most things work, and how to combine various tools radare offers, you are encouraged to read the rest of this book to understand how some non-trivial things work, and to ultimately improve your skills with radare.

Navigation, inspection and modification of a loaded binary file is performed using three simple actions: seek (to position), print (buffer), and alterate (write, append).

The 'seek' command is abbreviated as `s` and accepts an expression as its argument. The expression can be something like `10`, `+0x25`, or `[0x100+ptr_table]`. If you are working with block-based files, you may prefer to set the block size to a required value with `b` command, and seek forward or backwards with positions aligned to it. Use `>` and `<` commands to navigate this way.

And there are two addressing modes when you use 'seek' command to go around the file. If you open an executable or library file(e.g, the ELF files under *nix systems), by default you use the virtual addressing mode in which you navigate in the virtual address space of the executable or object file. And r2 will set the starting position to the entry point address of the executable(in case of object file, the starting position will be set to the beginning of text section). If you open a plain binary file, you seek using the offset from the beginning of the file as the address. Examples:

	> readelf -h web_of_science2     # executable
		ELF Header:
          Magic:   7f 45 4c 46 02 01 01 00 00 00 00 00 00 00 00 00
          Class:                             ELF64
          Data:                              2's complement, little endian
          Version:                           1 (current)
          OS/ABI:                            UNIX - System V
          ABI Version:                       0
          Type:                              EXEC (Executable file)
          Machine:                           Advanced Micro Devices X86-64
          Version:                           0x1
          Entry point address:               0x400930                                # the entry point address
          Start of program headers:          64 (bytes into file)
          Start of section headers:          8712 (bytes into file)
          Flags:                             0x0
          Size of this header:               64 (bytes)
          Size of program headers:           56 (bytes)
          Number of program headers:         9
          Size of section headers:           64 (bytes)
          Number of section headers:         28
          Section header string table index: 27

	> r2 web_of_science2
		[0x00400930]> ? $$            # showing the current position
		4196656 **0x400930** 020004460 4M 40000:0930 4196656 00110000 4196656.0 0.000000f 0.000000
		# the hexadecimal value of the current value is the same as the entry point address

	> file test.cfg   # plain binary file
		test.cfg: data
	> r2 test.cfg
		[0x00000000]> ? $$
        0 0x0 00 0 0000:0000 0 00000000 0.0 0.000000f 0.000000
		# the current position is 0

The 'print' command is abbreviated as `p` and has a number of submodes — the second letter specifying a desired print mode. Frequent variants include `px` to print in hexadecimal, and `pd` for disassembling.

To be allowed to write files, specify the `-w` option to radare when opening a file. The `w` command can be used to write strings, hexpairs (`x` subcommand), or even assembly opcodes (`a` subcommand). Examples:

    > w hello world         ; string
    > wx 90 90 90 90        ; hexpairs
    > wa jmp 0x8048140      ; assemble
    > wf inline.bin         ; write contents of file

Appending a `?` to a command will show its help message, for example, `p?`.

To enter visual mode, press `V<enter>`. Use `q` to quit visual mode and return to the prompt.
In visual mode you can use HJKL keys to navigate (left, down, up, and right, respectively). You can use these keys in cursor mode toggled by `c` key. To select a byte range in cursor mode, hold down `SHIFT` key, and press navigation keys HJKL to mark your selection.
While in visual mode, you can also overwrite bytes by pressing `i`. You can press `TAB` to switch between the hex (middle) and string (right) columns. Pressing `q` inside the hex panel returns you to visual mode.
