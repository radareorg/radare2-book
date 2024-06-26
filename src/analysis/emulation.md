# Emulation

Understanding the distinction between static analysis and dynamic analysis is crucial in reverse engineering. radare2 uses two different kind of instruction information to perform static analysis:

* OpType, Instruction Family plus other static details
* ESIL expression associated

Radare2 employs its own intermediate language and virtual machine, known as ESIL, for partial emulation (or imprecise full emulation).

Radare2's [ESIL](../disassembling/esil.md) supports partial emulation across all platforms by evaluating those expressions.

## Use Cases

There are many use cases for ESIL in radare2, not just bare code emulation:

* Resolve indirect branches
* Determine the likelity of a branch
* Search memory addresses matching complex nested conditionals
* Find out computed pointer references (`aae` or `/re`)
* Execution of a function portion
* Simulate behaviour of syscalls and imports
* r2wars (let's play!)

To view the ESIL representation of your program, use the `ao~esil` command or enable the `asm.esil` configuration variable. This will let you verify how the code is uplifted from assembly to ESIL and understand better how that works internally.

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

## Commands

To manually set up imprecise ESIL emulation, run the following sequence of commands:

- `aei` to initialize the ESIL VM
- `aeim` to initialize ESIL VM memory (stack)
- `aeip` to set the initial ESIL VM IP (instruction pointer)
- a sequence of `aer` commands to set the initial register values.

While performing emulation, please remember that the ESIL VM cannot emulate external calls system calls, nor SIMD instructions. Thus, the most common scenario is to emulate only a small chunk of code like encryption, decryption, unpacking, or a calculation.

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

## Options

The emulation can be triggered at analysis, runtime or at will with full manual control, in other words, the user can decide what and how to use ESIL.

To change some of the behaviours of the emulation engine in radare2 you can use the following options:

`[0x00000000]> e??esil.`

- esil.addr.size: maximum address size in accessed by the ESIL VM
- esil.breakoninvalid: break esil execution when instruction is invalid
- esil.dfg.mapinfo: use mapinfo for esil dfg
- esil.dfg.maps: set ro maps for esil dfg
- esil.exectrap: trap when executing code in non-executable memory
- esil.fillstack: initialize ESIL stack with (random, debruijn, sequence, zeros, ...)
- esil.gotolimit: maximum number of gotos per ESIL expression
- esil.iotrap: invalid read or writes produce a trap exception
- esil.maxsteps: If !=0 defines the maximum amount of steps to perform on aesu/aec/..
- esil.mdev.range: specify a range of memory to be handled by cmd.esil.mdev
- esil.nonull: prevent memory read, memory write at null pointer
- esil.prestep: step before esil evaluation in `de` commands
- esil.romem: set memory as read-only for ESIL
- esil.stack.addr: set stack address in ESIL VM
- esil.stack.depth: number of elements that can be pushed on the esilstack
- esil.stack.pattern: specify fill pattern to initialize the stack (0, w, d, i)
- esil.stack.size: set stack size in ESIL VM
- esil.stats: statistics from ESIL emulation stored in sdb
- esil.timeout: a timeout (in seconds) for when we should give up emulating
- esil.verbose: show ESIL verbose level (0, 1, 2)

## Problems

There are several situations where emulation will not work as expected or solve your problems. It is important to understand those situations to avoid undesired surprises and know how to workaround them.

* Path explossion (too many execution or unknown paths to follow)
* Incorrect stack size or contents (aeim)
* Thread local storage (custom segments or memory layouts) not defined
* Unimplemented instructions (Use ahe to set analysis hints)
* Undefined behaviour (analy
* Custom Ops (requires esil plugins)
* Don't go into Syscall / Imports implementations

## Emulation in the Analysis Loop

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

Apart from the basic ESIL VM setup, you can change its behavior with other options located in the `emu.` and `esil.` configuration namespaces.

## Debugging with ESIL

The debugger APIs can be configured to use different backends, to communicate with a local or remote GDB server, use the native debugger, a specific virtualization or emulation engine like Unicorn or BOCHS, but there's also an ESIL backend.

```
[0x00000000]> dL
0  ---  bf       LGPL3
1  ---  bochs    LGPL3
2  ---  esil     LGPL3
3  ---  evm      LGPL3
4  ---  gdb      LGPL3
5  ---  io       MIT
6  dbg  native   LGPL3
7  ---  null     MIT
8  ---  qnx      LGPL3
9  ---  rap      LGPL3
[0x00000000]> dL esil
[0x00000000]>
```

After this command, you can use any of the `d` sub-commands to change register values, step or skip instructions, set breakpoints, etc. but using the internal emulation logic of ESIL.
