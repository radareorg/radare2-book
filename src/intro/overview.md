## Toolchain

The Radare2 project provides a collection of command-line tools, APIs and scripting capabilities that focus on several aspects of reverse engineering.

This chapter will give you a quick understanding of them, but you can check the dedicated sections for each tool at the end of this book. 

**radare2**

When you run the radare2 command, you're presented with an interactive shell that gives you access to all the functionalities of the radare2 framework by running mnemonic commands or scripts.

At its core, radare2 lets you open various types of files and data sources, treating them all as if they were simple binary files. This includes executables, disk images, memory dumps, and even live processes or network connections.

Once you've opened a file, radare2 provides a wide array of commands for exploration and analysis. You can navigate through the file's contents, view disassembled code, examine data structures, and even modify the binary on the fly.

Some key features accessible through the radare2 command include:

- Hex editing capabilities
- Advanced code analysis and disassembly
- Debugging functionality
- Binary patching
- Data visualization tools
- Scripting support for automation and extending functionality

The radare2 command serves as the primary interface to these features, offering a flexible and powerful environment for reverse engineering, malware analysis, exploit development, and general binary exploration.

**rax2**

A minimalistic mathematical expression evaluator for the shell that is useful for making base conversions between floating point values, hexadecimal representations, hexpair strings to ASCII, octal to integer, and more. It also supports endianness settings and can be used as an interactive shell if no arguments are given.

Examples

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

**rabin2**

Another important tool distributed with radare2, rabin2 is designed to analyze binary files and extract various types of information from them. It supports a wide range of file formats (depending on which plugins are loaded or compiled in), the most famous ones are:

- ELF (Executable and Linkable Format)
- PE (Portable Executable)
- Mach-O (Mach Object)
- Java CLASS files

Key features and uses of rabin2 include:

* Extracting metadata: File type, architecture, OS, subsystem, etc.
* Listing symbols: Both imported and exported symbols.
* Displaying section information: Names, sizes, permissions, etc.
* Showing library dependencies.
* Extracting strings from the binary.
* Identifying entry points and constructor/destructors.
* Listing header structures and information.

rabin2 can be used standalone from the command line or integrated within other radare2 tools. It's particularly useful for quick analysis of binaries without the need to fully load them into a debugger or disassembler. The information provided by rabin2 is often used by other parts of the radare2 framework to enhance analysis and provide context during reverse engineering tasks.

**rasm2**

The command-line assembler and disassembler. It supports a wide range of architectures and can be used independently of the main radare2 tool. Key features include:

- Multi-architecture support: Can handle numerous architectures including x86, x86-64, ARM, MIPS, PowerPC, SPARC, and many others.
- Bi-directional operation: Functions as both an assembler (converting human-readable assembly code to machine code) and a disassembler (converting machine code back to assembly).
- Flexible input/output: Accepts input as hexadecimal strings, raw binary files, or text files containing assembly code.
- Shellcode generation: Useful for security research and exploit development.
- Inline assembly: Allows for quick assembly of individual instructions or small code snippets.
- Syntax highlighting: Provides colored output for better readability when disassembling.
- Plugins: Supports architecture-specific plugins for extended functionality.

For example assembling and disassembling a nop for java:

```
$ rasm2 -a java 'nop'
00

$ rasm2 -a x86 -d '90'
nop

$ rasm2 -a x86 -b 32 'mov eax, 33'
b821000000

$ echo 'push eax;nop;nop' | rasm2 -f -
509090
```

**rahash2**

Versatile command-line hashing tool that is part of the radare2 framework. It's designed to compute and verify cryptographic hashes and checksums for files, strings, or even large data streams like hard disks or network traffic.

* Supports a wide range of **hash algorithms**
  * MD4, MD5, SHA1, SHA256, SHA384, SHA512, CRC16, CRC32, and more.
* Input data from files, stdin, or command-line strings.
* Block-based hashing
* Incremental hashing for large files
* Hash verification
* Various output formats, including raw bytes, hexadecimal strings, or radare2 commands.
* Basic encryption capabilities

Here are few usage examples:

```
$ rahash2 file
file: 0x00000000-0x00000007 sha256: 887cfbd0d44aaff69f7bdbedebd282ec96191cce9d7fa7336298a18efc3c7a5a
```

Algorithms can be selected by specifying them separated with the `-a` flag.

```
$ rahash2 -a md5 file
file: 0x00000000-0x00000007 md5: d1833805515fc34b46c2b9de553f599d
```

**radiff2**

The commandline Binary Diffing utility of the radare2 reverse engineering framework. It is designed to compare and analyze differences between binary files or sections of binary data.

Key features and capabilities of radiff2 include:

* Compare byte-per-byte changes between two files
* Delta diffing on files with different sizes, finding common sections
* Code Analysis diffing, finding functions in common
* Binary header comparing libraries, exports, etc
* Visualization options: graphs, ascii art, plain text, json
* Integration with radare2: Using the `c` command or running it with the `-r` flag.
* Patch generation, writing r2 scripts that patch one file to update it like the other.

radiff2 is particularly useful for cases where you need to identify and understand changes between different versions of binary files or compare potentially malicious files with known good versions.

**rafind2**

rafind2 is a command-line utility designed to search for byte patterns, strings, or hexadecimal values within binary files or any other type of file.

This tool implements the most common subcommands of the radare2's `/` (search) operation.

* Search for patterns, strings, binary data and regular expression
* Run across multiple files
* Output formats, showing offsets, json and even r2 commands

**ragg2**

Ragg2 is a versatile tool primarily serving as a shellcode compiler.

The tool is written on top of the `r_egg` library which can construct exploit payloads from the commandline, adding paddings, nops, sequences of bytes, compile small relocatable programs and even apply transformations to avoid some specific characters.

The `ragg2-cc` tool can compile C programs into raw linear instructions that are relocatable and are useful for injecting them in target processes or use them in exploits.

This feature is conceptually based on shellforge4, but only linux/osx x86-32/64 platforms are supported.

It can also compile a specific low level domain specific language and generate tiny binaries without the need of any system compiler or toolchain as exemplified below:

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

**rarun2**

A launcher for running programs within different environments, with different arguments, permissions, directories, and overridden default file descriptors.
