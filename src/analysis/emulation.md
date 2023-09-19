# Emulation

One of the most important things to remember in reverse engineering is
a core difference between static analysis and dynamic analysis. As many already
know, static analysis suffers from the path explosion problem, which is impossible
to solve even in the most basic way without at least partial emulation.

Thus, many professional reverse engineering tools use code emulation while
performing an analysis of binary code, and radare2 is no different here.

For partial emulation (or imprecise full emulation) radare2 uses its own
intermediate language and virtual machine called [ESIL](../disassembling/esil.md).

Radare2 supports this kind of partial emulation for all platforms that
implement ESIL "uplifting" (x86/x86_64, ARM, arm64, MIPS, powerpc, sparc, AVR, 8051, Gameboy, ...).

One of the most common usages of such emulation is to calculate
indirect jumps and conditional jumps.

To see the ESIL representation of a program, one can use the `ao` command or enable the `asm.esil` configuration
variable to check if the program was correctly uplifted and to grasp how ESIL works:

```
[0x00001660]> pdf
. (fcn) fcn.00001660 40
|   fcn.00001660 ();
|     ; CALL XREF from 0x00001713 (entry2.fini)
|     0x00001660  lea rdi, obj.__progname      ; 0x207220
|     0x00001667  push rbp
|     0x00001668  lea rax, obj.__progname      ; 0x207220
|     0x0000166f  cmp rax, rdi
|     0x00001672  mov rbp, rsp
| .-< 0x00001675  je 0x1690
| |   0x00001677  mov rax, qword [reloc._ITM_deregisterTMCloneTable] ; [0x206fd8:8]=0
| |   0x0000167e  test rax, rax
|.--< 0x00001681  je 0x1690
|||   0x00001683  pop rbp
|||   0x00001684  jmp rax
|``-> 0x00001690  pop rbp
`     0x00001691  ret
[0x00001660]> e asm.esil=true
[0x00001660]> pdf
. (fcn) fcn.00001660 40
|   fcn.00001660 ();
|     ; CALL XREF from 0x00001713 (entry2.fini)
|     0x00001660  0x205bb9,rip,+,rdi,=
|     0x00001667  rbp,8,rsp,-=,rsp,=[8]
|     0x00001668  0x205bb1,rip,+,rax,=
|     0x0000166f  rdi,rax,==,$z,zf,=,$b64,cf,=,$p,pf,=,$s,sf,=,$o,of,=
|     0x00001672  rsp,rbp,=
| .-< 0x00001675  zf,?{,5776,rip,=,}
| |   0x00001677  0x20595a,rip,+,[8],rax,=
| |   0x0000167e  0,rax,rax,&,==,$z,zf,=,$p,pf,=,$s,sf,=,$0,cf,=,$0,of,=
|.--< 0x00001681  zf,?{,5776,rip,=,}
|||   0x00001683  rsp,[8],rbp,=,8,rsp,+=
|||   0x00001684  rax,rip,=
|``-> 0x00001690  rsp,[8],rbp,=,8,rsp,+=
`     0x00001691  rsp,[8],rip,=,8,rsp,+=
```

To manually set up imprecise ESIL emulation, run the following sequence of commands:

- `aei` to initialize the ESIL VM
- `aeim` to initialize ESIL VM memory (stack)
- `aeip` to set the initial ESIL VM IP (instruction pointer)
- a sequence of `aer` commands to set the initial register values.

While performing emulation, please remember that the ESIL VM cannot emulate external calls
system calls, nor SIMD instructions. Thus, the most common scenario is to
emulate only a small chunk of code like encryption, decryption, unpacking, or
a calculation.

After successfully setting up the ESIL VM, we can interact with it like a normal debugging session.
The command interface for the ESIL VM is almost identical to the debugging interface:

- `aes` to step (or `s` key in visual mode)
- `aesi` to step over function calls
- `aesu <address>` to step until some specified address
- `aesue <ESIL expression>` to step until some specified ESIL expression is met
- `aec` to continue until break (Ctrl-C). This one is rarely used due to the omnipresence of external calls

In visual mode, all of the debugging hotkeys will also work in ESIL emulation mode.

In addition to normal emulation, it's also possible to record and replay sessions:

- `aets` to list all current ESIL R&R sessions
- `aets+` to create a new one
- `aesb` to step back in the current ESIL R&R session

You can read more about this operation mode in the [Reverse Debugging](../debugger/revdebug.md) chapter.

## Emulation in analysis loop

Aside from manual emulation, automatic emulation is also possible in the analysis loop.
For example, the `aaaa` command performs the ESIL emulation stage, among others.
To disable or enable ESIL analysis, set the `anal.esil` configuration variable.

Furthermore, `emu.write` allows the ESIL VM to modify memory. However, enabling it
might be quite dangerous, especially when analyzing malware. Regardless, it is
still sometimes required, particularly when deobfuscating or unpacking code. To
show the emulation process, you can set `asm.emu` variable which will show
calculated register and memory values as comments in the disassembly:

```
[0x00001660]> e asm.emu=true
[0x00001660]> pdf
. (fcn) fcn.00001660 40
|   fcn.00001660 ();
|     ; CALL XREF from 0x00001713 (entry2.fini)
|     0x00001660  lea rdi, obj.__progname ; 0x207220 ; rdi=0x207220 -> 0x464c457f
|     0x00001667  push rbp                ; rsp=0xfffffffffffffff8
|     0x00001668  lea rax, obj.__progname ; 0x207220 ; rax=0x207220 -> 0x464c457f
|     0x0000166f  cmp rax, rdi            ; zf=0x1 -> 0x2464c45 ; cf=0x0 ; pf=0x1 -> 0x2464c45 ; sf=0x0 ; of=0x0
|     0x00001672  mov rbp, rsp            ; rbp=0xfffffffffffffff8
| .-< 0x00001675  je 0x1690               ; rip=0x1690 -> 0x1f0fc35d ; likely
| |   0x00001677  mov rax, qword [reloc._ITM_deregisterTMCloneTable] ; [0x206fd8:8]=0 ; rax=0x0
| |   0x0000167e  test rax, rax           ; zf=0x1 -> 0x2464c45 ; pf=0x1 -> 0x2464c45 ; sf=0x0 ; cf=0x0 ; of=0x0
|.--< 0x00001681  je 0x1690               ; rip=0x1690 -> 0x1f0fc35d ; likely
|||   0x00001683  pop rbp                 ; rbp=0xffffffffffffffff -> 0x4c457fff ; rsp=0x0
|||   0x00001684  jmp rax                 ; rip=0x0 ..
|``-> 0x00001690  pop rbp                 ; rbp=0x10102464c457f ; rsp=0x8 -> 0x464c457f
`     0x00001691  ret                     ; rip=0x0 ; rsp=0x10 -> 0x3e0003
```

Note the comments containing `likely`, which indicate conditional jumps likely to be taken by ESIL emulation.

Apart from the basic ESIL VM setup, you can change its behavior with other options located
in the `emu.` and `esil.` configuration namespaces.

For manipulating ESIL working with memory and the stack, you may use the following options:

- `esil.stack` to enable or disable a temporary stack for `asm.emu` mode
- `esil.stack.addr` to set stack address in the ESIL VM (like `aeim` command)
- `esil.stack.size` to set stack size in the ESIL VM (like `aeim` command)
- `esil.stack.depth` limits the number of elements that can be pushed onto the esilstack
- `esil.romem` sets memory as read-only for ESIL
- `esil.fillstack` and `esil.stack.pattern` fill the ESIL stack with various patterns when initialized (random, debrujn, sequence, zeros, ...)
- `esil.nonull` prevents memory read, memory write at null pointer
