# Emulation

One of the most important things to remember in reverse engineering is
a core difference between static analysis and dynamic analysis. As many already
know, static analysis suffers from the path explosion problem, which is impossible
to solve even in the most basic way without at least a partial emulation.

Thus many professional reverse engineering tools use code emulation while
performing an analysis of binary code, and radare2 is no difference here.

For partial emulation (or imprecise full emulation) radare2 uses its own
[ESIL](../disassembling/esil.md) intermediate language and virtual machine.

Radare2 supports this kind of partial emulation for all platforms that
implement ESIL uplifting (x86/x86_64, ARM, arm64, MIPS, powerpc, sparc, AVR, 8051, Gameboy, ...).

One of the most common usages of such emulation is to calculate
indirect jumps and conditional jumps.

To see the ESIL representation of the program one can use the `ao` command or enable the `asm.esil` configuration
variable, to check if the program uplifted correctly, and to grasp how ESIL works:

```
[0x00001660]> pdf
. (fcn) fcn.00001660 40
│   fcn.00001660 ();
│     ; CALL XREF from 0x00001713 (entry2.fini)
│     0x00001660  lea rdi, obj.__progname      ; 0x207220
│     0x00001667  push rbp
│     0x00001668  lea rax, obj.__progname      ; 0x207220
│     0x0000166f  cmp rax, rdi
│     0x00001672  mov rbp, rsp
│ .─< 0x00001675  je 0x1690
│ │   0x00001677  mov rax, qword [reloc._ITM_deregisterTMCloneTable] ; [0x206fd8:8]=0
│ │   0x0000167e  test rax, rax
│.──< 0x00001681  je 0x1690
│││   0x00001683  pop rbp
│││   0x00001684  jmp rax
│``─> 0x00001690  pop rbp
`     0x00001691  ret
[0x00001660]> e asm.esil=true
[0x00001660]> pdf
. (fcn) fcn.00001660 40
│   fcn.00001660 ();
│     ; CALL XREF from 0x00001713 (entry2.fini)
│     0x00001660  0x205bb9,rip,+,rdi,=
│     0x00001667  rbp,8,rsp,-=,rsp,=[8]
│     0x00001668  0x205bb1,rip,+,rax,=
│     0x0000166f  rdi,rax,==,$z,zf,=,$b64,cf,=,$p,pf,=,$s,sf,=,$o,of,=
│     0x00001672  rsp,rbp,=
│ .─< 0x00001675  zf,?{,5776,rip,=,}
│ │   0x00001677  0x20595a,rip,+,[8],rax,=
│ │   0x0000167e  0,rax,rax,&,==,$z,zf,=,$p,pf,=,$s,sf,=,$0,cf,=,$0,of,=
│.──< 0x00001681  zf,?{,5776,rip,=,}
│││   0x00001683  rsp,[8],rbp,=,8,rsp,+=
│││   0x00001684  rax,rip,=
│``─> 0x00001690  rsp,[8],rbp,=,8,rsp,+=
`     0x00001691  rsp,[8],rip,=,8,rsp,+=
```

To manually setup the ESIL imprecise emulation you need to run this command sequence:

- `aei` to initialize ESIL VM
- `aeim` to initialize ESIL VM memory (stack)
- `aeip` to set the initial ESIL VM IP (instruction pointer)
- a sequence of `aer` commands to set the initial register values.

While performing emulation, please remember, that ESIL VM cannot emulate external calls
or system calls, along with SIMD instructions. Thus the most common scenario is to
emulate only a small chunk of the code, like encryption/decryption, unpacking or
calculating something.

After we successfully set up the ESIL VM we can interact with it like with a usual debugging mode.
Commands interface for ESIL VM is almost identical to the debugging one:

- `aes` to step (or `s` key in visual mode)
- `aesi` to step over the function calls
- `aesu <address>` to step until some specified address
- `aesue <ESIL expression>` to step until some specified ESIL expression met
- `aec` to continue until break (Ctrl-C), this one is rarely used though, due to the omnipresence
	of external calls
- `aecu <address>` to continue until some specified address

In visual mode, all of the debugging hotkeys will work also in ESIL emulation mode.

Along with usual emulation, there is a possibility to record and replay mode:

- `aets` to list all current ESIL R&R sessions
- `aets+` to create a new one
- `aesb` to step back in the current ESIL R&R session

More about this operation mode you can read in [Reverse Debugging](../debugger/revdebug.md) chapter.

## Emulation in analysis loop

Apart from the manual emulation mode, it can be used automatically in the analysis loop.
For example, the `aaaa` command performs the ESIL emulation stage along with others.
To disable or enable its usage you can use `anal.esil` configuration variable.
There is one more important option, though setting it might be quite dangerous,
especially in the case of malware - `emu.write` which allows ESIL VM to modify memory.
Sometimes it is required though, especially in the process of deobfuscating or unpacking code.

To show the process of emulation you can set `asm.emu` variable, which will show calculated
register and memory values in disassembly comments:

```
[0x00001660]> e asm.emu=true
[0x00001660]> pdf
. (fcn) fcn.00001660 40
│   fcn.00001660 ();
│     ; CALL XREF from 0x00001713 (entry2.fini)
│     0x00001660  lea rdi, obj.__progname ; 0x207220 ; rdi=0x207220 -> 0x464c457f
│     0x00001667  push rbp                ; rsp=0xfffffffffffffff8
│     0x00001668  lea rax, obj.__progname ; 0x207220 ; rax=0x207220 -> 0x464c457f
│     0x0000166f  cmp rax, rdi            ; zf=0x1 -> 0x2464c45 ; cf=0x0 ; pf=0x1 -> 0x2464c45 ; sf=0x0 ; of=0x0
│     0x00001672  mov rbp, rsp            ; rbp=0xfffffffffffffff8
│ .─< 0x00001675  je 0x1690               ; rip=0x1690 -> 0x1f0fc35d ; likely
│ │   0x00001677  mov rax, qword [reloc._ITM_deregisterTMCloneTable] ; [0x206fd8:8]=0 ; rax=0x0
│ │   0x0000167e  test rax, rax           ; zf=0x1 -> 0x2464c45 ; pf=0x1 -> 0x2464c45 ; sf=0x0 ; cf=0x0 ; of=0x0
│.──< 0x00001681  je 0x1690               ; rip=0x1690 -> 0x1f0fc35d ; likely
│││   0x00001683  pop rbp                 ; rbp=0xffffffffffffffff -> 0x4c457fff ; rsp=0x0
│││   0x00001684  jmp rax                 ; rip=0x0 ..
│``─> 0x00001690  pop rbp                 ; rbp=0x10102464c457f ; rsp=0x8 -> 0x464c457f
`     0x00001691  ret                     ; rip=0x0 ; rsp=0x10 -> 0x3e0003
```

Note here `likely` comments, which indicates that ESIL emulation predicted for particular
conditional jump to happen.

Apart from the basic ESIL VM setup, you can change the behavior with other options located
in `emu.` and `esil.` configuration namespaces.

For manipulating ESIL working with memory and stack you can use the following options:

- `esil.stack` to enable or disable temporary stack for `asm.emu` mode
- `esil.stack.addr` to set stack address in ESIL VM (like `aeim` command)
- `esil.stack.size` to set stack size in ESIL VM (like `aeim` command)
- `esil.stack.depth` limits the number of PUSH operations into the stack
- `esil.romem` specifies read-only access to the ESIL memory
- `esil.fillstack` and `esil.stack.pattern` allows you to use a various pattern for filling ESIL VM
	stack upon initialization
- `esil.nonull` when set stops ESIL execution upon NULL pointer read or write.
