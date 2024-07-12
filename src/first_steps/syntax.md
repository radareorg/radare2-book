## Command Syntax

In a single line we can describe the syntax of the radare2 commands like this: 

```
 .- ignore special characters, same as full command quotes "?e hi > ho"
 |    .- interpret the output of the command or run a script `.?`
 |   /   .-- the repeat prefix operator, run a command N times   .------------.
 |  |   /     . the command to run                               | , = csv    |
 |  |  |    /    . command suffix output modifier --------------<  j = json   |
 |  |  |   |    /                                                | * = r2cmds |
 |  |  |   |   |                                                 `------------'
['][.][N][cmd[,?*j]][~filter][@[@[@]]addr!size][|>pipe] ; another command
                           |  | | |  \________/ | |     |
                           |  | | |      |      |  \    `--- cmd separator
 output filter modifier _.'  /  | |      |       \  `-- redirect to file
   @ temporal seek  --------'  /  |       \       `--- pipe to system shell
       @@ foreach operator ---'   |        `--- foreach modifiers @? @@? @@@?
                                   `--- advanced foreach (addr+size on items)
```

People who use Vim daily and are familiar with its commands will find themselves at home. You will see this format used throughout the book. Commands are identified by a single case-sensitive character [a-zA-Z].

As an exercise for the reader you may want to read the following lines and understand the purpose of the syntax with examples.

```
ds                    ; call the debugger's 'step' command
px 200 @ esp          ; show 200 hex bytes at esp
pc > file.c           ; dump buffer as a C byte array to file.c
wx 90 @@ sym.*        ; write a nop on every symbol
pd 2000 | grep eax    ; grep opcodes that use the 'eax' register
px 20 ; pd 3 ; px 40  ; multiple commands in a single line
```

### Repetitions

To repeatedly execute a command, prefix the command with a number:

```
px    # run px
3px   # run px 3 times
```

An useful way to use this command is to draw the classic donut animation with `100?3d` or perform an specific amount of steps when debugging like: `10ds` (that will do the same as `ds 10`

### Shell Execution

The `!` prefix is used to execute a command in shell context. If you want to use the cmd callback from the I/O plugin you must prefix with `:`.

Note that a single exclamation mark will run the command and print the output through the RCons API. This means that the execution will be blocking and not interactive. Use double exclamation marks -- `!!` -- to run a standard system call.

All the socket, filesystem and execution APIs can be restricted with the `cfg.sandbox` configuration variable.

### Environment

When executing system commands from radare2, we will get some special environment variables that can be used to script radare2 from shellscripts without the need to depend on r2pipe.

The environment variables can be listed and modified with the `%` command.

Note that the environment variables will be different depending on how we execute code with radare2:

* runtime environment (R2CORE tells where the instance is in memory)
* debugger environment (as clean as described in a rarun2 profile)
* spawning processes with ! (get some context details, like offset, file, ..)
* r2pipe environment (`R2PIPE_IN` and `R2PIPE_OUT` with the pipe descriptors)

```
[0x00000000]> !export | grep R2_
export R2_ARCH="arm"
export R2_BITS="64"
export R2_BSIZE="256"
export R2_COLOR="0"
export R2_DEBUG="0"
export R2_ENDIAN="little"
export R2_FILE="malloc://512"
export R2_IOVA="1"
export R2_OFFSET="0"
export R2_SIZE="512"
export R2_UTF8="1"
export R2_XOFFSET="0x00000000"
[0x00000000]>
```

We can also find the location in memory of the RCore instance in the current process. This can be useful when injecting code inside radare2 (like when injecting r2 via r2frida or using native api calls on live runtimes without having to pass pointers or depend on RLang setups) We may learn more about this in the scripting chapter.

```
[0x00000000]> %~R2
R2CORE=0x140018000
[0x00000000]>
```

### Pipes

The standard UNIX pipe `|` is also available in the radare2 shell. You can use it to filter the output of an r2 command with any shell program that reads from stdin, such as `grep`, `less`, `wc`. If you do not want to spawn anything, or you can't, or the target system does not have the basic UNIX tools you need (Windows or embedded users), you can also use the built-in grep (`~`).

### Filtering

The `~` is a special character that is used by the console filtering features. It can be chained multiple times to perform multiple filters like grepping, xml or json indentation, head/tail operations, select column of output, etc

You may find that `~` is very similar to using the unix `|` pipe, but this 

As you may expect appending a question mark will display the help message.

```console
[0x00000000]> ~?
Usage: [command]~[modifier][word,word][endmodifier][[column]][:line]
modifier:
|  &            all words must match to grep the line
|  $[n]         sort numerically / alphabetically the Nth column
|  $            sort in alphabetic order
|  $$           sort + uniq
|  $!           inverse alphabetical sort
|  $!!          reverse the lines (like the `tac` tool)
|  ,            token to define another keyword
|  +            case insensitive grep (grep -i)
|  *            zoom level
|  ^            words must be placed at the beginning of line
|  !            negate grep
|  ?            count number of matching lines
|  ?.           count number chars
|  ??           show this help message
|  ?ea          convert text into seven segment style ascii art
|  :s..e        show lines s-e
|  ..           internal 'less'
|  ...          internal 'hud' (like V_)
|  ....         internal 'hud' in one line
|  :)           parse C-like output from decompiler
|  :))          code syntax highlight
|  <50          perform zoom to the given text width on the buffer
|  <>           xml indentation
|  {:           human friendly indentation (yes, it's a smiley)
|  {:..         less the output of {:
|  {:...        hud the output of {:
|  {}           json indentation
|  {}..         less json indentation
|  {}...        hud json indentation
|  {=}          gron-like output (key=value)
|  {path}       json path grep
endmodifier:
|  $            words must be placed at the end of line
column:
|  [n]          show only column n
|  [n-m]        show column n to m
|  [n-]         show all columns starting from column n
|  [i,j,k]      show the columns i, j and k
Examples:
|  i~:0         show first line of 'i' output
|  i~:-2        show the second to last line of 'i' output
|  i~:0..3      show first three lines of 'i' output
|  pd~mov       disasm and grep for mov
|  pi~[0]       show only opcode
|  i~0x400$     show lines ending with 0x400
```

The `~` character enables internal grep-like function used to filter output of any command:

```
pd 20~call            ; disassemble 20 instructions and grep output for 'call'
```

Additionally, you can grep either for columns or for rows:

```
pd 20~call:0          ; get first row
pd 20~call:1          ; get second row
pd 20~call[0]         ; get first column
pd 20~call[1]         ; get second column
```

Or even combine them:

```
pd 20~call:0[0]       ; grep the first column of the first row matching 'call'
```

This internal grep function is a key feature for scripting radare2,
because it can be used to iterate over a list of offsets or data generated by disassembler,
ranges, or any other command. Refer to the [loops](../scripting/loops.md) section (iterators) for more information.

### Output Evaluation

The `.` character at the begining of the command is used to interpret or evaluate the output of the command you execute.

The purpose of this syntax rings some bells when you use the `*` suffix or the `-r` flag in all the r2 shell commands.

For example, we can load the symbols from a binary in disk by running the following line:

```
> .!rabin2 -rs $R2_FILE
```

### Temporal Seek

The `@` character is used to specify a temporary offset at which the command to its left will be executed.
The original seek position in a file is then restored.

For example, `pd 5 @ 0x100000fce` to disassemble 5 instructions at address 0x100000fce.

Most of the commands offer autocompletion support using `<TAB>` key, for example `s`eek or `f`lags commands.

It offers autocompletion using all possible values, taking flag names in this case.

The command history can be interactively inspected with `!~...`.

To extend the autocompletion support to handle more commands or enable autocompletion to your own commands defined in core, I/O plugins you must use the `!!!` command.
