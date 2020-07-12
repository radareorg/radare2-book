## Flags

Flags are conceptually similar to bookmarks. They associate a name with a given offset in a file. Flags can be grouped into 'flag spaces'. A flag space is a namespace for flags, grouping together flags of similar characteristics or type. Examples for flag spaces: sections, registers, symbols.

To create a flag:

```
[0x4A13B8C0]> f flag_name @ offset
```

You can remove a flag by appending the `-` character to command. Most commands accept `-` as argument-prefix as an indication to delete something.

```
[0x4A13B8C0]> f-flag_name
```

To switch between or create new flagspaces use the `fs` command:

```
[0x00005310]> fs?
|Usage: fs [*] [+-][flagspace|addr] # Manage flagspaces
| fs            display flagspaces
| fs*           display flagspaces as r2 commands
| fsj           display flagspaces in JSON
| fs *          select all flagspaces
| fs flagspace  select flagspace or create if it doesn't exist
| fs-flagspace  remove flagspace
| fs-*          remove all flagspaces
| fs+foo        push previous flagspace and set
| fs-           pop to the previous flagspace
| fs-.          remove the current flagspace
| fsq           list flagspaces in quiet mode
| fsm [addr]    move flags at given address to the current flagspace
| fss           display flagspaces stack
| fss*          display flagspaces stack in r2 commands
| fssj          display flagspaces stack in JSON
| fsr newname   rename selected flagspace
[0x00005310]> fs
0  439 * strings
1   17 * symbols
2   54 * sections
3   20 * segments
4  115 * relocs
5  109 * imports
[0x00005310]>
```

Here there are some command examples:

```
[0x4A13B8C0]> fs symbols ; select only flags in symbols flagspace
[0x4A13B8C0]> f          ; list only flags in symbols flagspace
[0x4A13B8C0]> fs *       ; select all flagspaces
[0x4A13B8C0]> f myflag   ; create a new flag called 'myflag'
[0x4A13B8C0]> f-myflag  ; delete the flag called 'myflag'
```

You can rename flags with `fr`.

### Local flags

Every flag name should be unique for addressing reasons. But it is quite a common need
to have the flags, for example inside the functions, with simple and ubiquitous names like `loop` or `return`. For this purpose you can use so called "local" flags, which are tied to the function where they reside. It is possible to add them using `f.` command:

```
[0x00003a04]> pd 10
│      0x00003a04      48c705c9cc21.  mov qword [0x002206d8], 0xffffffffffffffff ;
[0x2206d8:8]=0
│      0x00003a0f      c60522cc2100.  mov byte [0x00220638], 0     ; [0x220638:1]=0
│      0x00003a16      83f802         cmp eax, 2
│  .─< 0x00003a19      0f84880d0000   je 0x47a7
│  │   0x00003a1f      83f803         cmp eax, 3
│ .──< 0x00003a22      740e           je 0x3a32
│ ││   0x00003a24      83e801         sub eax, 1
│.───< 0x00003a27      0f84ed080000   je 0x431a
││││   0x00003a2d      e8fef8ffff     call sym.imp.abort           ; void abort(void)
││││   ; CODE XREF from main (0x3a22)
││╰──> 0x00003a32      be07000000     mov esi, 7
[0x00003a04]> f. localflag @ 0x3a32
[0x00003a04]> f.
0x00003a32 localflag   [main + 210]
[0x00003a04]> pd 10
│      0x00003a04      48c705c9cc21.  mov qword [0x002206d8], 0xffffffffffffffff ;
[0x2206d8:8]=0
│      0x00003a0f      c60522cc2100.  mov byte [0x00220638], 0     ; [0x220638:1]=0
│      0x00003a16      83f802         cmp eax, 2
│  .─< 0x00003a19      0f84880d0000   je 0x47a7
│  │   0x00003a1f      83f803         cmp eax, 3
│ .──< 0x00003a22      740e           je 0x3a32                    ; main.localflag
│ ││   0x00003a24      83e801         sub eax, 1
│.───< 0x00003a27      0f84ed080000   je 0x431a
││││   0x00003a2d      e8fef8ffff     call sym.imp.abort           ; void abort(void)
││││   ; CODE XREF from main (0x3a22)
││`──>  .localflag:
││││   ; CODE XREF from main (0x3a22)
││`──> 0x00003a32      be07000000     mov esi, 7
[0x00003a04]>
```

### Flag Zones

radare2 offers flag zones, which lets you label different offsets on the scrollbar, for making it easier to navigate through large binaries. You can set a flag zone on the current seek using:

```
[0x00003a04]> fz flag-zone-name
```

Set `scr.scrollbar=1` and go to the Visual mode, to see your flag zone appear on the scrollbar on the right end of the window.

See `fz?` for more information.