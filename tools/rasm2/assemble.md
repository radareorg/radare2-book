## Assembler

Assembling is the action to take a computer instruction in human readable form (using mnemonics) and convert that into a bunch of bytes that can be executed by a machine.

In radare2, the assembler and disassembler logic is implemented in the r_asm_* API, and can be used with the pa and pad commands from the commandline as well as using `rasm2`.

Rasm2 can be used to quickly copy-paste hexpairs that represent a given machine instruction. The following line is assembling this mov instruction for x86/32.

```
$ rasm2 -a x86 -b 32 'mov eax, 33'
b821000000
```

Apart from the specifying the input as an argument, you can also pipe it to rasm2:

```
$ echo 'push eax;nop;nop' | rasm2 -f -
5090
```

As you have seen, rasm2 can assemble one or many instructions. In line by separating them with a semicolon `;`, but can also read that from a file, using generic nasm/gas/.. syntax and directives. You can check the rasm2 manpage for more details on this.

The `pa` and `pad` are a subcommands of print, what means they will only print assembly or disassembly. In case you want to actually write the instruction it is required to use `wa` or `wx` commands with the assembly string or bytes appended.

The assembler understands the following input languages and their flavors: `x86` (Intel and AT&T variants), `olly` (OllyDBG syntax), `powerpc` (PowerPC), `arm` and `java`. For Intel syntax, rasm2 tries to mimic NASM or GAS.

There are several examples in the rasm2 source code directory. Consult them to understand how you can assemble a raw binary file from a rasm2 description.

Lets create an assembly file called `selfstop.rasm`:

```asm
;
; Self-Stop shellcode written in rasm for x86
;
; --pancake
;

.arch x86
.equ base 0x8048000
.org 0x8048000  ; the offset where we inject the 5 byte jmp

selfstop:
  push 0x8048000
  pusha
  mov eax, 20
  int 0x80

  mov ebx, eax
  mov ecx, 19
  mov eax, 37
  int 0x80
  popa
  ret
;
; The call injection
;

  ret
```

Now we can assemble it in place:

```
[0x00000000]> e asm.bits = 32
[0x00000000]> wx `!rasm2 -f a.rasm`
[0x00000000]> pd 20
	   0x00000000    6800800408   push 0x8048000 ;  0x08048000
	   0x00000005    60           pushad
	   0x00000006    b814000000   mov eax, 0x14 ;  0x00000014
	   0x0000000b    cd80         int 0x80
		  syscall[0x80][0]=?
	   0x0000000d    89c3         mov ebx, eax
	   0x0000000f    b913000000   mov ecx, 0x13 ;  0x00000013
	   0x00000014    b825000000   mov eax, 0x25 ;  0x00000025
	   0x00000019    cd80         int 0x80
		  syscall[0x80][0]=?
	   0x0000001b    61           popad
	   0x0000001c    c3           ret
	   0x0000001d    c3           ret
```

### Visual mode

Assembling also is accessible in radare2 visual mode through pressing `A` key to insert the assembly in the current offset.

The cool thing of writing assembly using the visual assembler interface that the changes are done in memory until you press enter.

So you can check the size of the code and which instructions is overlapping before commiting the changes.

