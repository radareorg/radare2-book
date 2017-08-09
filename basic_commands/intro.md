# Basic Commands

Most command names in radare are derived from action names. They should be easy to remember, as they are short. Actually, all commands are single letters. Subcommands or related commands are specified using the second character of command name. For example, `/ foo` is a command to search plain string, while `/x 90 90` is used to look for hexadecimal pairs.

The general format for a valid command (as explained in the 'Command Format' chapter) looks like this:

    [[.][times][cmd][~grep][@[@iter]addr!size][|>pipe] ; ...

For example,

    > 3s +1024    ; seeks three times 1024 from the current seek

If a command starts with `=!`, the rest of the string is passed to currently loaded IO plugin (a debugger, for example). Most plugins provide help messages with `=!?` or `=!help`.

    $ r2 -d /bin/ls
    > =!help      ; handled by the IO plugin

If a commands starts with `!`, posix_system() is called to pass the command to your shell. Check `!?` for more options and usage examples.

    > !ls         ; run `ls` in the shell

The meaning of arguments (iter, addr, size) depends on the specific command. As a rule of thumb, most commands take a number as an argument to specify number of bytes to work with, instead of currently defined block size. Some commands accept math expressions, or strings.

    > px 0x17     ; show 0x17 bytes in hexa at current seek
    > s base+0x33 ; seeks to flag 'base' plus 0x33
    > / lib       ; search for 'lib' string.
The `@` sign is used to specify a temporary offset location or seek position at which the command is executed, instead of current seek position. This is quite useful as you don't have to seek around all the time.

    > p8 10 @ 0x4010  ; show 10 bytes at offset 0x4010
    > f patata @ 0x10 ; set 'patata' flag at offset 0x10
Using `@@` you can execute a single command on a list of flags matching the glob. You can think of this as a foreach operation:

    > s 0
    > / lib             ; search 'lib' string
    > p8 20 @@ hit0_*   ; show 20 hexpairs at each search hit

The `>` operation is used to redirect output of a command into a file (overwriting it if it already exists).

    > pr > dump.bin   ; dump 'raw' bytes of current block to file named 'dump.bin'
    > f  > flags.txt  ; dump flag list to 'flags.txt'

The `|` operation (pipe) is similar to what you are used to expect from it in a *NIX shell: us output of one command as input to another.

    [0x4A13B8C0]> f | grep section | grep text
    0x0805f3b0 512 section._text
    0x080d24b0 512 section._text_end

You can pass several commands in a single line by separating them with semicolon `;`:

    > px ; dr
