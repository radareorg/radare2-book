## 2.2 Common configuration variables

Here's a list of the most common eval configuration variables, you can get the complete list using the `e` command without arguments or just use `e cfg.` (ending with dot, to list all the configuration variables of the cfg. space). You can get help on any eval configuration variable using : `??e cfg.` for example

    asm.arch
Defines the architecture to be used while disassembling (pd, pD commands) and analyzing code (`a` command). Currently it handles `intel32`, `intel64`, `mips`, `arm16`, `arm` `java`, `csr`, `sparc`, `ppc`, `msil` and `m68k`.

It is quite simple to add new architectures for disassembling and analyzing code, so there is an interface adapted for the GNU disassembler and others for udis86 or handmade ones.

    asm.bits
This variable will change the `asm.arch` one (in radare1) and viceversa (is determined by asm.arch). It determines the size in bits of the registers for the selected architecture. This is 8, 16, 32, 64.

    asm.syntax
Defines the syntax flavour to be used while disassembling. This is currently only targeting the udis86 disassembler for the x86 (32/64 bits). The supported values are `intel` or `att`.

    asm.pseudo
Boolean value that determines which string disassembly engine to use (the native one defined by the architecture) or the one filtered to show pseudocode strings. This is `eax=ebx` instead of a `mov eax, ebx` for example.

    asm.os
Defines the target operating system of the binary to analyze. This is automatically defined by `rabin -rI` and it's useful for switching between the different syscall tables and perform different depending on the OS.

    asm.flags
If defined to `true` shows the flags column inside the disassembly.

    asm.linescall
Draw lines at the left of the offset in the dissassemble print format (pd, pD) to graphically represent jumps and calls inside the current block.

    asm.linesout
When defined as `true`, also draws the jump lines in the current block that goes ouside of this block.

    asm.linestyle
Can get `true` or `false` values and makes the line analysis be performed from top to bottom if false or bottom to top if true. `false` is the optimal and default value for readability.

    asm.offset
Boolean value that shows or hides the offset address of the disassembled opcode.

    asm.profile
Set how much information is showed to the user on disassembly. Can get the values `default`, `simple`, `gas`, `smart`, `debug`, `full`.

This eval will modify other asm. variables to change the visualization properties for the disassembler engine. `simple` asm.profile will show only offset+opcode, and `debug` will show information about traced opcodes, stack pointer delta, etc..

    asm.trace
Show tracing information at the left of each opcode (sequence number and counter). This is useful to read execution traces of programs.

    asm.bytes
Boolean value that shows or hides the bytes of the disassembled opcode.

    cfg.bigendian
Choose the endian flavour `true` for big, `false` for little.

    file.analyze
Runs `.af* @@ sym.` and `.af* @ entrypoint`, after resolving the symbols while loading the binary, to determine the maximum information about the code analysis of the program. This will not be used while opening a project file, so it is preloaded. This option requires file.id and file.flag to be true.

    scr.color
This boolean variable allows to enable or disable the colorized output

    scr.seek
This variable accepts an expression, a pointer (eg. eip), etc. radare will automatically seek to make sure its value is always within the limits of the screen.

    cfg.fortunes
Enables or disables the 'fortune' message at the begining of the program