# Memory Maps

The ability to understand and manipulate the memory maps of a debugged program is important for many different Reverse Engineering tasks. radare2 offers a rich set of commands to handle memory maps in the binary. This includes listing the memory maps of the currently debugged binary, removing memory maps, handling loaded libraries and more.

First, let's see the help message for `dm`, the command which is responsible for handling memory maps:

```
[0x55f2104cf620]> dm?
Usage: dm   # Memory maps commands
| dm                               List memory maps of target process
| dm address size                  Allocate <size> bytes at <address> (anywhere if address is -1) in child process
| dm=                              List memory maps of target process (ascii-art bars)
| dm.                              Show map name of current address
| dm*                              List memmaps in radare commands
| dm- address                      Deallocate memory map of <address>
| dmd[a] [file]                    Dump current (all) debug map region to a file (from-to.dmp) (see Sd)
| dmh[?]                           Show map of heap
| dmi [addr|libname] [symname]     List symbols of target lib
| dmi* [addr|libname] [symname]    List symbols of target lib in radare commands
| dmi.                             List closest symbol to the current address
| dmiv                             Show address of given symbol for given lib
| dmj                              List memmaps in JSON format
| dml <file>                       Load contents of file into the current map region
| dmm[?][j*]                       List modules (libraries, binaries loaded in memory)
| dmp[?] <address> <size> <perms>  Change page at <address> with <size>, protection <perms> (perm)
| dms[?] <id> <mapaddr>            Take memory snapshot
| dms- <id> <mapaddr>              Restore memory snapshot
| dmS [addr|libname] [sectname]    List sections of target lib
| dmS* [addr|libname] [sectname]   List sections of target lib in radare commands
| dmL address size                 Allocate <size> bytes at <address> and promote to huge page
```

In this chapter, we'll go over some of the most useful subcommands of `dm` using simple examples. For the following examples, we'll use a simple `helloworld` program for Linux but it'll be the same for every binary.

First things first - open a program in debugging mode:

```
$ r2 -d helloworld
Process with PID 20304 started...
= attach 20304 20304
bin.baddr 0x56136b475000
Using 0x56136b475000
asm.bits 64
[0x7f133f022fb0]>
```

> Note that we passed "helloworld" to radare2 without "./". radare2 will try to find this program in the current directory and then in $PATH, even if no "./" is passed. This is contradictory with UNIX systems, but makes the behaviour consistent for windows users

Let's use `dm` to print the memory maps of the binary we've just opened:

```
[0x7f133f022fb0]> dm
0x0000563a0113a000 - usr   4K s r-x /tmp/helloworld /tmp/helloworld ; map.tmp_helloworld.r_x
0x0000563a0133a000 - usr   8K s rw- /tmp/helloworld /tmp/helloworld ; map.tmp_helloworld.rw
0x00007f133f022000 * usr 148K s r-x /usr/lib/ld-2.27.so /usr/lib/ld-2.27.so ; map.usr_lib_ld_2.27.so.r_x
0x00007f133f246000 - usr   8K s rw- /usr/lib/ld-2.27.so /usr/lib/ld-2.27.so ; map.usr_lib_ld_2.27.so.rw
0x00007f133f248000 - usr   4K s rw- unk0 unk0 ; map.unk0.rw
0x00007fffd25ce000 - usr 132K s rw- [stack] [stack] ; map.stack_.rw
0x00007fffd25f6000 - usr  12K s r-- [vvar] [vvar] ; map.vvar_.r
0x00007fffd25f9000 - usr   8K s r-x [vdso] [vdso] ; map.vdso_.r_x
0xffffffffff600000 - usr   4K s r-x [vsyscall] [vsyscall] ; map.vsyscall_.r_x
```

For those of you who prefer a more visual way, you can use `dm=` to see the memory maps using an ASCII-art bars. This will be handy when you want to see how these maps are located in the memory.

If you want to know the memory-map you are currently in, use `dm.`:

```
[0x7f133f022fb0]> dm.
0x00007f947eed9000 # 0x00007f947eefe000 * usr   148K s r-x /usr/lib/ld-2.27.so /usr/lib/ld-2.27.so ; map.usr_lib_ld_2.27.so.r_x
```

Using `dmm` we can "List modules (libraries, binaries loaded in memory)", this is quite a handy command to see which modules were loaded.

```
[0x7fa80a19dfb0]> dmm
0x55ca23a4a000 /tmp/helloworld
0x7fa80a19d000 /usr/lib/ld-2.27.so
```
> Note that the output of `dm` subcommands, and `dmm` specifically, might be different in various systems and different binaries.

We can see that along with our `helloworld` binary itself, another library was loaded which is `ld-2.27.so`. We don't see `libc` yet and this is because radare2 breaks before `libc` is loaded to memory. Let's use `dcu` (**d**ebug **c**ontinue **u**ntil) to execute our program until the entry point of the program, which radare flags as `entry0`.

```
[0x7fa80a19dfb0]> dcu entry0
Continue until 0x55ca23a4a520 using 1 bpsize
hit breakpoint at: 55ca23a4a518
[0x55ca23a4a520]> dmm
0x55ca23a4a000 /tmp/helloworld
0x7fa809de1000 /usr/lib/libc-2.27.so
0x7fa80a19d000 /usr/lib/ld-2.27.so
```

Now we can see that `libc-2.27.so` was loaded as well, great!

Speaking of `libc`, a popular task for binary exploitation is to find the address of a specific symbol in a library. With this information in hand, you can build, for example, an exploit which uses ROP. This can be achieved using the `dmi` command. So if we want, for example, to find the address of [`system()`](http://man7.org/linux/man-pages/man3/system.3.html) in the loaded `libc`, we can simply execute the following command:

```
[0x55ca23a4a520]> dmi libc system
514 0x00000000 0x7fa809de1000  LOCAL  FILE    0 system.c
515 0x00043750 0x7fa809e24750  LOCAL  FUNC 1221 do_system
4468 0x001285a0 0x7fa809f095a0 LOCAL  FUNC  100 svcerr_systemerr
5841 0x001285a0 0x7fa809f095a0 LOCAL  FUNC  100 svcerr_systemerr
6427 0x00043d10 0x7fa809e24d10  WEAK  FUNC   45 system
7094 0x00043d10 0x7fa809e24d10 GLBAL  FUNC   45 system
7480 0x001285a0 0x7fa809f095a0 GLBAL  FUNC  100 svcerr_systemerr
```

Similar to the `dm.` command, with `dmi.` you can see the closest symbol to the current address.

Another useful command is to list the sections of a specific library. In the following example we'll list the sections of `ld-2.27.so`:

```
[0x55a7ebf09520]> dmS ld-2.27
[Sections]
00 0x00000000     0 0x00000000     0 ---- ld-2.27.so.
01 0x000001c8    36 0x4652d1c8    36 -r-- ld-2.27.so..note.gnu.build_id
02 0x000001f0   352 0x4652d1f0   352 -r-- ld-2.27.so..hash
03 0x00000350   412 0x4652d350   412 -r-- ld-2.27.so..gnu.hash
04 0x000004f0   816 0x4652d4f0   816 -r-- ld-2.27.so..dynsym
05 0x00000820   548 0x4652d820   548 -r-- ld-2.27.so..dynstr
06 0x00000a44    68 0x4652da44    68 -r-- ld-2.27.so..gnu.version
07 0x00000a88   164 0x4652da88   164 -r-- ld-2.27.so..gnu.version_d
08 0x00000b30  1152 0x4652db30  1152 -r-- ld-2.27.so..rela.dyn
09 0x00000fb0 11497 0x4652dfb0 11497 -r-x ld-2.27.so..text
10 0x0001d0e0 17760 0x4654a0e0 17760 -r-- ld-2.27.so..rodata
11 0x00021640  1716 0x4654e640  1716 -r-- ld-2.27.so..eh_frame_hdr
12 0x00021cf8  9876 0x4654ecf8  9876 -r-- ld-2.27.so..eh_frame
13 0x00024660  2020 0x46751660  2020 -rw- ld-2.27.so..data.rel.ro
14 0x00024e48   336 0x46751e48   336 -rw- ld-2.27.so..dynamic
15 0x00024f98    96 0x46751f98    96 -rw- ld-2.27.so..got
16 0x00025000  3960 0x46752000  3960 -rw- ld-2.27.so..data
17 0x00025f78     0 0x46752f80   376 -rw- ld-2.27.so..bss
18 0x00025f78    17 0x00000000    17 ---- ld-2.27.so..comment
19 0x00025fa0    63 0x00000000    63 ---- ld-2.27.so..gnu.warning.llseek
20 0x00025fe0 13272 0x00000000 13272 ---- ld-2.27.so..symtab
21 0x000293b8  7101 0x00000000  7101 ---- ld-2.27.so..strtab
22 0x0002af75   215 0x00000000   215 ---- ld-2.27.so..shstrtab
```
