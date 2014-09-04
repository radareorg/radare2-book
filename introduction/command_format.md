## Command format

The general format for commands is as follows:

    [.][times][cmd][~grep][@[@iter]addr!size][|>pipe] ; ...

Commands are identified by a single character [a-zA-Z]. To repeatedly execute a command, prefix the command with a number:

    px    # run px
    3px   # run px 3 times

The `!` prefix is used to execute a command in shell context. If a single exclamation is used, commands will be sent to the system() hook defined in the currently-loaded I/O plug-in. This is used, for example, in the ptrace I/O plug-in, which accepts debugger commands from this interface.

Some examples:

    ds                    ; call the debugger's 'step' command
    px 200 @ esp          ; show 200 hex bytes at esp
    pc > file.c           ; dump buffer as a C byte array to file
    wx 90 @@ sym.*        ; write a nop on every symbol
    pd 2000 | grep eax    ; grep opcodes that use the 'eax' register
    px 20 ; pd 3 ; px 40  ; multiple commands in a single line

The `@` character is used to specify a temporary offset at which the command to its left will be executed. For example, `pd 5 @ 0x100000fce` will start disassembling at address 0x100000fce.

The `~` character enables the internal grep function used to filter the output of any command. For example:

    pd 20~call            ; disassemble 20 instructions and grep for 'call'

You can either grep for columns or rows:

    pd 20~call:0          ; get first row
    pd 20~call:1          ; get second row
    pd 20~call[0]         ; get first column
    pd 20~call[1]         ; get second column

Or even combine them:

    pd 20~call[0]:0       ; grep the first column of the first row matching 'call'

The internal grep function is a key feature for scripting radare, because it can be used to iterate over a list of offsets or data processed from disassembly, ranges, or any other command. Refer to the macros section (iterators) for more information.
