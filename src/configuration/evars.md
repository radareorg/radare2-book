## Configuration Variables

Below is a list of the most frequently used configuration variables. You can get a complete list by issuing `e` command without arguments. For example, to see all variables defined in the "cfg" namespace, issue `e cfg.` (mind the ending dot). You can get help on any eval configuration variable by using `e? cfg.`

The `e??` command to get help on all the evaluable configuration variables of radare2. As long as the output of this command is pretty large you can combine it with the internal grep `~` to filter for what you are looking for:

![e??~color](../img/configuration/e--color.png)

The Visual mode has an eval browser that is accessible through the `Vbe` command.

### asm.arch

Defines the target CPU architecture used for disassembling (`pd`, `pD` commands) and code analysis (`a` command). You can find the list of possible values by looking at the result of `e asm.arch=?` or `rasm2 -L`.
It is quite simple to add new architectures for disassembling and analyzing code. There is an interface for that. For x86, it is used to attach a number of third-party disassembler engines, including GNU binutils, Udis86 and a few handmade ones.

### asm.bits

Determines width in bits of registers for the current architecture. Supported values: 8, 16, 32, 64. Note that not all target architectures support all combinations for asm.bits.

### asm.syntax

Changes syntax flavor for disassembler between Intel and AT&T. At the moment, this setting affects Udis86 disassembler for Intel 32/Intel 64 targets only. Supported values are `intel` and `att`.

### asm.pseudo

A boolean value to choose a string disassembly engine. "False" indicates a native one, defined by the current architecture, "true" activates a pseudocode strings format; for example, it will show `eax=ebx` instead of a `mov eax, ebx`.

### asm.os

Selects a target operating system of currently loaded binary. Usually, OS is automatically detected by `rabin -rI`. Yet, `asm.os` can be used to switch to a different syscall table employed by another OS.

### asm.flags

If defined to "true", disassembler view will have flags column.

### asm.lines.call

If set to "true", draw lines at the left of the disassemble output (`pd`, `pD` commands) to graphically represent control flow changes (jumps and calls) that are targeted inside current block. Also, see `asm.linesout`.

### asm.linesout

When defined as "true", the disassembly view will also draw control flow lines that go outside of the block.

### asm.linestyle

A boolean value which changes the direction of control flow analysis. If set to "false", it is done from top to bottom of a block; otherwise, it goes from bottom to top. The "false" setting seems to be a better choice for improved readability and is the default one.

### asm.offset

Boolean value which controls the visibility of offsets for individual disassembled instructions.

### asm.trace

A boolean value that controls displaying of tracing information (sequence number and counter) at the left of each opcode. It is used to assist with programs trace analysis.

### asm.bytes

A boolean value used to show or hide displaying of raw bytes of instructions.

### cfg.bigendian

Change endianness. "true" means big-endian, "false" is for little-endian.
"file.id" and "file.flag" both to be true.

### cfg.newtab

If this variable is enabled, help messages will be displayed along with command names in tab completion for commands.

### scr.color

This variable specifies the mode for colorized screen output: "false" (or 0) means no colors, "true" (or 1) means 16-colors mode, 2 means 256-colors mode, 3 means 16 million-colors mode. If your favorite theme looks weird, try to bump this up.

### scr.seek

This variable accepts a full-featured expression or a pointer/flag (eg. eip). If set, radare will set seek position to its value on startup.

### cfg.fortunes

Enables or disables "fortune" messages displayed at each radare start.

### cfg.fortunes.type

Fortunes are classified by type. This variable determines which types are allowed for displaying when `cfg.fortunes` is `true`, so they can be fine-tuned on what's appropriate for the intended audience. Current types are `tips`, `fun`, `nsfw`, `creepy`.
