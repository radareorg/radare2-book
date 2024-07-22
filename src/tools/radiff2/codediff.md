## Code Diffing

This tool can be also used to compare code, which can be really handy when analyzing two shellcodes, functions or libraries, looking for changes in its code.

To understand this feature we will start by using the basic delta diffing from the previous data analysis example but using the `-D` flag to display the changes in assembly instead of hexadecimal data.

Note that radiff2 permits to specify the arch/bits/.. settings using these flags:

```
$ radiff2 -h | grep -e arch -e bits
-a [arch]  specify architecture plugin to use (x86, arm, ..)
-b [bits]  specify register size for arch (16 (thumb), 32, 64, ..)
```

Let's test it out!

```
$ cat 1
j0X40PZHf5sOf5A0PRXRj0X40hXXshXf5wwPj0X4050binHPTXRQSPTUVWaPYS4J4A
$ cat 2
j0X4PX0PZHf5sOf5A0PRXRj0X40hXXshXf5wwPj0X4050binHPTXRQSPTUVWaPYS4J
```

Wen can compare the changes as disassembly like this:
```
$ radiff2 -D 5 6
  push 0x30
  pop rax
  xor al, 0x30
+ push rax
+ pop rax
  pop rdx
  xor ax, 0x4f73
  xor ax, 0x3041
  push rax
```

So we can see that the second file added an extra `push rax + pop rax` which is basically a nop, and maybe this was introduced to bypass some signatures to avoid being detected.

If you are looking for some more advanced bindiffing tool for code you may want to have a look at the `r2diaphora` and the zignatures features under the `z` command in the radare2 shell.

It's also possible to compare the changes between two functions using the `-g` flag, but you'll need to analize both binaries using the `-A` flag, and tell the symbol name as target.


