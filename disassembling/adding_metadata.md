Adding metadata

The work on binary files makes the task of taking notes and defining information on top of the file quite important. Radare offers multiple ways to retrieve and adquire this information from many kind of file types.

Following some *nix principles becomes quite easy to write a small utility in shellscript that using objdump, otool, etc.. to get information from a binary and import it into radare just making echo's of the commands script.

You can have a look on one of the many scripts that are distributed with radare like 'idc2r.py':

This script is called with 'idc2r.py file.idc > file.r2'. It reads an IDC file exported from an IDA database and imports the comments and the names of the functions.

We can import the 'file.r2' using the '.' command of radare (similar to the shell):
     
     [0x00000000]> . file.r2
     
The command '.' is used to interpret data from external resources like files, programs, etc.. In the same way we can do the same without writing a file.

     [0x00000000]> .!idc2r.py < file.idc

The 'C' command is the one used to manage comments and data conversions. So you can define a range of bytes to be interpreted as code, or a string. It is also possible to define flags and execute code in a certain seek to fetch a comment from an external file or database.

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

  

The 'C' command allows us to change the type of data. The three basic types are: code (disassembly using asm.arch), data (byte array) or string.

In visual mode is easier to manage this because it is hooked to the 'd' key trying to mean 'data type change'. Use the cursor to select a range of bytes ('c' key to toggle cursor mode and HJKL to move with selection) and then press 'ds' to convert to string.

You can use the Cs command from the shell also:

     [0x00000000]> f string_foo @ 0x800
     [0x00000000]> Cs 10 @ string_foo

The folding/unfolding is quite premature but the idea comes from the 'folder' concepts in vim. So you can select a range of bytes in the disassembly view and press '<' to fold these bytes in a single line or '>' to unfold them. Just to ease the readability of the code.

The Cm command is used to define a memory format string (the same used by the pf command). Here's a example:

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

 
This way it is possible to define structures by just using simple oneliners. See 'print memory' for more information.

All those C* commands can also be accessed from the visual mode by pressing 'd' (data conversion) key.

