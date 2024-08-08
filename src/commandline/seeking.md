## Seeking

To move around the file we are inspecting we will need to change the offset at which we are using the `s` command.

The argument is a math expression that can contain flag names, parenthesis, addition, subtraction, multiplication of immediates of contents of memory using brackets.

Some example commands:

```console
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

```console
[0x00000000]> s $$+4
```

From the debugger (or when emulating) we can also use the register names as references. They are loaded as flags with the `.dr*` command, which happens under the hood.

```console
[0x00000000]> s rsp+0x40
```

Here's the full help of the `s` command. We will explain in more detail below.

```console
[0x00000000]> s?
Usage: s   # Help for the seek commands. See ?$? to see all variables
| s               print current address
| s addr          seek to address
| s.[?]hexoff     seek honoring a base from core->offset
| s:pad           print current address with N padded zeros (defaults to 8)
| s-              undo seek
| s-*             reset undo seek history
| s- n            seek n bytes backward
| s--[n]          seek blocksize bytes backward (/=n)
| s+              redo seek
| s+ n            seek n bytes forward
| s++[n]          seek blocksize bytes forward (/=n)
| s[j*=!]         list undo seek history (JSON, =list, *r2, !=names, s==)
| s/ DATA         search for next occurrence of 'DATA' (see /?)
| s/x 9091        search for next occurrence of \x90\x91
| sa ([+-]addr)   seek to block-size aligned address (addr=$$ if not specified)
| sb ([addr])     seek to the beginning of the basic block
| sC[?] string    seek to comment matching given string
| sd ([addr])     show delta seek compared to all possible reference bases
| sf              seek to next function (f->addr+f->size)
| sf function     seek to address of specified function
| sf.             seek to the beginning of current function
| sfp             seek to the function prelude checking back blocksize bytes
| sff             seek to the nearest flag backwards (uses fd and ignored the delta)
| sg/sG           seek begin (sg) or end (sG) of section or file
| sh              open a basic shell (aims to support basic posix syntax)
| sl[?] [+-]line  seek to line
| sn/sp ([nkey])  seek to next/prev location, as specified by scr.nkey
| snp             seek to next function prelude
| spp             seek to prev function prelude
| so ([[-]N])     seek to N opcode(s) forward (or backward when N is negative), N=1 by default
| sr PC           seek to register (or register alias) value
| ss[?]           seek silently (without adding an entry to the seek history)
| sort [file]     sort the contents of the file

> 3s++        ; 3 times block-seeking
> s 10+0x80   ; seek at 0x80+10
```

If you want to inspect the result of a math expression, you can evaluate it using the `?` command. Simply pass the expression as an argument. The result can be displayed in hexadecimal, decimal, octal or binary formats.

```console
> ? 0x100+200
0x1C8 ; 456d ; 710o ; 1100 1000
```

There are also subcommands of `?` that display the output in one specific format (base 10, base 16 ,...). See `?v` and `?vi`.

In the visual mode, you can press `u` (undo) or `U` (redo) inside the seek history to return back to previous or forward to the next location.

### Open file

As a test file, let's use a simple `hello_world.c` compiled in Linux ELF format.
After we compile it let's open it with radare2:

```console
$ r2 hello_world
```

Now we have the command prompt:

```console
[0x00400410]>
```

And it is time to go deeper.

### Seeking at any position

All seeking commands that take an address as a command parameter can use any numeral base
such as hex, octal, binary or decimal.

Seek to an address 0x0. An alternative command is simply `0x0`

```console
[0x00400410]> s 0x0
[0x00000000]>
```

Print current address:

```console
[0x00000000]> s
0x0
[0x00000000]>
```

There is an alternate way to print current position: `?v $$`.

Seek N positions forward, space is optional:

```console
[0x00000000]> s+ 128
[0x00000080]>
```

Undo last two seeks to return to the initial address:

```console
[0x00000080]> s-
[0x00000000]> s-
[0x00400410]>
```

We are back at _0x00400410_.

There's also a command to show the seek history:

```console
[0x00400410]> s*
f undo_3 @ 0x400410
f undo_2 @ 0x40041a
f undo_1 @ 0x400410
f undo_0 @ 0x400411
# Current undo/redo position.
f redo_0 @ 0x4005b4
```

### Partial Seeks

Another important `s` subcommand is the `s..` one which permits to seek to another address taking the higher nibbles of the current address as reference, this technique works great for kernel, aslr or large binaries where you really don't want to type different or large numbers everytime.

```console
[0x100003a84]> s..00
[0x100003a00]> s..3b00
[0x100003b00]> s..0000
[0x100000000]> s 0
[0x00000000]>
```

### Dereferencing pointers

Sometimes programs store pointers in memory, these needs to be derefenced in order to follow them and see what are they pointing at.

The r2 shell provides many ways to do this, that's because reading pointers from memory can be tricky and powerful it gives the power to the user to do it in the way that works the best for them.

Use the `*` command, which acts like in C. It reads the the value from the given address (`$$` stands for current seek) and honors `asm.bits` and `cfg.bigendian`.

```console
[0x004000c8]> s `*$$`
[0x0040032e]>
```

This is the help message from the * command, which can be used as an alias for `wv` to write a value or to read from memory like the brackets syntax would do on any math expression in r2:

```console
[0x100003a84]> *
Usage: *<addr>[=[0x]value]  Pointer read/write data/values
| *entry0=cc           write trap in entrypoint
| *entry0+10=0x804800  write value in delta address
| *entry0              read byte at given address
| */                   end multiline comment. (use '/*' to start mulitiline comment
[0x100003a84]>
```

Note that * can be also expressed as a math expression using the brackets syntax:

```console
[0x100003a84]> ?v [$$]
0xa9ba6ffcd503237f
[0x100003a84]>
```

Alternatively you can always use data analysis (`ad`) and the periscoped hexdump (`pxr`) to analyze linked lists, nested structures, pointers and more!

There are several more commands and features to follow and analyze pointers:

* aav : analyze all values pointing to code or data in a given range
* aaw : analyze all meta words (defined by Cd) as pointers to code
* pdp : disassemble following pointers in stack for ropchain gadgets
* pxw/pxq : word/qword hexdumps
* ahp : set pointer hints for analysis
* :iP : diversity pointer infromation from
* pxt : delta pointer table dumping, handy for manual switch table analysis
