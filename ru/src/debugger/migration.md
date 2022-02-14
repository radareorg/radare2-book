# Migration from ida, GDB or WinDBG

## How to run the program using the debugger ##

`r2 -d /bin/ls` - start in debugger mode => [[video](http://asciinema.org/a/12022)]

## How do I attach/detach to running process ? (gdb -p) ##

`r2 -d <pid>` - attach to process

`r2 ptrace://pid` - same as above, but only for io (not debugger backend hooked)

`[0x7fff6ad90028]> o-225` - close fd=225 (listed in `o~[1]:0`)

`r2 -D gdb gdb://localhost:1234` - attach to gdbserver

## How to set args/environment variable/load a specific libraries for the debugging session of radare

Use `rarun2` (`libpath=$PWD:/tmp/lib`, `arg2=hello`, `setenv=FOO=BAR` ...) see `rarun2 -h` / `man rarun2`

## How to script radare2 ?

`r2 -i <scriptfile> ...` - run a script **after** loading the file => [[video](http://asciinema.org/a/12020)]

`r2 -I <scriptfile> ...` - run a script **before** loading the file

`r2 -c $@ | awk $@`  - run through awk to get asm from function => [[link](http://sprunge.us/dEOK)]

`[0x80480423]> . scriptfile` - interpret this file => [[video](http://asciinema.org/a/12017)]

`[0x80480423]> #!c` - enter C repl  (see `#!` to list all available RLang plugins)  => [[video](http://asciinema.org/a/12019)], everything have to be done in a oneliner or a .c file must be passed as an argument.

To get `#!python` and much more, just build [radare2-bindings](https://github.com/radareorg/radare2-bindings)

## How to list Source code as in gdb list ?

`CL @ sym.main` - though the feature is highly experimental

# shortcuts

| Command       | IDA Pro       | radare2        | r2 (visual mode) | GDB | WinDbg |
| ------------- | ------------- |----------------|------------------|-----|-----|
|**Analysis** |               |                |
|Analysis of everything        |`Automatically launched when opening a binary`            |`aaa or -A (aaaa or -AA for even experimental analysis)`           | `N/A`              |N/A              |N/A
|**Navigation** |               |                |
|xref to        |`x`            |`axt`           | `x`              |N/A              |N/A
|xref from      |`ctrl + j`     |`axf`           | `X`              |N/A              |N/A
|xref to graph  |?              |`agt [offset]`  | ?                |N/A              |N/A
|xref from graph|?              |`agf [offset]`  | ?                |N/A              |N/A
|list functions |`alt + 1`      |`afl;is`        | `t`              |N/A              |N/A
|listing        |`alt + 2`      |`pdf`           | `p`              |N/A              |N/A
|hex mode       |`alt + 3`      |`pxa`           | `P`              |N/A              |N/A
|imports        |`alt + 6`      |`ii`            | `:ii`            |N/A              |N/A
|exports        |`alt + 7`      |`is~FUNC`       | ?                |N/A              |N/A
|follow jmp/call|`enter`        |`s offset`      |`enter` or `0`-`9`|N/A              |N/A
|undo seek      |`esc`          |`s-`            | `u`              |N/A              |N/A
|redo seek      |`ctrl+enter`   |`s+`            | `U`              |N/A              |N/A
|show graph     |`space`        |`agv`           | `V`              |N/A              |N/A
|**Edit**       |               |                |
|rename         |`n`            |`afn`| `dr`            |N/A              |N/A
|graph view     |`space`        |`agv`           | `V`              |N/A              |N/A
|define as data |`d`            |`Cd [size]`     | `dd`,`db`,`dw`,`dW`|N/A              |N/A
|define as code |`c`            |`C- [size]`     | `d-` or `du`     |N/A              |N/A
|define as undefined|`u`        |`C- [size]`     | `d-` or `du`     |N/A              |N/A
|define as string|`A`           |`Cs [size]`     | `ds`             |N/A              |N/A
|define as struct|`Alt+Q`       |`Cf [size]`     | `dF`             |N/A              |N/A
|**Debugger**   |               |                |                | 
|Start Process/ Continue execution|`F9`    |`dc` | `F9`             | `r` and `c`             | `g`
|Terminate Process|`Ctrl+F2`    |`dk 9`          | ?                | `kill`             | `q`
|Detach         |`?`            | `o-`           | ?                | `detach`             |
|step into      |`F7`           |`ds`            | `s`              | `n`             | `t`
|step into 4 instructions | ?   | `ds 4`         | F7               | `n 4`             | `t 4`
|step over      |`F8`           |`dso`           | `S`              | `s`             | `p` 
|step until a specific address|?|`dsu <addr>`    | ?                | `s`             | `g <addr>`
|Run until return|`Ctrl+F7`     |`dcr`           | ?                | `finish`             | `gu`
|Run until cursor|`F4`          |[#249](https://github.com/radareorg/radare2/issues/249)| [#249](https://github.com/radareorg/radare2/issues/249) | N/A             | N/A
|Show Backtrace |`?`            |`dbt`           | ?                | `bt`             |
|display Register|On register Windows|`dr all`   | Shown in Visual mode | `info registers`             | `r`
|display eax    |On register Windows |`dr?eax`   | Shown in Visual mode | `info registers eax`             | `r rax`
|display old state of all registers |? |`dro`    | ?                | ?             | ?
|display function addr + N  |?  |`afi $$` - display function information of current offset (`$$`) | ?               | ?             | ?
|display frame state     |?     |`pxw rbp-rsp@rsp`| ?               | `i f`             | ?
|How to step until condition is true |?  |`dsi`  | ?                | ?             | ?
|Update a register value |?     |`dr rip=0x456`  | ?                | `set $rip=0x456`             | `r rip=456`
|**Disassembly**   |            |                |                  |
|disassembly forward   | N/A           | `pd`               | `Vp`                 | `disas` | `uf`, `u`
|disassembly N instructions   | N/A           | `pd X`               | Vp                 | x/<N>i | `u <addr> LX`
|disassembly N (backward)   | N/A           | `pd -X`               | `Vp`                 | `disas <a-o> <a>` | `ub`
|**Information on the bin**   |            |                |                  |
|Sections/regions   | `Menu sections`           | `iS` or `S` (append j for json)               | N/A                 | maint info sections | !address
|**Load symbol file**   |            |                |                  |
|Sections/regions   | `pdb menu`           | `asm.dwarf.file`, `pdb.XX`)               | N/A                 |  add-symbol-file | r
|**BackTrace**   |            |                |                  |
|Stack Trace   | N/A           | `dbt`               | N/A                 | `bt` | `k`
|Stack Trace in Json   | N/A           | `dbtj`               | N/A                 |  | 
|Partial Backtrace (innermost)  | N/A           | `dbt` (`dbg.btdepth` `dbg.btalgo`)               | N/A                 | bt <N> | k <N>
|Partial Backtrace (outermost)  | N/A           | `dbt` (`dbg.btdepth` `dbg.btalgo`)               | N/A                 | bt -<N> |
|Stacktrace for all threads  | N/A           | `dbt@t`               | N/A                 | `thread apply all bt` | `~* k`
|**Breakpoints**   |            |                |                  | |
|Breakpoint list |`Ctrl+Alt+B`  |`db`            | ?                | `info breakpoints` | `bl`
|add breakpoint |`F2`           |`db [offset]`   |`F2`              | `break`| `bp`
|**Threads**   |        |                |                  |                  |
|Switch to thread |`Thread menu`           |`dp`   | N/A              | `thread <N>`| `~<N>s`
|**Frames**   |        |                |                  |                  |
|Frame Numbers |`N/A`           |`?`   | N/A              | `any bt command`| `kn`
|Select Frame |`N/A`           |`?`   | N/A              | `frame`| `.frame`
|**Parameters/Locals**   |        |                |                  |                  |
|Display parameters |`N/A`           |`afv`   | N/A              | `info args`| `dv /t /i /V`
|Display parameters |`N/A`           |`afv`   | N/A              | `info locals`| `dv /t /i /V`
|Display parameters/locals in json |`N/A`           |`afvj`   | N/A              | `info locals`| `dv /t /i /V`
|list addresses where vars are accessed(R/W) |`N/A`           |`afvR/afvW`   | N/A              | `?`| `?`
|**Project Related**   |        |                |                  |
|open project |                 |`Po [file]`     |                  | ?
|save project | automatic       |`Ps [file]`     |                  | ?
|show project informations |    |`Pi [file]`     |                  | ?
|**Miscellaneous**   |               |                |             |
|Dump byte char array        | `N/A`    |`pc?` (json, C, char, etc.)            | Vpppp              | x/<N>bc | db
|options        |option menu    |`e?`            | `e`              |
|search         |search menu    |`/?`            | Select the zone with the cursor `c` then `/`              | | `s`

## Equivalent of "set-follow-fork-mode" gdb command
This can be done using 2 commands:

1. `dcf` - until a fork happen
2. then use `dp` to select what process you want to debug.

# Common features
- r2 accepts FLIRT signatures
- r2 can connect to GDB, LLVM and WinDbg
- r2 can write/patch in place
- r2 have fortunes and [s]easter eggs[/s]balls of steel
- r2 can do basic loading of ELF core files from the box and MDMP (Windows minidumps)
