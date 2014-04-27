## Command format

The general format for commands looks something like this:

    [.][times][cmd][~grep][@[@iter]addr!size][|>pipe] ; ...

Commands are identified by a single character [a-zA-Z]. To repeatedly execute a command, simply prefix the command with a number.

    px    # run px
    3px   # run 3 times 'px'

The `!` prefix is used to execute a command in shell context. If a single exclamation is used, commands will be send to the system() hook defined in the currently loaded IO plugin. This is used, for example in the ptrace IO plugin which accepts debugger commands from this interface.

Some examples:

    ds                    ; call debugger 'step' command
    px 200 @ esp          ; show 200 hex bytes at esp
    pc > file.c           ; dump buffer as a C byte array to file
    wx 90 @@ sym.*        ; write a nop on every symbol
    pd 2000 | grep eax    ; grep opcodes using 'eax' register
    px 20 ; pd 3 ; px 40  ; multiple commands in a single line

The `@` character is used to specify a temporary offset at which the command to its left will be executed. 

The `~` character enables the internal grep function which can be used to filter the output of any command. The usage is quite simple:

    pd 20~call            ; disassemble 20 instructions and grep for 'call'

We can either grep for columns or rows:

    pd 20~call:0          ; get first row
    pd 20~call:1          ; get second row
    pd 20~call[0]         ; get first column
    pd 20~call[1]         ; get second column

Or even combine them:

    pd 20~call[0]:0       ; grep first column of the first row matching 'call'

The use of the internal grep function is a key feature for scripting radare, because it can be used to iterate over list of offsets or data processed from disassembly, ranges, or any other command. Here's an example of usage. See macros section (iterators) for more information.
