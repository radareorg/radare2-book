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
Usage: s[+-] [addr]
s                 print current address
s 0x320           seek to this address
s-                undo seek
s+                redo seek
s*                list undo seek history
s++               seek blocksize bytes forward
s--               seek blocksize bytes backward
s+ 512            seek 512 bytes forward
s- 512            seek 512 bytes backward
sg/sG             seek begin (sg) or end (sG) of section or file
s.hexoff          Seek honoring a base from core->offset
sa [[+-]a] [asz]  seek asz (or bsize) aligned to addr
sn/sp             seek next/prev scr.nkey
s/ DATA           search for next occurrence of 'DATA'
s/x 9091          search for next occurrence of \x90\x91
sb                seek aligned to bb start
so [num]          seek to N next opcode(s)
sf                seek to next function (f->addr+f->size)
sC str            seek to comment matching given string
sr pc             seek to register

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

