# Radare2 Reference Card

This chapter is based on the Radare 2 reference card by Thanat0s, which is under the GNU GPL. Original license is as follows:

```
This card may be freely distributed under the terms of the GNU
general public licence — Copyright by Thanat0s - v0.1 -
```

## Survival Guide

Those are the basic commands you will want to know and use for moving
around a binary and getting information about it.

| Command                 | Description                      |
|:------------------------|:---------------------------------|
| s (tab)                 | Seek to a different place        |
| x [nbytes]              | Hexdump of nbytes, $b by default |
| aa                      | Auto analyze                     |
| pdf@fcn(Tab)            | Disassemble function             |
| f fcn(Tab)              | List functions                   |
| f str(Tab)              | List strings                     |
| fr [flagname] [newname] | Rename flag                      |
| psz [offset]~grep       | Print strings and grep for one   |
| arf [flag]              | Find cross reference for a flag  |

## Flags

Flags are like bookmarks, but they carry some extra information like size, tags or associated flagspace. Use the `f` command to list, set, get them.

| Command             | Description           |
|:--------------------|:----------------------|
| f                   | List flags            |
| fd $$               | Describe an offset    |
| fj                  | Display flags in JSON |
| fl                  | Show flag length      |
| fx                  | Show hexdump of flag  |
| fC [name] [comment] | Set flag comment      |

## Flagspaces

Flags are created into a flagspace, by default none is selected,
and listing flags will list them all. To display a subset of flags
you can use the `fs` command to restrict it.

| Command       | Description           |
|:--------------|:----------------------|
| fs            | Display flagspaces    |
| fs *          | Select all flagspaces |
| fs [sections] | Select one flagspace  |

## Information

Binary files have information stored inside the headers. The `i`
command uses the RBin api and allows us to the same things rabin2
do. Those are the most common ones.

| Command | Description              |
|:--------|:-------------------------|
| ii      | Information on imports   |
| iI      | Info on binary           |
| ie      | Display entrypoint       |
| iS      | Display sections         |
| ir      | Display relocations      |
| iz      | List strings (izz, izzz) |

## Print string

There are different ways to represent a string in memory. The `ps` command
allows us to print it in utf-16, pascal, zero terminated, .. formats.

| Command      | Description                    |
|:-------------|:-------------------------------|
| psz [offset] | Print zero terminated string   |
| psb [offset] | Print strings in current block |
| psx [offset] | Show string with scaped chars  |
| psp [offset] | Print pascal string            |
| psw [offset] | Print wide string              |


## Visual mode

The visual mode is the standard interactive interface of radare2.

To enter in visual mode use the `v` or `V` command, and then you'll only
have to press keys to get the actions happen instead of commands.

| Command        | Description                                       |
|:---------------|:--------------------------------------------------|
| V              | Enter visual mode                                 |
| p/P            | Rotate modes (hex, disasm, debug, words, buf)     |
| c              | Toggle (c)ursor                                   |
| q              | Back to Radare shell                              |
| hjkl           | Move around (or HJKL) (left-down-up-right)        |
| Enter          | Follow address of jump/call                       |
| sS             | Step/step over                                    |
| o              | Go/seek to given offset                           |
| .              | Seek to program counter                           |
| /              | In cursor mode, search in current block           |
| :cmd           | Run radare command                                |
| ;[-]cmt        | Add/remove comment                                |
| x+-/[]         | Change block size, [] = resize hex.cols           |
| >&#124;&#124;< | Seek aligned to block size                        |
| i/a/A          | (i)nsert hex, (a)ssemble code, visual (A)ssembler |
| b/B            | Toggle breakpoint / automatic block size          |
| d[f?]          | Define function, data, code, ..                   |
| D              | Enter visual diff mode (set diff.from/to)         |
| e              | Edit eval configuration variables                 |
| f/F            | Set/unset flag                                    |
| gG             | Go seek to begin and end of file (0-$s)           |
| mK/’K          | Mark/go to Key (any key)                          |
| M              | Walk the mounted filesystems                      |
| n/N            | Seek next/prev function/flag/hit (scr.nkey)       |
| o              | Go/seek to given offset                           |
| C              | Toggle (C)olors                                   |
| R              | Randomize color palette (ecr)                     |
| t              | Track flags (browse symbols, functions..)         |
| T              | Browse anal info and comments                     |
| v              | Visual code analysis menu                         |
| V/W            | (V)iew graph (agv?), open (W)ebUI                 |
| uU             | Undo/redo seek                                    |
| x              | Show xrefs to seek between them                   |
| yY             | Copy and paste selection                          |
| z              | Toggle zoom mode                                  |


## Searching

There are many situations where we need to find a value inside a binary
or in some specific regions. Use the `e search.in=?` command to choose
where the `/` command may search for the given value.

| Command        | Description                                   |
|:---------------|:----------------------------------------------|
| / foo\00       | Search for string ’foo\0’                     |
| /b             | Search backwards                              |
| //             | Repeat last search                            |
| /w foo         | Search for wide string ’f\0o\0o\0’            |
| /wi foo        | Search for wide string ignoring case          |
| /! ff          | Search for first occurrence not matching      |
| /i foo         | Search for string ’foo’ ignoring case         |
| /e /E.F/i      | Match regular expression                      |
| /x a1b2c3      | Search for bytes; spaces and uppercase nibbles are allowed, same as /x A1 B2 C3|
| /x a1..c3      | Search for bytes ignoring some nibbles (auto-generates mask, in this example: ff00ff)|
| /x a1b2:fff3   | Search for bytes with mask (specify individual bits)|
| /d 101112      | Search for a deltified sequence of bytes      |
| /!x 00         | Inverse hexa search (find first byte != 0x00) |
| /c jmp [esp]   | Search for asm code (see search.asmstr)       |
| /a jmp eax     | Assemble opcode and search its bytes          |
| /A             | Search for AES expanded keys                  |
| /r sym.printf  | Analyze opcode reference an offset            |
| /R             | Search for ROP gadgets                        |
| /P             | Show offset of previous instruction           |
| /m magicfile   | Search for matching magic file                |
| /p patternsize | Search for pattern of given size              |
| /z min max     | Search for strings of given size              |
| /v[?248] num   | Look for a asm.bigendian 32bit value          |

## Saving

By default, when you open a file in write mode (`r2 -w`) all changes
will be written directly into the file. No undo history is saved by
default.

Use `e io.cache.write=true` and the `wc` command to manage the *write cache*
history changes. To undo, redo, commit them to write the changes on the file..

If, instead, we want to save the analysis information, comments, flags and
other user-created metadata, we may want to use projects with `r2 -p` and the `P` command.

| Command   | Description              |
|:----------|:-------------------------|
| Po [file] | Open project             |
| Ps [file] | Save project             |
| Pi [file] | Show project information |

## Usable variables in expression

The `?$?` command will display the variables that can be used in any math
operation inside the r2 shell. For example, using the `? $$` command to evaluate
a number or `?v` to just the value in one format.

All commands in r2 that accept a number supports the use of those variables.

| Command       | Description                                      |
|:--------------|:-------------------------------------------------|
| $$            | here (current virtual seek)|
| $$$           | current non-temporary virtual seek|
| $?            | last comparison value|
| $alias=value  | alias commands (simple macros)|
| $b            | block size|
| $B            | base address (aligned lowest map address)|
| $f            | jump fail address (e.g. jz 0x10 => next instruction)|
| $fl           | flag length (size) at current address (fla; pD $l @ entry0)|
| $F            | current function size|
| $FB           | begin of function|
| $Fb           | address of the current basic block|
| $Fs           | size of the current basic block|
| $FE           | end of function|
| $FS           | function size|
| $Fj           | function jump destination|
| $Ff           | function false destination|
| $FI           | function instructions|
| $c,$r         | get width and height of terminal|
| $Cn           | get nth call of function|
| $Dn           | get nth data reference in function|
| $D            | current debug map base address ?v $D @ rsp|
| $DD           | current debug map size|
| $e            | 1 if end of block, else 0|
| $j            | jump address (e.g. jmp 0x10, jz 0x10 => 0x10)|
| $Ja           | get nth jump of function|
| $Xn           | get nth xref of function|
| $l            | opcode length|
| $m            | opcode memory reference (e.g. mov eax,[0x10] => 0x10)|
| $M            | map address (lowest map address)|
| $o            | here (current disk io offset)|
| $p            | getpid()|
| $P            | pid of children (only in debug)|
| $s            | file size|
| $S            | section offset|
| $SS           | section size|
| $v            | opcode immediate value (e.g. lui a0,0x8010 => 0x8010)|
| $w            | get word size, 4 if asm.bits=32, 8 if 64, ...|
| ${ev}         | get value of eval config variable|
| $r{reg}       | get value of named register|
| $k{kv}        | get value of an sdb query value|
| $s{flag}      | get size of flag|
| RNum          | $variables usable in math expressions|
