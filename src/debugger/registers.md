# Registers

The registers are part of a user area stored in the context structure used by the scheduler. This structure can be manipulated to get and set the values of those registers, and, for example, on Intel hosts, it is possible to directly manipulate DR0-DR7 hardware registers to set hardware breakpoints.

There are different commands to get values of registers. For the General Purpose ones use:

```
[0x4A13B8C0]> dr
r15 = 0x00000000
r14 = 0x00000000
r13 = 0x00000000
r12 = 0x00000000
rbp = 0x00000000
rbx = 0x00000000
r11 = 0x00000000
r10 = 0x00000000
r9 = 0x00000000
r8 = 0x00000000
rax = 0x00000000
rcx = 0x00000000
rdx = 0x00000000
rsi = 0x00000000
rdi = 0x00000000
oeax = 0x0000003b
rip = 0x7f20bf5df630
rsp = 0x7fff515923c0

[0x7f0f2dbae630]> dr rip ; get value of 'rip'
0x7f0f2dbae630

[0x4A13B8C0]> dr rip = esp   ; set 'rip' as esp
```

Interaction between a plugin and the core is done by commands returning radare instructions. This is used, for example, to set flags in the core to set  values of registers.

```
[0x7f0f2dbae630]> dr*      ; Appending '*' will show radare commands
f r15 1 0x0
f r14 1 0x0
f r13 1 0x0
f r12 1 0x0
f rbp 1 0x0
f rbx 1 0x0
f r11 1 0x0
f r10 1 0x0
f r9 1 0x0
f r8 1 0x0
f rax 1 0x0
f rcx 1 0x0
f rdx 1 0x0
f rsi 1 0x0
f rdi 1 0x0
f oeax 1 0x3b
f rip 1 0x7fff73557940
f rflags 1 0x200
f rsp 1 0x7fff73557940

[0x4A13B8C0]> .dr*  ; include common register values in flags
```

An old copy of registers is stored all the time to keep track of the changes done during execution of a program being analyzed. This old copy can be accessed with `oregs`.

```
[0x7f1fab84c630]> dro
r15 = 0x00000000
r14 = 0x00000000
r13 = 0x00000000
r12 = 0x00000000
rbp = 0x00000000
rbx = 0x00000000
r11 = 0x00000000
r10 = 0x00000000
r9 = 0x00000000
r8 = 0x00000000
rax = 0x00000000
rcx = 0x00000000
rdx = 0x00000000
rsi = 0x00000000
rdi = 0x00000000
oeax = 0x0000003b
rip = 0x7f1fab84c630
rflags = 0x00000200
rsp = 0x7fff386b5080
```
Current state of registers

```
[0x7f1fab84c630]> dr
r15 = 0x00000000
r14 = 0x00000000
r13 = 0x00000000
r12 = 0x00000000
rbp = 0x00000000
rbx = 0x00000000
r11 = 0x00000000
r10 = 0x00000000
r9 = 0x00000000
r8 = 0x00000000
rax = 0x00000000
rcx = 0x00000000
rdx = 0x00000000
rsi = 0x00000000
rdi = 0x7fff386b5080
oeax = 0xffffffffffffffff
rip = 0x7f1fab84c633
rflags = 0x00000202
rsp = 0x7fff386b5080
```

Values stored in eax, oeax and eip have changed.

To store and restore register values you can just dump the output of 'dr*' command to disk and then re-interpret it again:

```
[0x4A13B8C0]> dr* > regs.saved ; save registers
[0x4A13B8C0]> drp regs.saved ; restore
```

EFLAGS can be similarly altered. E.g., setting selected flags:

```
[0x4A13B8C0]> dr eflags = pst
[0x4A13B8C0]> dr eflags = azsti
```

You can get a string which represents latest changes of registers using `drd` command (diff registers):

```
[0x4A13B8C0]> drd
oeax = 0x0000003b was 0x00000000 delta 59
rip = 0x7f00e71282d0 was 0x00000000 delta -418217264
rflags = 0x00000200 was 0x00000000 delta 512
rsp = 0x7fffe85a09c0 was 0x00000000 delta -396752448
```

## Register Profiles

The way register values are transferred from kernel to userland (or via network when using gdb or other debuggers) it's usually done through a linear memory buffer and an associated register profile which is in charge to describe the name, location, size and other attributes to interpret that buffer.

Usually not all registers are transferred in the same buffer, this is because there are register groups or families, like floating pointer, general purpose, privileged ones..

In the case of GDB, XML format is choosen to describe all this information, in radare2 we use our own space/tab separated document which can be dumped or changed at any time with the `drp` and `arp` commands (note one is for debugging sessions, and the other will be used for the static esil emulation).

Radare2 is able to parse the gdb xml register profile and generate one in the radare2 syntax when connecting to unknown targets that support those commands.

### Reading The Profile

Let's check how the x86-16 (real mode) register profile looks like by typing the following command:

```
$ r2 -a x86 -b 16 -qc arp --
=PC	ip
=SP	sp
=BP	bp
=R0	ax
=A0	ax
=A1	bx
=A2	cx
=A3	dx
=A4	si
=A5	di
=SN	ah
=TR	fs
gpr	ip	.16	48	0
gpr	ax	.16	24	0
gpr	ah	.8	25	0
gpr	al	.8	24	0
gpr	bx	.16	0	0
gpr	bh	.8	1	0
gpr	bl	.8	0	0
gpr	cx	.16	4	0
gpr	ch	.8	5	0
gpr	cl	.8	4	0
gpr	dx	.16	8	0
gpr	dh	.8	9	0
gpr	dl	.8	8	0
gpr	sp	.16	60	0
gpr	bp	.16	20	0
gpr	si	.16	12	0
gpr	di	.16	16	0
seg	cs	.16	52	0
seg	ss	.16	54	0
seg	ds	.16	56	0
seg	es	.16	58	0
gpr	flags	.16	56	0
flg	cf	.1	.448	0
flg	pf	.1	.449	0
flg	af	.1	.450	0
flg	zf	.1	.451	0
flg	sf	.1	.452	0
flg	tf	.1	.453	0
flg	if	.1	.454	0
flg	df	.1	.455	0
flg	of	.1	.456	0
flg	rf	.1	.457	0
```

### Understanding Each Row

The format for this file is quite easy, but somehow tricky as it's not self-documented and there's no scheme like in XML or JSON. That's probably something that can be improved at some point, but for simplicity reasons it was made like this.

### Register Naming and Aliases

Note that in radare2, all registers are lowercase words that can't contain dots or spaces. This is important because that's how they are identified as flags in the disassembly as well as in the reg profile and makes it more readable for the users when inspecting the disassembly listings.

Some flavours use the percentage `%` sign before the name to indicate its nature. But that's not our style. So let's move on.

If the row starts with a equality `=` sign it's indicating that the line will be used to define an alias. Register aliases must be two uppercase letters. This is for parsing performance reasons, but also helps quickly identifying them when using those in the shell.

`=PC ip`

This line means that everytime we try to pick the "PC" register, it will be redirected to point to the one named `ip`. This way radare2 is able to work across multiple architectures having a generic way to refer to each of them.

### Alias Register

The register aliases are used for a variety of actions in radare2, so the code analysis, calling conventions, syscall scanning and so on will be affected by those.

* PC : Program Counter
* SP : Stack Pointer
* BP : Base Pointer (delta for the stack frame with SP)
* R0 : First Register used to return values
* R1 : Second Register (used for tuples or 64bit values on 32bit systems)
* A0 : First argument passed to a syscall
* A1 : Second argument..
* A2 : Third argument..
* A3 : Fourth argument..
* SN : Register used Syscall Number
* TR : Thread Local Storage Register

### Register Groups

As mentioned earlier the registers can be grouped and classified depending on the uses for proper displaying them as well as the debugger backend pulling them back from the source.

These are the group names, as you will notice, all of them have 3 lowercase letters:

* gpr : General Purpose Registers
* flg : Status Flags
* seg : Segment Registers
* fpu : Floating Pointer Registers
* vec : Vector Registers
* pri : Privileged Registers

### Column Meanings

The following lines will look like this:

```
..
gpr    r2      .32     8       0
gpr    r3      .32     12      0
...
```

Anyway, let's focus on the meaning of each column:

* gpr: Register Group
* r2, r3: Register Names
* .32 : Register Size
* 8, 12 : Offset inside the buffer
* 0 : Packing Size

Few notes here:

* These columns are separated by tabs or spaces
* Lines starting with `#` are ignored
* Register names must start with a lowercase letter and have no dots
* Sizes or offsets starting with a dot `.` are represented in bits instead of bytes

The Packing size is used to define the syllab size of each register word for vector registers.

### Endianness

Values can be stored and represented in different endianness when working in local or remote instances with the debugger.

The way this information is represented in the reg profile is with a line starting with `^`. The next letter will tell the register profile the endianness to use which can be **b**ig, **l**ittle or **m**iddle.

You can find an usage example for this feature in the register profile for the native debugger for s390x architecture.
