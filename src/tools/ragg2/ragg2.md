# ragg2

ragg2 stands for `radare2's egg compiler`, it's the basic tool to compile relocatable snippets of code and modify paddings and inject sequences in order to be used for injection in target processes when doing exploiting.

ragg2 compiles programs written in a simple high-level language into tiny binaries for x86, x86-64, and ARM.

The final bytestream can be rendered in a variety of output formats, including raw binary, C arrays, and various executable formats. This flexibility allows users to generate code that can be easily integrated into different types of projects or testing scenarios. Additionally, ragg2 can perform operations like encoding and encryption on the generated shellcode, which can be useful for evading detection or bypassing security measures in controlled testing environments.

## Example

By default it will compile it's own `ragg2` language, but you can also compile C code using GCC or Clang shellcodes depending on the file extension. Lets create C file called `a.c`:

```c
int main() {
	write(1, "Hello World\n", 13);
	return 0;
}
```

That small C program can be compiled with ragg2 like this:

```
$ ragg2 -a x86 -b32 a.c
e900000000488d3516000000bf01000000b80400000248c7c20d0000000f0531c0c348656c6c6f20576f726c640a00

$ rasm2 -a x86 -b 32 -D e900000000488d3516000000bf01000000b80400000248c7c20d0000000f0531c0c348656c6c6f20576f726c640a00
0x00000000   5               e900000000  jmp 5
0x00000005   1                       48  dec eax
0x00000006   6             8d3516000000  lea esi, [0x16]
0x0000000c   5               bf01000000  mov edi, 1
0x00000011   5               b804000002  mov eax, 0x2000004
0x00000016   1                       48  dec eax
0x00000017   6             c7c20d000000  mov edx, 0xd
0x0000001d   2                     0f05  syscall
0x0000001f   2                     31c0  xor eax, eax
0x00000021   1                       c3  ret
0x00000022   1                       48  dec eax
0x00000023   2                     656c  insb byte es:[edi], dx
0x00000025   1                       6c  insb byte es:[edi], dx
0x00000026   1                       6f  outsd dx, dword [esi]
0x00000027   3                   20576f  and byte [edi + 0x6f], dl
0x0000002a   2                     726c  jb 0x98
0x0000002c   3                   640a00  or al, byte fs:[eax]
```

## Help message

Checking the help from the commandline will give you a wide understanding of what's the tool about and its capabilities

```
Usage: ragg2 [-FOLsrxhvz] [-a arch] [-b bits] [-k os] [-o file] [-I path]
             [-i sc] [-E enc] [-B hex] [-c k=v] [-C file] [-p pad] [-q off]
             [-S string] [-f fmt] [-nN dword] [-dDw off:hex] [-e expr] file|f.asm|-
 -a [arch]       select architecture (x86, mips, arm)
 -b [bits]       register size (32, 64, ..)
 -B [hexpairs]   append some hexpair bytes
 -c [k=v]        set configuration options
 -C [file]       append contents of file
 -d [off:dword]  patch dword (4 bytes) at given offset
 -D [off:qword]  patch qword (8 bytes) at given offset
 -e [egg-expr]   take egg program from string instead of file
 -E [encoder]    use specific encoder. see -L
 -f [format]     output format (raw, c, pe, elf, mach0, python, javascript)
 -F              output native format (osx=mach0, linux=elf, ..)
 -h              show this help
 -i [shellcode]  include shellcode plugin, uses options. see -L
 -I [path]       add include path
 -k [os]         operating system's kernel (linux,bsd,osx,w32)
 -L              list all plugins (shellcodes and encoders)
 -n [dword]      append 32bit number (4 bytes)
 -N [dword]      append 64bit number (8 bytes)
 -o [file]       output file
 -O              use default output file (filename without extension or a.out)
 -p [padding]    add padding after compilation (padding=n10s32)
                 ntas : begin nop, trap, 'a', sequence
                 NTAS : same as above, but at the end
 -P [size]       prepend debruijn pattern
 -q [fragment]   debruijn pattern offset
 -r              show raw bytes instead of hexpairs
 -s              show assembler
 -S [string]     append a string
 -v              show version
 -w [off:hex]    patch hexpairs at given offset
 -x              execute
 -X [hexpairs]   execute rop chain, using the stack provided
 -z              output in C string syntax
```

## First Example

```
$ cat hello.r
exit@syscall(1);

main@global() {
	exit(2);
}

$ ragg2 -a x86 -b 64 hello.r
48c7c00200000050488b3c2448c7c0010000000f054883c408c3
0x00000000   1                       48  dec eax
0x00000001   6             c7c002000000  mov eax, 2
0x00000007   1                       50  push eax
0x00000008   1                       48  dec eax
0x00000009   3                   8b3c24  mov edi, dword [esp]
0x0000000c   1                       48  dec eax
0x0000000d   6             c7c001000000  mov eax, 1
0x00000013   2                     0f05  syscall
0x00000015   1                       48  dec eax
0x00000016   3                   83c408  add esp, 8
0x00000019   1                       c3  ret

$ rasm2 -a x86 -b 64 -D 48c7c00200000050488b3c2448c7c0010000000f054883c408c3
0x00000000   7           48c7c002000000  mov rax, 2
0x00000007   1                       50  push rax
0x00000008   4                 488b3c24  mov rdi, qword [rsp]
0x0000000c   7           48c7c001000000  mov rax, 1
0x00000013   2                     0f05  syscall
0x00000015   4                 4883c408  add rsp, 8
0x00000019   1                       c3  ret
```

## Injectable machine code in different forms

Consider the following program:

```c
$ cat code1.c
int main()
{
	write(1, "Hello World\n", 13);
	exit(0);
}
```

One can get raw machine code of the above program like this:

```sh
$ ragg2 code1.c
eb0e66666666662e0f1f84000000000050bf01000000488d359f000000ba0d000000e81900000031ff89442404e85e00000031d289042489d059c30f1f440000897c24fc48897424f0895424ec8b5424fc895424dc488b7424f048897424d08b5424ec895424cc8b7c24dc488b7424d08b5424ccb8010000000f0548894424e0488b4424e089c1894c24c88b4424c8c3897c24fc8b7c24fc897c24ec8b7c24ecb83c0000000f0548894424f0488b4424f089c1894c24e88b4424e8c348656c6c6f20576f726c640a00
```

If you want it in a file, you may use the '-O' flag which uses the default filename or you may specify the output filename using '-o' option.

```sh
$ ragg2 -O code1.c
$ cat code1
eb0e66666666662e0f1f84000000000050bf01000000488d359f000000ba0d000000e81900000031ff89442404e85e00000031d289042489d059c30f1f440000897c24fc48897424f0895424ec8b5424fc895424dc488b7424f048897424d08b5424ec895424cc8b7c24dc488b7424d08b5424ccb8010000000f0548894424e0488b4424e089c1894c24c88b4424c8c3897c24fc8b7c24fc897c24ec8b7c24ecb83c0000000f0548894424f0488b4424f089c1894c24e88b4424e8c348656c6c6f20576f726c640a00
```

Printing as in raw

```sh
$ ragg2 -o code1.raw code1.c
$ cat code1.raw
eb0e66666666662e0f1f84000000000050bf01000000488d359f000000ba0d000000e81900000031ff89442404e85e00000031d289042489d059c30f1f440000897c24fc48897424f0895424ec8b5424fc895424dc488b7424f048897424d08b5424ec895424cc8b7c24dc488b7424d08b5424ccb8010000000f0548894424e0488b4424e089c1894c24c88b4424c8c3897c24fc8b7c24fc897c24ec8b7c24ecb83c0000000f0548894424f0488b4424f089c1894c24e88b4424e8c348656c6c6f20576f726c640a00
```

The above is a basic 'raw' output. ragg2 offers a number of output format options via `-f`. One can find it through ```ragg2 -h``` or ragg2's manpage.

```
-f format   output format (raw, c, pe, elf, mach0, python, javascript)
```

The following is 'c' format output - shellcode which can be readily used in your C program.

```sh
$ ragg2 -f c -o code1.c.c code1.c
$ cat code1.c.c
const unsigned char cstr[201] = ""\
  "\xeb\x0e\x66\x66\x66\x66\x66\x2e\x0f\x1f\x84\x00\x00\x00\x00\x00\x50\xbf\x01\x00\x00"\
  "\x00\x48\x8d\x35\x9f\x00\x00\x00\xba\r\x00\x00\x00\xe8\x19\x00\x00\x00"\
  "\x31\xff\x89\x44\x24\x04\xe8\x5e\x00\x00\x00\x31\xd2\x89\x04\x24\x89\xd0\x59\xc3\x0f"\
  "\x1f\x44\x00\x00\x89\x7c\x24\xfc\x48\x89\x74\x24\xf0\x89\x54\x24\xec\x8b\x54\x24\xfc\x89\x54\x24\xdc"\
  "\x48\x8b\x74\x24\xf0\x48\x89\x74\x24\xd0\x8b\x54\x24\xec\x89\x54\x24\xcc\x8b\x7c\x24\xdc\x48\x8b\x74\x24\xd0"\
  "\x8b\x54\x24\xcc\xb8\x01\x00\x00\x00\x0f\x05\x48\x89\x44\x24\xe0\x48\x8b\x44\x24\xe0\x89"\
  "\xc1\x89\x4c\x24\xc8\x8b\x44\x24\xc8\xc3\x89\x7c\x24\xfc\x8b\x7c\x24\xfc\x89\x7c\x24\xec\x8b"\
  "\x7c\x24\xec\xb8\x3c\x00\x00\x00\x0f\x05\x48\x89\x44\x24\xf0\x48\x8b\x44\x24\xf0\x89\xc1"\
  "\x89\x4c\x24\xe8\x8b\x44\x24\xe8\xc3\x48\x65\x6c\x6c\x6f\x20\x57\x6f\x72\x6c\x64\n\x00";
```

You may readily use it in C programs like this:

```c
$ cat code1.c.c
int main()
{
	const unsigned char cstr[201] = ""\
	  "\xeb\x0e\x66\x66\x66\x66\x66\x2e\x0f\x1f\x84\x00\x00\x00\x00\x00\x50\xbf\x01\x00\x00"\
	  "\x00\x48\x8d\x35\x9f\x00\x00\x00\xba\r\x00\x00\x00\xe8\x19\x00\x00\x00"\
	  "\x31\xff\x89\x44\x24\x04\xe8\x5e\x00\x00\x00\x31\xd2\x89\x04\x24\x89\xd0\x59\xc3\x0f"\
	  "\x1f\x44\x00\x00\x89\x7c\x24\xfc\x48\x89\x74\x24\xf0\x89\x54\x24\xec\x8b\x54\x24\xfc\x89\x54\x24\xdc"\
	  "\x48\x8b\x74\x24\xf0\x48\x89\x74\x24\xd0\x8b\x54\x24\xec\x89\x54\x24\xcc\x8b\x7c\x24\xdc\x48\x8b\x74\x24\xd0"\
	  "\x8b\x54\x24\xcc\xb8\x01\x00\x00\x00\x0f\x05\x48\x89\x44\x24\xe0\x48\x8b\x44\x24\xe0\x89"\
	  "\xc1\x89\x4c\x24\xc8\x8b\x44\x24\xc8\xc3\x89\x7c\x24\xfc\x8b\x7c\x24\xfc\x89\x7c\x24\xec\x8b"\
	  "\x7c\x24\xec\xb8\x3c\x00\x00\x00\x0f\x05\x48\x89\x44\x24\xf0\x48\x8b\x44\x24\xf0\x89\xc1"\
	  "\x89\x4c\x24\xe8\x8b\x44\x24\xe8\xc3\x48\x65\x6c\x6c\x6f\x20\x57\x6f\x72\x6c\x64\n\x00";
	
	void (*func)() = cstr;
	func();
	return 0;
}
```

Compile and run it.

```
$ gcc code1.c.c -o code1.c.elf -zexecstack
code1.c.c: In function ‘main’:
code1.c.c:14:19: warning: initialization from incompatible pointer type [-Wincompatible-pointer-types]
  void (*func)() = cstr;
                   ^~~~
$ ./code1.c.elf
Hello World
```

Similar to how the above code is readily usable in C programs, ragg2 can emit python-ready and js-ready code too(using the '-f python' and -f javascript' options).

To generate an executable binary for your native architecture, you may use the '-F' option.

```sh
$ ragg2 -F -o code1.elf code1.c
$ ./code1.elf
Hello World
```

You may specify the binary format output.

```sh
$ ragg2 -f elf -o code1_f.elf code1.c
$ ./code1_f.elf
Hello World
```

or

```sh
$ ragg2 -f mach0 -o code1_f.mach0 code1.c
$ file code1_f.mach0
code1_f.mach0: Mach-O 64-bit x86_64 executable
```

Same with the 'PE' format.

In the above examples, the target architecture is the architecture of your machine. But target architecture can explicitly be specified using the '-a' option.

```sh
$ ragg2 -f raw -a x86 -b 32 code1.c
eb4e66666666662e0f1f840000000000575683ec108b4424208b4c241c8b54241c8b742420bf66000000894c240889f18944240489f85389d3cd805b8944240c8b44240c8904248b042483c4105e5fc3535683ec24e8000000005881c0661e0000b9010000008d9083e2ffffbe0d000000c704240100000089542404c74424080d00000089c389442420894c241c89742418e82900000031c9c70424000000008b5c242089442414894c2410e86f00000031c083c4245e5bc30f1f80000000005553575683ec148b4424308b4c242c8b5424288b7424288b7c242c8b5c2430bd04000000894c240c89f98954240889da8944240489e85389f3cd805b894424108b4424108904248b042483c4145e5f5b5dc366666666662e0f1f84000000000083ec088b44240c8b4c240cba0100000089042489d05389cbcd805b8944240483c408c348656c6c6f20576f726c640a00
```

The '-b' option can be used to specify the bits. One can see the supported architectures from ragg2's help or manpage.

```
 -a [arch]       select architecture (x86, mips, arm)
 -b [bits]       register size (32, 64, ..)
```

The '-r' flag can be used to generate binary output instead of the above hex-string style output.

```sh
$ ragg2 -f raw -r code1.c | rax2 -S
efbfbd31efbfbdefbfbd4424efbfbd5e31d28924efbfbdefbfbd59efbfbd44efbfbd7c24efbfbd48efbfbd
7424efbfbdefbfbd5424efbfbdefbfbd5424efbfbdefbfbd5424efbfbd48efbfbd7424efbfbd48efbfbd74
24d08b5424efbfbdefbfbd5424cc8b7c24efbfbd48efbfbd7424d08b5424ccb848efbfbd4424efbfbd48ef
bfbd4424efbfbdefbfbdefbfbdefbfbd4c24c88b4424efbfbdc3897c24efbfbdefbfbd7c24efbfbdefbfbd
7c24efbfbdefbfbd7c24efbfbdefbfbd3c48efbfbd4424efbfbd48efbfbd4424efbfbdefbfbdefbfbdefbf
bd4c24efbfbdefbfbd4424efbfbdefbfbd48656c6c6f20576f726c640a
```

```
$ ragg2 -f raw -a x86 -b 64 -r code1.c | xxd
00000000: eb0e 6666 6666 662e 0f1f 8400 0000 0000  ..fffff.........
00000010: 50bf 0100 0000 488d 359f 0000 00ba 0d00  P.....H.5.......
00000020: 0000 e819 0000 0031 ff89 4424 04e8 5e00  .......1..D$..^.
00000030: 0000 31d2 8904 2489 d059 c30f 1f44 0000  ..1...$..Y...D..
00000040: 897c 24fc 4889 7424 f089 5424 ec8b 5424  .|$.H.t$..T$..T$
00000050: fc89 5424 dc48 8b74 24f0 4889 7424 d08b  ..T$.H.t$.H.t$..
00000060: 5424 ec89 5424 cc8b 7c24 dc48 8b74 24d0  T$..T$..|$.H.t$.
00000070: 8b54 24cc b801 0000 000f 0548 8944 24e0  .T$........H.D$.
00000080: 488b 4424 e089 c189 4c24 c88b 4424 c8c3  H.D$....L$..D$..
00000090: 897c 24fc 8b7c 24fc 897c 24ec 8b7c 24ec  .|$..|$..|$..|$.
000000a0: b83c 0000 000f 0548 8944 24f0 488b 4424  .<.....H.D$.H.D$
000000b0: f089 c189 4c24 e88b 4424 e8c3 4865 6c6c  ....L$..D$..Hell
000000c0: 6f20 576f 726c 640a 00				   o World..
```

Using '-z' flag instead of '-r' would generate a C-style hex-string output.

```sh
$ ragg2 -f raw -z code1.c
"\xeb\x0e\x66\x66\x66\x66\x66\x2e\x0f\x1f\x84\x00\x00\x00\x00\x00\x50\xbf\x01\x00\x00\x00\x48\x8d\x35\x9f\x00\x00\x00\xba\x0d\x00\x00\x00\xe8\x19\x00\x00\x00\x31\xff\x89\x44\x24\x04\xe8\x5e\x00\x00\x00\x31\xd2\x89\x04\x24\x89\xd0\x59\xc3\x0f\x1f\x44\x00\x00\x89\x7c\x24\xfc\x48\x89\x74\x24\xf0\x89\x54\x24\xec\x8b\x54\x24\xfc\x89\x54\x24\xdc\x48\x8b\x74\x24\xf0\x48\x89\x74\x24\xd0\x8b\x54\x24\xec\x89\x54\x24\xcc\x8b\x7c\x24\xdc\x48\x8b\x74\x24\xd0\x8b\x54\x24\xcc\xb8\x01\x00\x00\x00\x0f\x05\x48\x89\x44\x24\xe0\x48\x8b\x44\x24\xe0\x89\xc1\x89\x4c\x24\xc8\x8b\x44\x24\xc8\xc3\x89\x7c\x24\xfc\x8b\x7c\x24\xfc\x89\x7c\x24\xec\x8b\x7c\x24\xec\xb8\x3c\x00\x00\x00\x0f\x05\x48\x89\x44\x24\xf0\x48\x8b\x44\x24\xf0\x89\xc1\x89\x4c\x24\xe8\x8b\x44\x24\xe8\xc3\x48\x65\x6c\x6c\x6f\x20\x57\x6f\x72\x6c\x64\x0a\x00"
```

Instead of using C, a domain specific language designed for ragg2 can also be used - you may refer to [this](https://book.rada.re/tools/ragg2/lang.html) page.

The `-e` option can be used if you want to input the code as an argument to ragg2 and not as a file. Note that you can only use ragg2 language here(and not C).

```sh
$ ragg2 -e "exit@syscall(60); write@syscall(4); main@global(128) {write(1, \"Hello World\n\", 13); exit(0);}" 
554889e54881ec8000000048c7c00d00000050c7452048656c6cc745246f20576fc74528726c640ac7452c00000000c7453000000000488d452048898518000000488b45185048c7c00100000050488b3c24488b742408488b54241048c7c0040000000f054883c41848c7c00000000050488b3c2448c7c03c0000000f054883c4084881c4800000005dc3
```

Other options like architecture, bits, output file format etc., can be used here too.

```sh
$ ragg2 -e "exit@syscall(60); write@syscall(4); main@global(128) {write(1, \"Hello World\n\", 13); exit(0);}"  -f elf -o output.elf
$ file output.elf
output.elf: ELF 64-bit LSB executable, x86-64, version 1 (SYSV), statically linked, corrupted section header size
$ ragg2 -e "exit@syscall(60); write@syscall(4); main@global(128) {write(1, \"Hello World\n\", 13); exit(0);}"  -f c
const unsigned char cstr[139] = ""\
  "\x55\x48\x89\xe5\x48\x81\xec\x80\x00\x00\x00\x48\xc7\xc0\r\x00\x00\x00\x50\xc7"\
  "\x45\x20\x48\x65\x6c\x6c\xc7\x45\x24\x6f\x20\x57\x6f\xc7\x45\x28\x72\x6c\x64\n\xc7\x45\x2c\x00\x00\x00\x00\xc7\x45\x30\x00\x00"\
  "\x00\x00\x48\x8d\x45\x20\x48\x89\x85\x18\x00\x00\x00\x48\x8b\x45\x18\x50\x48\xc7\xc0\x01"\
  "\x00\x00\x00\x50\x48\x8b\x3c\x24\x48\x8b\x74\x24\x08\x48\x8b\x54\x24\x10\x48\xc7\xc0\x04\x00\x00"\
  "\x00\x0f\x05\x48\x83\xc4\x18\x48\xc7\xc0\x00\x00\x00\x00\x50\x48\x8b\x3c\x24\x48\xc7"\
  "\xc0\x3c\x00\x00\x00\x0f\x05\x48\x83\xc4\x08\x48\x81\xc4\x80\x00\x00\x00"\
  "\x5d\xc3";
```

In case you just want to execute the input, you may use the `-x` option.

```
-x          execute (just-in-time)
```

```sh
$ cat code1.c
int main()
{
	write(1, "Hello World\n", 13);
	exit(0);
}
$ ragg2 -x code1.c
Hello World
```
