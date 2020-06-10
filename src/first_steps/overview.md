## The Framework

The Radare2 project is a set of small command-line utilities that can be used together or independently.

This chapter will give you a quick understanding of them, but you can check the dedicated sections for each tool at the end of this book. 

### radare2

The main tool of the whole framework. It uses the core of the hexadecimal editor and debugger. radare2 allows you to open a number of input/output sources as if they were simple, plain files, including disks, network connections, kernel drivers, processes under debugging, and so on.

It implements an advanced command line interface for moving around a file, analyzing data, disassembling, binary patching, data comparison, searching, replacing, and visualizing. It can be scripted with a variety of languages, including Python, Ruby, JavaScript, Lua, and Perl.

### rabin2

A program to extract information from executable binaries, such as ELF, PE, Java CLASS, Mach-O, plus any format supported by r2 plugins. rabin2 is used by the core to get data like exported symbols, imports, file information, cross references (xrefs), library dependencies, and sections.

### rasm2

A command line assembler and disassembler for multiple architectures (including Intel x86 and x86-64, MIPS, ARM, PowerPC, Java, and myriad of others).

#### Examples
```
$ rasm2 -a java 'nop'
00
```
```
$ rasm2 -a x86 -d '90'
nop
```
```
$ rasm2 -a x86 -b 32 'mov eax, 33'
b821000000
```
```
$ echo 'push eax;nop;nop' | rasm2 -f -
509090
```

### rahash2

An implementation of a block-based hash tool. From small text strings to large disks, rahash2 supports multiple algorithms, including MD4, MD5, CRC16, CRC32, SHA1, SHA256, and others.
rahash2 can be used to check the integrity or track changes of big files, memory dumps, or disks.

### Examples
```
$ rahash2 file
file: 0x00000000-0x00000007 sha256: 887cfbd0d44aaff69f7bdbedebd282ec96191cce9d7fa7336298a18efc3c7a5a
```
```
$ rahash2 -a md5 file
file: 0x00000000-0x00000007 md5: d1833805515fc34b46c2b9de553f599d
```
### radiff2

A binary diffing utility that implements multiple algorithms. It supports byte-level or delta diffing for binary files, and code-analysis diffing to find changes in basic code blocks obtained from the radare code analysis.

### rafind2

A program to find byte patterns in files.

### ragg2

A frontend for r_egg. ragg2 compiles programs written in a simple high-level language into tiny binaries for x86, x86-64, and ARM.

#### Examples

```
$ cat hi.r
/* hello world in r_egg */
write@syscall(4); //x64 write@syscall(1);
exit@syscall(1); //x64 exit@syscall(60);

main@global(128) {
 .var0 = "hi!\n";
 write(1,.var0, 4);
 exit(0);
}
$ ragg2 -O -F hi.r
$ ./hi
hi!

$ cat hi.c
main@global(0,6) {
 write(1, "Hello0", 6);
 exit(0);
}
$ ragg2 hi.c
$ ./hi.c.bin
Hello
```

### rarun2

A launcher for running programs within different environments, with different arguments,
permissions, directories, and overridden default file descriptors. rarun2 is useful for:

* Solving crackmes
* Fuzzing
* Test suites

#### Sample rarun2 script
```
$ cat foo.rr2
#!/usr/bin/rarun2
program=./pp400
arg0=10
stdin=foo.txt
chdir=/tmp
#chroot=.
./foo.rr2
```

#### Connecting a Program with a Socket
```
$ nc -l 9999
$ rarun2 program=/bin/ls connect=localhost:9999
```

#### Debugging a Program Redirecting the stdio into Another Terminal

1 - open a new terminal and type 'tty' to get a terminal name:

```
$ tty ; clear ; sleep 999999
/dev/ttyS010
```

2 - Create a new file containing the following rarun2 profile named foo.rr2:
```
#!/usr/bin/rarun2
program=/bin/ls
stdio=/dev/ttys010
```

3 - Launch the following radare2 command:
```
r2 -r foo.rr2 -d /bin/ls
```

### rax2

A minimalistic mathematical expression evaluator for the shell that is useful for making base conversions between floating point values, hexadecimal representations, hexpair strings to ASCII, octal to integer, and more. It also supports endianness settings and can be used as an interactive shell if no arguments are given.

#### Examples

```
$ rax2 1337
0x539

$ rax2 0x400000
4194304

$ rax2 -b 01111001
y

$ rax2 -S radare2
72616461726532

$ rax2 -s 617765736f6d65
awesome
```
