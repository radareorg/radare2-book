## Adding Metadata to Disassembly

The typical work involved in reversing binary files makes powerful annotation capabailities essential.
Radare offers multiple ways to store and retrieve such metadata.

By following common basic *NIX principles, it is easy to write a small utility in a scripting language which uses `objdump`, `otool`, etc. to obtain information from a binary and to import it into radare. For example, take a look at one of many scripts that are distributed with radare, e.g., `idc2r.py`. To use it, invoke it as `idc2r.py file.idc > file.r2`. It reads an IDC file exported from an IDA Pro database and produces an r2 script containing the same comments, names of functions etc. You can import the resulting 'file.r2' by using the dot `.` command of radare:

     [0x00000000]> . file.r2

The `.` command is used to interpret Radare commands from external sources, including files and program output. For example, to omit generation of an intermediate file and import the script directly you can use this combination:

     [0x00000000]> .!idc2r.py < file.idc

The `C` command is used to manage comments and data conversions. You can define a range of program's bytes to be interpreted as either code, binary data or string. It is also possible to execute external code at every specified flag location in order to fetch some metadata, such as a comment, from an external file or database.

Here's the help:

    [0x00404cc0]> C?
    |Usage: C[-LCvsdfm?] [...]
    | C*                              List meta info in r2 commands
    | C- [len] [@][ addr]             delete metadata at given address range
    | CL[-] [addr|file:line [addr] ]  show 'code line' information (bininfo)
    | Cl  file:line [addr]            add comment with line information
    | CC[-] [comment-text]    add/remove comment. Use CC! to edit with $EDITOR
    | CCa[-at]|[at] [text]    add/remove comment at given address
    | Cv[-] offset reg name   add var substitution
    | Cs[-] [size] [[addr]]   add string
    | Ch[-] [size] [@addr]    hide data
    | Cd[-] [size]            hexdump data
    | Cf[-] [sz] [fmt..]      format memory (see pf?)
    | Cm[-] [sz] [fmt..]      magic parse (see pm?)
    [0x00404cc0]>


    [0x00000000]> CCa 0x0000002 this guy seems legit

    [0x00000000]> pd 2
               0x00000000    0000         add [rax], al
       ;      this guy seems legit
               0x00000002    0000         add [rax], al



The `C` command allows to change type for a byte range. Three basic types are: code (disassembly is done using asm.arch), data (a byte array) or string.

It is easier to manage data types conversion in the visual mode, because the action is bound to "d" key, short for "data type change". Use the cursor to select a range of bytes (press `c` key to toggle cursor mode and use HJKL keys to expand selection) and then press 'ds' to convert it to a string. Alternatively, you can use the `Cs` command from the shell:

     [0x00000000]> f string_foo @ 0x800
     [0x00000000]> Cs 10 @ string_foo

The folding/unfolding support is quite premature, but the idea comes from "folder" concept found in Vim editor. You can select a range of bytes in the disassembly view and press '<' to fold them in a single line, or '>' to unfold them. This is used to improve readability by hiding unimportant portions of code/data.

The `Cm` command is used to define a memory format string (the same used by the `pf` command). Here's a example:

      [0x7fd9f13ae630]> Cf 16 2xi foo bar
      [0x7fd9f13ae630]> pd
                  ;-- rip:
                  0x7fd9f13ae630 format 2xi foo bar {
      0x7fd9f13ae630 [0] {
         foo : 0x7fd9f13ae630 = 0xe8e78948
         bar : 0x7fd9f13ae634 = 14696
      }
      0x7fd9f13ae638 [1] {
         foo : 0x7fd9f13ae638 = 0x8bc48949
         bar : 0x7fd9f13ae63c = 571928325
      }
      } 16
                  0x7fd9f13ae633    e868390000   call 0x7fd9f13b1fa0
                     0x7fd9f13b1fa0() ; rip
                  0x7fd9f13ae638    4989c4       mov r12, rax

It is possible to define structures with simple oneliners. See 'print memory' for more information.

All these "C*" commands can also be accessed from the visual mode by pressing 'd' (data conversion) key.
