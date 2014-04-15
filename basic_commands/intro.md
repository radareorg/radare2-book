# Basic commands

The basic set of commands in radare can be mostly grouped by action, and they should be easy to remember and short. This is why they are grouped with a single character, subcommands or related commands are described with a second character. For example `/ foo` for searching plain strings or `/x 90 90` to look for hexpair strings.

The format of the commands (as explained in 'Command format' chapter) looks something like that:

    [[.][times][cmd][~grep][@[@iter]addr!size][|>pipe] ; ...

    > 3s +1024    ; seeks three times 1024 from the current seek
    
If the command starts with `!` the string is passed to the plugin hadling the current IO (the debugger for example), if no one handles it calls to posix_system() which is a shell escape, you can prefix the command with two `!!`.

    > !help       ; handled by the debugger or shell
    > !!ls        ; runs ls in the shell
The [arg] argument depends on the command, but most of them take a number as argument to specify the number of bytes to work on instead of block size. Other commands accept math expressions, or strings.

    > px 0x17     ; show 0x17 bytes in hexa at current seek
    > s base+0x33 ; seeks to flag 'base' plus 0x33
    > / lib       ; search for 'lib' string.
The `@` is used to specify a temporal seek where the command is executed. This is quite useful to not seeking all the time.

    > p8 10 @ 0x4010  ; show 10 bytes at offset 0x4010
    > f patata @ 0x10 ; set 'patata' flag at offset 0x10
Using `@@` you can execute a single command on a list of flags matching the glob:

    > s 0
    > / lib             ; search 'lib' string
    > p8 20 @@ hit0_*   ; show 20 hexpairs at each search hit
    
The `>` is used to pipe the output of the command to a file (truncating to 0 if exist)

    > pr > dump.bin   ; dump 'raw' bytes of current block to 'dump.bin' file
    > f  > flags.txt  ; dump flag list to 'flags.txt'
    
The `|` pipe is used to dump the output of the command to another program.

    [0x4A13B8C0]> f | grep section | grep text
    0x0805f3b0 512 section._text
    0x080d24b0 512 section._text_end
    
Using the `;` chars you can concatenate multiple commands in a single line:

    > px ; dr
