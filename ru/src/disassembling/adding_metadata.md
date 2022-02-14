## Adding Metadata to Disassembly

The typical work involved in reversing binary files makes powerful annotation capabilities essential.
Radare offers multiple ways to store and retrieve such metadata.

By following common basic UNIX principles, it is easy to write a small utility in a scripting language which uses `objdump`, `otool` or any other existing utility to obtain information from a binary and to import it into radare. For example, take a look at `idc2r.py` shipped with [radare2ida](https://github.com/radareorg/radare2ida). To use it, invoke it as `idc2r.py file.idc > file.r2`. It reads an IDC file exported from an IDA Pro database and produces an r2 script containing the same comments, names of functions and other data. You can import the resulting 'file.r2' by using the dot `.` command of radare:
```
[0x00000000]> . file.r2
```
The `.` command is used to interpret Radare commands from external sources, including files and program output. For example, to omit generation of an intermediate file and import the script directly you can use this combination:
```
[0x00000000]> .!idc2r.py < file.idc
```

Please keep in mind that importing IDA Pro metadata from IDC dump is deprecated mechanism and might
not work in the future. The recommended way to do it - use [python-idb](https://github.com/williballenthin/python-idb)-based `ida2r2.py` which
opens IDB files directly without IDA Pro installed.

The `C` command is used to manage comments and data conversions. You can define a range of program's bytes to be interpreted as either code, binary data or string. It is also possible to execute external code at every specified flag location in order to fetch some metadata, such as a comment, from an external file or database.

There are many different metadata manipulation commands, here is the glimpse of all of them:

```
[0x00404cc0]> C?
| Usage: C[-LCvsdfm*?][*?] [...]   # Metadata management
| C                                              list meta info in human friendly form
| C*                                             list meta info in r2 commands
| C*.                                            list meta info of current offset in r2 commands
| C- [len] [[@]addr]                             delete metadata at given address range
| C.                                             list meta info of current offset in human friendly form
| CC! [@addr]                                    edit comment with $EDITOR
| CC[?] [-] [comment-text] [@addr]               add/remove comment
| CC.[addr]                                      show comment in current address
| CCa[-at]|[at] [text] [@addr]                   add/remove comment at given address
| CCu [comment-text] [@addr]                     add unique comment
| CF[sz] [fcn-sign..] [@addr]                    function signature
| CL[-][*] [file:line] [addr]                    show or add 'code line' information (bininfo)
| CS[-][space]                                   manage meta-spaces to filter comments, etc..
| C[Cthsdmf]                                     list comments/types/hidden/strings/data/magic/formatted in human friendly form
| C[Cthsdmf]*                                    list comments/types/hidden/strings/data/magic/formatted in r2 commands
| Cd[-] [size] [repeat] [@addr]                  hexdump data array (Cd 4 10 == dword [10])
| Cd. [@addr]                                    show size of data at current address
| Cf[?][-] [sz] [0|cnt][fmt] [a0 a1...] [@addr]  format memory (see pf?)
| Ch[-] [size] [@addr]                           hide data
| Cm[-] [sz] [fmt..] [@addr]                     magic parse (see pm?)
| Cs[?] [-] [size] [@addr]                       add string
| Ct[?] [-] [comment-text] [@addr]               add/remove type analysis comment
| Ct.[@addr]                                     show comment at current or specified address
| Cv[bsr][?]                                     add comments to args
| Cz[@addr]                                      add string (see Cs?)
```

Simply to add the comment to a particular line/address you can use `Ca` command:

```
[0x00000000]> CCa 0x0000002 this guy seems legit
[0x00000000]> pd 2
0x00000000    0000         add [rax], al
;      this guy seems legit
0x00000002    0000         add [rax], al
```

The `C?` family of commands lets you mark a range as one of several kinds of types. Three basic types are: code (disassembly is done using asm.arch), data (an array of data elements) or string. Use the `Cs` comand to define a string, use the `Cd` command for defining an array of data elements, and use the `Cf` command to define more complex data structures like structs.

Annotating data types is most easily done in visual mode, using the "d" key, short for "data type change". First, use the cursor to select a range of bytes (press `c` key to toggle cursor mode and use HJKL keys to expand selection), then press 'd' to get a menu of possible actions/types. For example, to mark the range as a string, use the 's' option from the menu. You can achieve the same result from the shell using the `Cs` command:

```
[0x00000000]> f string_foo @ 0x800
[0x00000000]> Cs 10 @ string_foo
```

The `Cf` command is used to define a memory format string (the same syntax used by the `pf` command). Here's an example:

```
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
0x7fd9f13ae638    4989c4       mov r12, rax
```

The `[sz]` argument to `Cf` is used to define how many bytes the struct should take up in the disassembly, and is completely independent from the size of the data structure defined by the format string. This may seem confusing, but has several uses. For example, you may want to see the formatted structure displayed in the disassembly, but still have those locations be visible as offsets and with raw bytes. Sometimes, you find large structures, but only identified a few fields, or only interested in specific fields. Then, you can tell r2 to display only those fields, using the format string and using 'skip' fields, and also have the disassembly continue after the entire structure, by giving it full size using the `sz` argument.

Using `Cf`, it's easy to define complex structures with simple oneliners. See `pf?` for more information.
Remember that all these `C` commands can also be accessed from the visual mode by pressing the `d` (data conversion) key.
Note that unlike [`t`](../analysis/types.md) commands `Cf` doesn't change analysis results. It is only
a visual boon.

Sometimes just adding a single line of comments is not enough, in this case radare2 allows you to
create a link for a particular text file. You can use it with `CC,` command or by pressing `,` key in
the visual mode. This will open an `$EDITOR` to create a new file, or if filename does exist, just
will create a link. It will be shown in the disassembly comments:

```
[0x00003af7 11% 290 /bin/ls]> pd $r @ main+55 # 0x3af7
│0x00003af7  call sym.imp.setlocale        ;[1] ; ,(locale-help.txt) ; char *setlocale(int category, const char *locale)
│0x00003afc  lea rsi, str.usr_share_locale ; 0x179cc ; "/usr/share/locale"
│0x00003b03  lea rdi, [0x000179b2]         ; "coreutils"
│0x00003b0a  call sym.imp.bindtextdomain   ;[2] ; char *bindtextdomain(char *domainname, char *dirname)
```

Note `,(locale-help.txt)` appeared in the comments, if we press `,` again in the visual mode, it
will open the file. Using this mechanism we can create a long descriptions of some particular places
in disassembly, link datasheets or related articles.

