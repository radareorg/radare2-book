# Radare2 Reference Card

## Survival Guide
<table>
<tr>
<td>Command</td>
<td>Description</td>
</tr>
<tr>
<td>aa</td>
<td>Auto analyze</td>
</tr>
<tr>
<td>pdf@fcn(Tab)</td>
<td>Disassemble function</td>
</tr>
<tr>
<td>f fcn(Tab)</td>
<td>List functions</td>
</tr>
<tr>
<td>f str(Tab)</td>
<td>List strings</td>
</tr>
<tr>
<td>fr [flagname] [newname]</td>
<td>Rename flag</td>
</tr>
<tr>
<td>psz [offset]</td>
<td>Print string</td>
</tr>
<tr>
<td>arf [flag]</td>
<td>Find cross reference for a flag</td>
</tr>
</table>

## Flagspaces

<table>
<tr>
<td>Command</td>
<td>Description</td>
</tr>
<tr>
<td>fs</td>
<td>Display flagspaces</td>
</tr>
<tr>
<td>fs *</td>
<td>Select all flagspaces</td>
</tr>
<tr>
<td>fs [sections]</td>
<td>Select one flagspace</td>
</tr>
</table>


## Flags

<table>
<tr>
<td>Command</td>
<td>Description</td>
</tr>
<tr>
<td>f</td>
<td>List flags</td>
</tr>
<tr>
<td>fj</td>
<td>Display flags in JSON</td>
</tr>
<tr>
<td>fl</td>
<td>Show flag length</td>
</tr>
<tr>
<td>fx</td>
<td>Show hexdump of flag</td>
</tr>
<tr>
<td>fC [name] [comment]</td>
<td>Set flag comment</td>
</tr>
</table>



## Information

<table>
<tr>
<td>Command</td>
<td>Description</td>
</tr>
<tr>
<td>ii</td>
<td>Info on imports</td>
</tr>
<tr>
<td>iI</td>
<td>Info on binary</td>
</tr>
<tr>
<td>ie</td>
<td>Display entrypoint</td>
</tr>
<tr>
<td>iS</td>
<td>Display sections</td>
</tr>
<tr>
<td>ir</td>
<td>Display relocations</td>
</tr>
</table>



## Print string

<table>
<tr>
<td>Command</td>
<td>Description</td>
</tr>
<tr>
<td>psz [offset]</td>
<td>Print zero terminated string</td>
</tr>
<tr>
<td>psb [offset]</td>
<td>Print strings in current block</td>
</tr>
<tr>
<td>psx [offset]</td>
<td>Show string with scaped chars</td>
</tr>
<tr>
<td>psp [offset]</td>
<td>Print pascal string</td>
</tr>
<tr>
<td>psw [offset]</td>
<td>Print wide string</td>
</tr>
</table>



## Visual mode

<table>
<tr>
<td>Command</td>
<td>Description</td>
</tr>
<tr>
<td>V</td>
<td>Enter visual mode</td>
</tr>
<tr>
<td>p/P</td>
<td>Rotate modes (hex, disasm, debug, words, buf)</td>
</tr>
<tr>
<td>c</td>
<td>Toggle (c)ursor</td>
</tr>
<tr>
<td>q</td>
<td>Back to Radare shell</td>
</tr>
<tr>
<td>hjkl</td>
<td>Move around (or HJKL) (left-down-up-right)</td>
</tr>
<tr>
<td>Enter</td>
<td>Follow address of jump/call</td>
</tr>
<tr>
<td>sS</td>
<td>Step/step over</td>
</tr>
<tr>
<td>o</td>
<td>Go/seek to given offset</td>
</tr>
<tr>
<td>.</td>
<td>Seek to program counter</td>
</tr>
<tr>
<td>/</td>
<td>In cursor mode, search in current block</td>
</tr>
<tr>
<td>:cmd</td>
<td>Run radare command</td>
</tr>
<tr>
<td>;[-]cmt</td>
<td>Add/remove comment</td>
</tr>
<tr>
<td>/*+-[]</td>
<td>Change block size, [] = resize hex.cols</td>
</tr>
<tr>
<td>>||<</td>
<td>Seek aligned to block size</td>
</tr>
<tr>
<td>i/a/A</td>
<td>(i)nsert hex, (a)ssemble code, visual (A)ssembler</td>
</tr>
<tr>
<td>b/B</td>
<td>Toggle breakpoint / automatic block size</td>
</tr>
<tr>
<td>d[f?]</td>
<td>Define function, data, code, ..</td>
</tr>
<tr>
<td>D</td>
<td>Enter visual diff mode (set diff.from/to)</td>
</tr>
<tr>
<td>e</td>
<td>Edit eval configuration variables</td>
</tr>
<tr>
<td>f/F</td>
<td>Set/unset flag</td>
</tr>
<tr>
<td>gG</td>
<td>Go seek to begin and end of file (0-$s)</td>
</tr>
<tr>
<td>mK/’K</td>
<td>Mark/go to Key (any key)</td>
</tr>
<tr>
<td>M</td>
<td>Walk the mounted filesystems</td>
</tr>
<tr>
<td>n/N</td>
<td>Seek next/prev function/flag/hit (scr.nkey)</td>
</tr>
<tr>
<td>o</td>
<td>Go/seek to given offset</td>
</tr>
<tr>
<td>C</td>
<td>Toggle (C)olors</td>
</tr>
<tr>
<td>R</td>
<td>Randomize color palette (ecr)</td>
</tr>
<tr>
<td>t</td>
<td>Track flags (browse symbols, functions..)</td>
</tr>
<tr>
<td>T</td>
<td>Browse anal info and comments</td>
</tr>
<tr>
<td>v</td>
<td>Visual code analysis menu</td>
</tr>
<tr>
<td>V/W</td>
<td>(V)iew graph (agv?), open (W)ebUI</td>
</tr>
<tr>
<td>uU</td>
<td>Undo/redo seek</td>
</tr>
<tr>
<td>x</td>
<td>Show xrefs to seek between them</td>
</tr>
<tr>
<td>yY</td>
<td>Copy and paste selection</td>
</tr>
<tr>
<td>z</td>
<td>Toggle zoom mode</td>
</tr>
</table>


## Searching
<table>
<tr>
<td>Command</td>
<td>Description</td>
</tr>
<tr>
<td>/ foo\00</td>
<td>Search for string ’foo\0’</td>
</tr>
<tr>
<td>/b</td>
<td>Search backwards</td>
</tr>
<tr>
<td>//</td>
<td>Repeat last search</td>
</tr>
<tr>
<td>/w foo</td>
<td>Search for wide string ’f\0o\0o\0’</td>
</tr>
<tr>
<td>/wi foo</td>
<td>Search for wide string ignoring case</td>
</tr>
<tr>
<td>/! ff</td>
<td>Search for first occurrence not matching</td>
</tr>
<tr>
<td>/i foo</td>
<td>Search for string ’foo’ ignoring case</td>
</tr>
<tr>
<td>/e /E.F/i</td>
<td>Match regular expression</td>
</tr>
<tr>
<td>/x ff0.23</td>
<td>Search for hex string</td>
</tr>
<tr>
<td>/x ff..33</td>
<td>Search for hex string ignoring some nibbles</td>
</tr>
<tr>
<td>/x ff43 ffd0</td>
<td>Search for hexpair with mask</td>
</tr>
<tr>
<td>/d 101112</td>
<td>Search for a deltified sequence of bytes</td>
</tr>
<tr>
<td>/!x 00</td>
<td>Inverse hexa search (find first byte != 0x00)</td>
</tr>
<tr>
<td>/c jmp [esp]</td>
<td>Search for asm code (see search.asmstr)</td>
</tr>
<tr>
<td>/a jmp eax</td>
<td>Assemble opcode and search its bytes</td>
</tr>
<tr>
<td>/A</td>
<td>Search for AES expanded keys</td>
</tr>
<tr>
<td>/r sym.printf</td>
<td>Analyze opcode reference an offset</td>
</tr>
<tr>
<td>/R</td>
<td>Search for ROP gadgets</td>
</tr>
<tr>
<td>/P</td>
<td>Show offset of previous instruction</td>
</tr>
<tr>
<td>/m magicfile</td>
<td>Search for matching magic file</td>
</tr>
<tr>
<td>/p patternsize</td>
<td>Search for pattern of given size</td>
</tr>
<tr>
<td>/z min max</td>
<td>Search for strings of given size</td>
</tr>
<tr>
<td>/v[?248] num</td>
<td>Look for a asm.bigendian 32bit value</td>
</tr>
</table>



## Saving


<table>
<tr>
<td>Command</td>
<td>Description</td>
</tr>
<tr>
<td>Po [file]</td>
<td>Open project</td>
</tr>
<tr>
<td>Ps [file]</td>
<td>Save project</td>
</tr>
<tr>
<td>Pi [file]</td>
<td>Show project information</td>
</tr>
</table>


## Usable variables in expression

<table>
<tr>
<td>$$</td>
<td>Here (current virtual seek)</td>
</tr>
<tr>
<td>$o</td>
<td>Here (current disk io offset)</td>
</tr>
<tr>
<td>$s</td>
<td>File size</td>
</tr>
<tr>
<td>$b</td>
<td>Block size</td>
</tr>
<tr>
<td>$w</td>
<td>Get word size, 4 if asm.bits=32, 8 if 64</td>
</tr>
<tr>
<td>$c,$r</td>
<td>Get width and height of terminal</td>
</tr>
<tr>
<td>$S</td>
<td>Section offset</td>
</tr>
<tr>
<td>$SS</td>
<td>Section size</td>
</tr>
<tr>
<td>$j</td>
<td>Jump address (jmp 0x10, jz 0x10 => 0x10)</td>
</tr>
<tr>
<td>$f</td>
<td>Jump fail address (jz 0x10 => next instruction)</td>
</tr>
<tr>
<td>$I</td>
<td>Number of instructions of current function</td>
</tr>
<tr>
<td>$F</td>
<td>Current function size</td>
</tr>
<tr>
<td>$Jn</td>
<td>Get nth jump of function</td>
</tr>
<tr>
<td>$Cn</td>
<td>Get nth call of function</td>
</tr>
<tr>
<td>$Dn</td>
<td>Get nth data reference in function</td>
</tr>
<tr>
<td>$Xn</td>
<td>Get nth xref of function</td>
</tr>
<tr>
<td>$m</td>
<td>Opcode memory reference (mov eax,[0x10] => 0x10)</td>
</tr>
<tr>
<td>$l</td>
<td>Opcode length</td>
</tr>
<tr>
<td>$e</td>
<td>1 if end of block, else 0</td>
</tr>
<tr>
<td>$ev</td>
<td>Get value of eval config variable</td>
</tr>
<tr>
<td>$?</td>
<td>Last comparison value</td>
</tr>
</table>

## License

Based on the Radare 2 reference card by Thanat0s, which is under the GNU GPL. Original license is as follows:
```
This card may be freely distributed under the terms of the GNU
general public licence — Copyright c  by Thanat0s - v0.1 -
```
