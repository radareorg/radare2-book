## Common Configuration Variables

Below is a list of the most frequently used configuration variables. You can get a complete list by issuing `e` command without arguments. For example, to see all variables defined in the "cfg" namespace, issue `e cfg.` (mind the ending dot). You can get help on any eval configuration variable by using `e? cfg.`

    asm.arch
Defines target CPU architecture used for disassembling (`pd`, `pD` commands) and code analysis (`a` command). You can find the list of possible value by looking at the result of `e asm.arch=?` or `rasm2 -L`.
It is quite simple to add new architectures for disassembling and analyzing code. There is an interface for that. For x86, it is used to attach a number of third-party disassembler engines, including GNU binutils, Udis86 and a few of handmade ones.

    asm.bits
Determines width in bits of registers for current architecture. Supported values: 8, 16, 32, 64. Note that not all target architectures support all combinations for asm.bits.

    asm.syntax
Changes syntax flavor for disassembler between Intel and AT&T. At the moment, this setting affects Udis86 disassembler for Intel 32/Intel 64 targets only. Supported values are `intel` and `att`.

    asm.pseudo
A boolean value to choose a string disassembly engine. "False" indicates a native one, defined by current architecture, "true" activates a pseudocode strings format; for example, it will show `eax=ebx` instead of a `mov eax, ebx`.

    asm.os
Selects a target operating system of currently loaded binary. Usually OS is automatically detected by `rabin -rI`. Yet, `asm.os` can be used to switch to a different syscall table employed by another OS.

    asm.flags
If defined to "true", disassembler view will have flags column.

    asm.lines.call
If set to "true", draw lines at the left of disassemble output (`pd`, `pD` commands) to graphically represent control flow changes (jumps and calls) that are targeted inside current block. Also, see `asm.linesout`.

    asm.linesout
When defined as "true", the disassembly view will also draw control flow lines that go ouside of the block.

    asm.linestyle
A boolean value which changes the direction of control flow analysis. If set to "false", it is done from top to bottom of a block; otherwise, it goes from bottom to top. The "false" setting seems to be a better choice for improved readability, and is the default one.

    asm.offset
Boolean value which controls visibility of offsets for individual disassembled instructions.

    asm.trace
A boolean value that controls displaying of tracing information (sequence number and counter) at the left of each opcode. It is used to assist programs trace analysis.

    asm.bytes
A boolean value used to show or hide displaying of raw bytes of instructions.

    cfg.bigendian
Change endianness. "true" means big-endian, "false" is for little-endian.
"file.id" and "file.flag" both to be true.

    cfg.newtab
If this variable is enabled, help messages will be displayed along with command names in tab completion for commands.

    scr.color
This boolean variable enables or disables colorized screen output.

    scr.seek
This variable accepts an expression, a pointer (eg. eip), etc. If set, radare will set seek position to its value on startup.

    cfg.fortunes
Enables or disables "fortune" messages displayed at each radare start.
