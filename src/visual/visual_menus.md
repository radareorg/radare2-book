## Visual Menus

There are a couple of keystrokes in Visual Mode that will lead to some menus with useful actions in a very accessible way. These are some of them:

* Edit bits and decompose an instruction (Vd1)
* Change function signature (vvs)
* Rename current function (vdr)
* Set the current offset as string (Vds)
* Change base of the immediate in current instruction (Vdi)
* Vbc - browse classes

### Vv visual analysis

This visual mode can be used to navigate through the program like in the visual disassembly view, but having some extra visual modes to follow references, etc

```console
[0x00000000]> Vv
.-- functions ----- pdr ---------------------------------.
| (a)nalyze   (-)delete (x)xrefs (X)refs (j/k) next/prev |
| (r)ename    (c)alls   (d)efine (Tab)disasm (_) hud     |
| (d)efine    (v)ars    (?)help  (:)shell    (q) quit    |
| (s)ignature                                            |
'--------------------------------------------------------'
```

Note that all those actions can be done through the `Vd` menu, as well as the commandline, and it is probably that most of these actions will end up being moved into the `Vd` for simplicity reasons and compatibility with Visual Mode and Visual Panels.

### Ve visual config

The `e` command is used to change any configuration option inside radare2, the visual mode have a visual version so you can use it to change settings interactively.

Note that you can also press the `R` key to quickly rotate between different the available color themes.

### Vd as in define

The Vd menu can be used to redefine information in the current function or instruction quickly.

This is the list of actions:

```console
[0x00000000]> Vd
[Vd]- Define current block as:                                                                   
 $    define flag size
 1    edit bits
 >    small integer (shift right by 1)
 a    assembly
 b    as byte (1 byte)
 B    define half word (16 bit, 2 byte size)
 c    as code (unset any data / string / format) in here
 C    define flag color (fc)
 d    set as data
 e    end of function
 E    esil debugger (aev)
 f    analyze function
 F    format
 h    define hint (for half-word, see 'B')
 i    (ahi) immediate base (b(in), o(ct), d(ec), h(ex), s(tr))
 I    (ahi1) immediate base (b(in), o(ct), d(ec), h(ex), s(tr))
 j    merge down (join this and next functions)
 k    merge up (join this and previous function)
 h    define anal hint
 m    manpage for current call
 n    rename flag or variable referenced by the instruction in cursor
 N    edit function signature (afs!)
 o    opcode string
 r    rename function
 s    set string
 S    set strings in current block
 t    set opcode type via aht hints (call, nop, jump, ...)
 u    undefine metadata here
 v    rename variable at offset that matches some hex digits
 x    find xrefs to current address (./r)
 X    find cross references /r
 w    set as 32bit word
 W    set as 64bit word
 q    quit menu
 z    zone flag
```

`d` can be used to change the type of data of the current block, several basic types/structures are available as well as more advanced one using `pf` template:

```
d → ...
0x004048f7      48c1e83f       shr rax, 0x3f
d → b
0x004048f7 .byte 0x48
d → B
0x004048f7 .word 0xc148
d → d
0x004048f7 hex length=165 delta=0
0x004048f7  48c1 e83f 4801 c648 d1fe 7415 b800 0000
...
```

To improve code readability you can change how radare2 presents numerical values in disassembly, by default most of disassembly display numerical value as hexadecimal. Sometimes you would like to view it as a decimal, binary or even custom defined constant. To change value format you can use `d` following by `i` then choose what base to work in, this is the equivalent to `ahi`:

```
d → i → ...
0x004048f7      48c1e83f       shr rax, 0x3f
d → i →  10
0x004048f7      48c1e83f       shr rax, 63
d → i →  2
0x004048f7      48c1e83f       shr rax, '?'
```
