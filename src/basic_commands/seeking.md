## Seeking

To move around the file we are inspecting we will need to change the offset at which we are using the `s` command.

The argument is a math expression that can contain flag names, parenthesis, addition, substraction, multiplication of immediates of contents of memory using brackets.

Some example commands:

```
[0x00000000]> s 0x10
[0x00000010]> s+4
[0x00000014]> s-
[0x00000010]> s+
[0x00000014]>
```

Observe how the prompt offset changes. The first line moves the current offset to the address 0x10.

The second does a relative seek 4 bytes forward.

And finally, the last 2 commands are undoing, and redoing the last seek operations.

Instead of using just numbers, we can use complex expressions, or basic arithmetic operations to represent the address to seek.

To do this, check the ?$? Help message which describes the internal variables that can be used in the expressions. For example, this is the same as doing s+4 .

```
[0x00000000]> s $$+4
```

From the debugger (or when emulating) we can also use the register names as references. They are loaded as flags with the `.dr*` command, which happens under the hood.

```
[0x00000000]> s rsp+0x40
```

Here's the full help of the `s` command. We will explain in more detail below.

```
[0x00000000]> s?
Usage: s    # Help for the seek commands. See ?$? to see all variables
| s                 Print current address
| s.hexoff          Seek honoring a base from core->offset
| s:pad             Print current address with N padded zeros (defaults to 8)
| s addr            Seek to address
| s-                Undo seek
| s-*               Reset undo seek history
| s- n              Seek n bytes backward
| s--[n]            Seek blocksize bytes backward (/=n)
| s+                Redo seek
| s+ n              Seek n bytes forward
| s++[n]            Seek blocksize bytes forward (/=n)
| s[j*=!]           List undo seek history (JSON, =list, *r2, !=names, s==)
| s/ DATA           Search for next occurrence of 'DATA'
| s/x 9091          Search for next occurrence of \x90\x91
| sa [[+-]a] [asz]  Seek asz (or bsize) aligned to addr
| sb                Seek aligned to bb start
| sC[?] string      Seek to comment matching given string
| sf                Seek to next function (f->addr+f->size)
| sf function       Seek to address of specified function
| sf.               Seek to the beginning of current function
| sg/sG             Seek begin (sg) or end (sG) of section or file
| sl[?] [+-]line    Seek to line
| sn/sp ([nkey])    Seek to next/prev location, as specified by scr.nkey
| so [N]            Seek to N next opcode(s)
| sr pc             Seek to register
| ss                Seek silently (without adding an entry to the seek history)

> 3s++        ; 3 times block-seeking
> s 10+0x80   ; seek at 0x80+10
```

If you want to inspect the result of a math expression, you can evaluate it using the `?` command. Simply pass the expression as an argument. The result can be displayed in hexadecimal, decimal, octal or binary formats.

```
> ? 0x100+200
0x1C8 ; 456d ; 710o ; 1100 1000
```

There are also subcommands of `?` that display the output in one specific format (base 10, base 16 ,...). See `?v` and `?vi`.

In the visual mode, you can press `u` (undo) or `U` (redo) inside the seek history to return back to previous or forward to the next location.

## Open file

As a test file, let's use a simple `hello_world.c` compiled in Linux ELF format.
After we compile it let's open it with radare2:

```
$ r2 hello_world
```

Now we have the command prompt:

```
[0x00400410]>
```

And it is time to go deeper.

## Seeking at any position

All seeking commands that take an address as a command parameter can use any numeral base
such as hex, octal, binary or decimal.

Seek to an address 0x0. An alternative command is simply `0x0`

```
[0x00400410]> s 0x0
[0x00000000]>
```

Print current address:
```
[0x00000000]> s
0x0
[0x00000000]>
```

There is an alternate way to print current position: `?v $$`.

Seek N positions forward, space is optional:

```
[0x00000000]> s+ 128
[0x00000080]>
```

Undo last two seeks to return to the initial address:

```
[0x00000080]> s-
[0x00000000]> s-
[0x00400410]>
```

We are back at _0x00400410_.

There's also a command to show the seek history:

```
[0x00400410]> s*
f undo_3 @ 0x400410
f undo_2 @ 0x40041a
f undo_1 @ 0x400410
f undo_0 @ 0x400411
# Current undo/redo position.
f redo_0 @ 0x4005b4
```

