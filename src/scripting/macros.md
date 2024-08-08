## Macros

Apart from simple sequencing and looping, radare2 allows to write
simple macros, using this construction:

```console
[0x00404800]> (qwe; pd 4; ao)
```

This will define a macro called 'qwe' which runs sequentially first 'pd 4' then 'ao'.
Calling the macro using syntax `.(macro)` is simple:

```console
[0x00404800]> (qwe; pd 4; ao)
[0x00404800]> .(qwe)
0x00404800  mov eax, 0x61e627      ; "tab"
0x00404805  push rbp
0x00404806  sub rax, section_end.LOAD1
0x0040480c  mov rbp, rsp

address: 0x404800
opcode: mov eax, 0x61e627
prefix: 0
bytes: b827e66100
ptr: 0x0061e627
refptr: 0
size: 5
type: mov
esil: 6415911,rax,=
stack: null
family: cpu
[0x00404800]>
```

To list available macroses simply call `(*`:

```console
[0x00404800]> (*
(qwe ; pd 4; ao)
```

And if want to remove some macro, just add '-' before the name:

```console
[0x00404800]> (-qwe)
Macro 'qwe' removed.
[0x00404800]>
```

Moreover, it's possible to create a macro that takes arguments, which comes in handy in some
simple scripting situations. To create a macro that takes arguments you simply add them to macro definition.

```console
[0x00404800]
[0x004047d0]> (foo x y; pd $0; s +$1)
[0x004047d0]> .(foo 5 6)
;-- entry0:
0x004047d0      xor ebp, ebp
0x004047d2      mov r9, rdx
0x004047d5      pop rsi
0x004047d6	mov rdx, rsp
0x004047d9	and rsp, 0xfffffffffffffff0
[0x004047d6]>
```

As you can see, the arguments are named by index, starting from 0: $0, $1, ...

### Aliases

The command to create, manage and run command aliases is the `$`. This is also the prefix used for aliases files, this chapter will dig

* Variable (like flags)
* Commands (simplest macros)
* Files (in-memory virtual files)

You can find some interesting details and examples by checking the help message:

``` console
[0x100003a84]> $?
Usage: $alias[=cmd] [args...]  Alias commands and data (See ?$? for help on $variables)
| $                        list all defined aliases
| $*                       list all defined aliases and their values, with unprintable characters escaped
| $**                      same as above, but if an alias contains unprintable characters, b64 encode it
| $foo:=123                alias for 'f foo=123'
| $foo-=4                  alias for 'f foo-=4'
| $foo+=4                  alias for 'f foo+=4'
| $foo                     alias for 's foo' (note that command aliases can override flag resolution)
| $dis=base64:AAA=         alias $dis to the raw bytes from decoding this base64 string
| $dis=$hello world        alias $dis to the string after '$'
| $dis=$hello\\nworld\\0a  string aliases accept double-backslash and hex escaping
| $dis=-                   edit $dis in cfg.editor (use single-backslashes for escaping)
| $dis=af                  alias $dis to the af command
| "$dis=af;pdf"            alias $dis to run af, then pdf. you must quote the whole command.
| $test=. /tmp/test.js     create command - rlangpipe script
| $dis=                    undefine alias
| $dis                     execute a defined command alias, or print a data alias with unprintable characters escaped
| $dis?                    show commands aliased by $dis
[0x100003a84]>
```

#### Command Aliases

The general usage of the feature is: `$alias=cmd`

```console
[0x00404800]> $disas=pdf
```

The above command will create an alias `disas` for `pdf`. The following command prints the disassembly of the main function.

```console
[0x00404800]> $disas @ main
```

Apart from commands, you can also alias a text to be printed, when called.

```console
[0x00404800]> $my_alias=$test input
[0x00404800]> $my_alias
test input
```

To undefine alias, use `$alias=`:

```console
[0x00404800]> $pmore='b 300;px'
[0x00404800]> $
$pmore
[0x00404800]> $pmore=
[0x00404800]> $

```

A single `$` in the above will list all defined aliases. It's also possible check the aliased command of an alias:

```console
[0x00404800]> $pmore?
b 200; px
```

Can we create an alias contains alias ? The answer is yes:

```console
[0x00404800]> $pStart='s 0x0;$pmore'
[0x00404800]> $pStart
- offset -   0 1  2 3  4 5  6 7  8 9  A B  C D  E F  0123456789ABCDEF
0x00000000  7f45 4c46 0201 0100 0000 0000 0000 0000  .ELF............
0x00000010  0300 3e00 0100 0000 1014 0000 0000 0000  ..>.............
0x00000020  4000 0000 0000 0000 5031 0000 0000 0000  @.......P1......
0x00000030  0000 0000 4000 3800 0d00 4000 1e00 1d00  ....@.8...@.....
0x00000040  0600 0000 0400 0000 4000 0000 0000 0000  ........@.......
0x00000050  4000 0000 0000 0000 4000 0000 0000 0000  @.......@.......
0x00000060  d802 0000 0000 0000 d802 0000 0000 0000  ................
0x00000070  0800 0000 0000 0000 0300 0000 0400 0000  ................
0x00000080  1803 0000 0000 0000 1803 0000 0000 0000  ................
0x00000090  1803 0000 0000 0000 1c00 0000 0000 0000  ................
0x000000a0  1c00 0000 0000 0000 0100 0000 0000 0000  ................
0x000000b0  0100 0000 0400 0000 0000 0000 0000 0000  ................
0x000000c0  0000 0000 0000 0000                      ........
[0x00000000]> 
```

#### File Aliases

If a file accessed from the repl starts with the dollar sign it will be treated as a virtual one that lives in memory only for the current session of radare2. They are handy when you don't want to depend on the filesystem to create or read files. For example in webassembly environments or other sandboxed executions.

```console
[0x00000000]> echo hello > $world
[0x00000000]> cat $world
hello
[0x00000000]>
[0x00000000]> $
$world
[0x00000000]> $world
hello
[0x00000000]> rm $world
[0x00000000]> $
[0x00000000]>
```

#### Variable Aliases

This is a short syntax for accessing and modifying flags, use them as numeric variables.

```console
[0x00000000]> $foo:=4
[0x00000000]> s foo
[0x00000004]> $foo+=4
[0x00000004]> s foo
[0x00000008]>
```
