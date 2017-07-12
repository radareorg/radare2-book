# Radare2 Reference Card

## Survival Guide

| Command                 | Description                     |
|:------------------------|:--------------------------------|
| aa                      | Auto analyze                    |
| pdf@fcn(Tab)            | Disassemble function            |
| f fcn(Tab)              | List functions                  |
| f str(Tab)              | List strings                    |
| fr [flagname] [newname] | Rename flag                     |
| psz [offset]            | Print string                    |
| arf [flag]              | Find cross reference for a flag |


## Flagspaces



| Command       | Description           |
|:--------------|:----------------------|
| fs            | Display flagspaces    |
| fs *          | Select all flagspaces |
| fs [sections] | Select one flagspace  |


## Flags


| Command             | Description           |
|:--------------------|:----------------------|
| f                   | List flags            |
| fs *                | Select all flagspaces |
| fs [sections]       | Select one flagspace  |
| fj                  | Display flags in JSON |
| fl                  | Show flag length      |
| fx                  | Show hexdump of flag  |
| fC [name] [comment] | Set flag comment      |




## Information


| Command | Description            |
|:--------|:-----------------------|
| ii      | Information on imports |
| iI      | Info on binary         |
| ie      | Display entrypoint     |
| iS      | Display sections       |
| ir      | Display relocations    |



## Print string


| Command      | Description                    |
|:-------------|:-------------------------------|
| psz [offset] | Print zero terminated string   |
| psb [offset] | Print strings in current block |
| psx [offset] | Show string with scaped chars  |
| psp [offset] | Print pascal string            |
| psw [offset] | Print wide string              |


## Visual mode


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
| /*+-[]         | Change block size, [] = resize hex.cols           |
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
| /x ff0.23      | Search for hex string                         |
| /x ff..33      | Search for hex string ignoring some nibbles   |
| /x ff43 ffd0   | Search for hexpair with mask                  |
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


| Command   | Description              |
|:----------|:-------------------------|
| Po [file] | Open project             |
| Ps [file] | Save project             |
| Pi [file] | Show project information |



## Usable variables in expression

| Command | Description                                      |
|:--------|:-------------------------------------------------|
| $$      | Here (current virtual seek)                      |
| $o      | Here (current disk io offset)                    |
| $s      | File size                                        |
| $b      | Block size                                       |
| $w      | Get word size, 4 if asm.bits=32, 8 if 64         |
| $c,$r   | Get width and height of terminal                 |
| $S      | Section offset                                   |
| $SS     | Section size                                     |
| $j      | Jump address (jmp 0x10, jz 0x10 => 0x10)         |
| $f      | Jump fail address (jz 0x10 => next instruction)  |
| $I      | Number of instructions of current function       |
| $F      | Current function size                            |
| $Jn     | Get nth jump of function                         |
| $Cn     | Get nth call of function                         |
| $Dn     | Get nth data reference in function               |
| $Xn     | Get nth xref of function                         |
| $m      | Opcode memory reference (mov eax,[0x10] => 0x10) |
| $l      | Opcode length                                    |
| $e      | 1 if end of block, else 0                        |
| $ev     | Get value of eval config variable                |
| $?      | Last comparison value                            |


## License

This chapter is based on the Radare 2 reference card by Thanat0s, which is under the GNU GPL. Original license is as follows:
```
This card may be freely distributed under the terms of the GNU
general public licence — Copyright c  by Thanat0s - v0.1 -
```
