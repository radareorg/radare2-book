## Comparing Bytes

For most generic reverse engineering tasks like finding the differences between two binary files, which bytes has changed, find differences in the graphs of the code analysis results, and other diffing operations you can just use radiff2:

```
$ radiff2 -h
```

Inside r2, the functionalities exposed by radiff2 are available with the `c` command.

`c` (short for "compare") allows you to compare arrays of bytes from different sources. The command accepts input in a number of formats and then compares it against values found at current seek position.

```
[0x00404888]> c?
Usage: c[?dfx] [argument]   # Compare
| c [string]               Compare a plain with escaped chars string
| c* [string]              Same as above, but printing r2 commands instead
| c1 [addr]                Compare 8 bits from current offset
| c2 [value]               Compare a word from a math expression
| c4 [value]               Compare a doubleword from a math expression
| c8 [value]               Compare a quadword from a math expression
| cat [file]               Show contents of file (see pwd, ls)
| cc [at]                  Compares in two hexdump columns of block size
| ccc [at]                 Same as above, but only showing different lines
| ccd [at]                 Compares in two disasm columns of block size
| ccdd [at]                Compares decompiler output (e cmd.pdc=pdg|pdd)
| cf [file]                Compare contents of file at current seek
| cg[?] [o] [file]         Graphdiff current file and [file]
| cu[?] [addr] @at         Compare memory hexdumps of $$ and dst in unified diff
| cud [addr] @at           Unified diff disasm from $$ and given address
| cv[1248] [hexpairs] @at  Compare 1,2,4,8-byte (silent return in $?)
| cV[1248] [addr] @at      Compare 1,2,4,8-byte address contents (silent, return in $?)
| cw[?] [us?] [...]        Compare memory watchers
| cx [hexpair]             Compare hexpair string (use '.' as nibble wildcard)
| cx* [hexpair]            Compare hexpair string (output r2 commands)
| cX [addr]                Like 'cc' but using hexdiff output
| cd [dir]                 chdir
| cl|cls|clear             Clear screen, (clear0 to goto 0, 0 only)
```

To compare memory contents at current seek position against a given string of values, use `cx`:

```
[0x08048000]> p8 4
7f 45 4c 46

[0x08048000]> cx 7f 45 90 46
Compare 3/4 equal bytes
0x00000002 (byte=03)   90 ' '  ->  4c 'L'
[0x08048000]>
```

Another subcommand of the `c` command is `cc` which stands for "compare code".
To compare a byte sequence with a sequence in memory:

```
[0x4A13B8C0]> cc 0x39e8e089 @ 0x4A13B8C0
```

To compare contents of two functions specified by their names:

```
[0x08049A80]> cc sym.main2 @ sym.main
```

`c8` compares a quadword from the current seek (in the example below, 0x00000000) against a math expression:

```
[0x00000000]> c8 4

Compare 1/8 equal bytes (0%)
0x00000000 (byte=01)   7f ' '  ->  04 ' '
0x00000001 (byte=02)   45 'E'  ->  00 ' '
0x00000002 (byte=03)   4c 'L'  ->  00 ' '
```

The number parameter can, of course, be math expressions which use flag names and anything allowed in an expression:

```
[0x00000000]> cx 7f469046

Compare 2/4 equal bytes
0x00000001 (byte=02)   45 'E'  ->  46 'F'
0x00000002 (byte=03)   4c 'L'  ->  90 ' '
```

You can use the compare command to find differences between a current block and a file previously dumped to a disk:

```
r2 /bin/true
[0x08049A80]> s 0
[0x08048000]> cf /bin/true
Compare 512/512 equal bytes
```
