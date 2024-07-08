## R2Wars

R2wars is an exciting and unique way to use radare2, allow users to engage in programming duels using real assembly languages in this game-like environment confronting two programs against each other in a virtual memory space, where they compete to overwrite each other's code to make the opponent crash.

Players must get familiar with the radare2 toolchain, assembly language and the rules of the game.

### Implementations

The intial implementation of the r2wars game was done in a Python r2pipe script. But this served as a proof-of-concept for another more performant implementation written in `C#` and available on this repository. This implementation is the one used in the official competitions at r2con.

* https://github.com/radareorg/r2wars

### Supported Architectures

R2wars supports various architectures, and it used as an excuse to improve the state of the assembler, disassemblers and emulation capabilities of radare2.

The most relevant are: x86, 8051, ARM32, ARM64, MIPS and  RISC-V.

Players can choose their preferred architecture or agree on a specific one for each battle, as specified by the competition rules if inter-arch bots are permitted.

### Writing Warriors

To create a warrior for r2wars, participants write assembly code tailored to their chosen architecture.

Note that the assembly code should be optimized for size and efficiency, as space in the virtual memory is limited. Other strategies are:

* Creating small, fast-moving code that's hard to target
* Exploiting the use of memory scanning and copying data
* Note that turns depend on the instruction cost not the instruction count
* Developing efficient scanning techniques to locate the opponent quickly
* Drawing images and text in the r2wars panel GUI for fun
* Use instructions with low cycle count costs

### Battle Mechanics

When a battle begins, both programs are loaded into the shared memory space. Execution alternates between the two programs, with each getting a turn to execute a single instruction. This continues until one program crashes or a predefined number of cycles is reached.

The r2wars interface provides real-time visualization of the memory space, allowing spectators to observe the battle as it unfolds. This includes tracking changes in memory, register values, and the current execution point of each program.

### Examples

These are some of the bots used in real r2wars competitions.

**jordi.x86-32.asm**

```assembly
call label
label:
	pop eax
loop:
	sub eax, 10
	cmp dword ptr[eax], 0
je loop
	mov dword ptr[eax], 0
	jmp loop
```

**pancake.mips-64.asm**

```assembly
bal 0 + getpc
getpc:
  move v0, ra
  lui v1, 1
loop:
  sw v0, 0(v1)
  addiu v1, v1, v0
  b loop
  nop
```

**ricardoapl.x86-32.asm**

```
mov edi, 0x0f60fc83
mov esi, 0x6060e04c
mov ebp, 0xffe4ff60

mov esp, 0x00000200

mov ebx, 0xffffffff
mov edx, 0xffffffff
mov ecx, 0xffffffff
mov eax, 0x00000400

pushal
jmp esp
```

**zeta.arm-32.asm**

```
_start:
    ldr r0, [pc, #48]
    ldr r1, [pc, #48]
    ldr r2, [pc, #48]
    ldr r3, [pc, #44]
    ldr r4, [pc, #40]
    ldr r5, [pc, #36]
    ldr r6, [pc, #36]

    movt r7, #0xffff
    movt r8, #0xffff
    movt r9, #0xffff
    movw r10, #256

    movw sp, #0x0400
    push {r0, r1, r2, r3, r4, r5, r6, r7, r8, sb, sl, fp, ip, sp, lr, pc}
    bx sp

_data:
    cmp sp, r10
    movlt sp, 0x400
    push {r0, r1, r2, r3, r4, r5, r6, r7, r8, sb, sl, fp, ip, sp, lr, pc}
    bx sp
```
