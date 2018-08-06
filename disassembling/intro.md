# Disassembling

Disassembling in radare is just a way to represent an array of bytes. It is handled as a special print mode within `p` command.

In the old times, when the radare core was smaller, the disassembler was handled by an external rsc file. That is, radare first dumped current block into a file, and then simply called `objdump` configured to disassemble for Intel, ARM or other supported architectures.

It was a working and unix friendly solution, but it was inefficient as it repeated the same expensive actions over and over, because there were no caches. As a result, scrolling was terribly slow.

So there was a need to create a generic disassembler library to support multiple plugins for different architectures. We can list the current loaded plugins with

```
$ rasm2 -L
```

Or from inside radare2:

```
> e asm.arch=??
```

This was many years before capstone appeared. So r2 was using udis86 and olly disassemblers, many gnu (from binutils).

Nowadays, the disassembler support is one of the basic features of radare. It now has many options, endianness, including target architecture flavor and disassembler variants, among other things.

To see the disassembly, use the `pd` command. It accepts a numeric argument to specify how many opcodes of current block you want to see. Most of the commands in radare consider the current block size as the default limit for data input. If you want to disassemble more bytes, set a new block size using the `b` command.

```
[0x00000000]> b 100    ; set block size to 100
[0x00000000]> pd       ; disassemble 100 bytes
[0x00000000]> pd 3     ; disassemble 3 opcodes
[0x00000000]> pD 30    ; disassemble 30 bytes
```

The `pD` command works like `pd` but accepts the number of input bytes as its argument, instead of the number of opcodes.

The "pseudo" syntax may be somewhat easier for a human to understand than the default assembler notations. But it can become annoying if you read lots of code. To play with it:

```
[0x00405e1c]> e asm.pseudo = true
[0x00405e1c]> pd 3
		  ; JMP XREF from 0x00405dfa (fcn.00404531)
		  0x00405e1c    488b9424a80. rdx = [rsp+0x2a8]
		  0x00405e24    64483314252. rdx ^= [fs:0x28]
		  0x00405e2d    4889d8       rax = rbx

[0x00405e1c]> e asm.syntax = intel
[0x00405e1c]> pd 3
		  ; JMP XREF from 0x00405dfa (fcn.00404531)
		  0x00405e1c    488b9424a80. mov rdx, [rsp+0x2a8]
		  0x00405e24    64483314252. xor rdx, [fs:0x28]
		  0x00405e2d    4889d8       mov rax, rbx

[0x00405e1c]> e asm.syntax=att
[0x00405e1c]> pd 3
		  ; JMP XREF from 0x00405dfa (fcn.00404531)
		  0x00405e1c    488b9424a80. mov 0x2a8(%rsp), %rdx
		  0x00405e24    64483314252. xor %fs:0x28, %rdx
		  0x00405e2d    4889d8       mov %rbx, %rax
```

