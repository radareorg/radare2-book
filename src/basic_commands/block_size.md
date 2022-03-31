## Block Size

The block size determines how many bytes radare2 commands will process when not given an explicit size argument. You can temporarily change the block size by specifying a numeric argument to the print commands. For example `px 20`.

```
[0x00000000]> b?
Usage: b[f] [arg]  # Get/Set block size
| b 33     set block size to 33
| b eip+4  numeric argument can be an expression
| b        display current block size
| b+3      increase blocksize by 3
| b-16     decrease blocksize by 16
| b*       display current block size in r2 command
| bf foo   set block size to flag size
| bj       display block size information in JSON
| bm 1M    set max block size
```

The `b` command is used to change the block size:

```
[0x00000000]> b 0x100   # block size = 0x100
[0x00000000]> b+16      #  ... = 0x110
[0x00000000]> b-32      #  ... = 0xf0
```

The `bf` command is used to change the block size to value specified by a flag. For example, in symbols, the block size of the flag represents the size of the function. To make that work, you have to either run function analysis `af` (which is included in `aa`) or manually seek and define some functions e.g. via `Vd`.

```
[0x00000000]> bf sym.main    # block size = sizeof(sym.main)
[0x00000000]> pD @ sym.main  # disassemble sym.main
```

You can combine two operations in a single `pdf` command. Except that `pdf` neither uses nor affects global block size.

```
[0x00000000]> pdf @ sym.main  # disassemble sym.main
```

Another way around is to use special variables `$FB` and `$FS` which denote Function's Beginning and Size at the current seek. Read more about [Usable variables](../refcard/intro.md#usable-variables-in-expression).

```
[0x00000000]> s sym.main + 0x04
[0x00001ec9]> pD @ $FB !$FS  # disassemble current function
╭ 211: int main (int argc, char **argv, char **envp);
|           0x00001ec5      55                 push rbp
|           0x00001ec6      4889e5             mov rbp, rsp
|           0x00001ec9      4881ecc0000000     sub rsp, 0xc0
...
╰           0x00001f97      c3                 ret
```

Note: don't put space after `!` size designator. See also [Command Format](../first_steps/command_format.md).
