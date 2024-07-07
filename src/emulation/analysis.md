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
