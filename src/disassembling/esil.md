# ESIL

ESIL stands for 'Evaluable Strings Intermediate Language'. It aims to describe a [Forth](https://en.wikipedia.org/wiki/Forth_%28programming_language%29)-like representation for every target CPU opcode semantics. ESIL representations can be evaluated (interpreted) in order to emulate individual instructions. Each command of an ESIL expression is separated by a comma. Its virtual machine can be described as this:
```
   while ((word=haveCommand())) {
     if (word.isOperator()) {
       esilOperators[word](esil);
     } else {
       esil.push (word);
     }
     nextCommand();
   }
```
As we can see ESIL uses a stack-based interpreter similar to what is commonly used for calculators. You have two categories of inputs: values and operators. A value simply gets pushed on the stack, an operator then pops values (its arguments if you will) off the stack, performs its operation and pushes its results (if any) back on. We can think of ESIL as a post-fix notation of the operations we want to do.

So let's see an example:
```
4,esp,-=,ebp,esp,=[4]
```
Can you guess what this is? If we take this post-fix notation and transform it back to in-fix we get
```
esp -= 4
4bytes(dword) [esp] = ebp
```
We can see that this corresponds to the x86 instruction `push ebp`! Isn't that cool?
The aim is to be able to express most of the common operations performed by CPUs, like binary arithmetic operations, memory loads and stores, processing syscalls. This way if we can transform the instructions to ESIL we can see what a program does while it is running even for the most cryptic architectures you definitely don't have a device to debug on for.

## Using ESIL

r2's visual mode is great to inspect the ESIL evaluations.

There are 2 environment variables that are important for watching what a program does:
```
[0x00000000]> e emu.str = true
```

`asm.emu` tells r2 if you want ESIL information to be displayed. If it is set to true, you will see comments appear to the right of your disassembly that tell you how the contents of registers and memory addresses are changed by the current instruction. For example, if you have an instruction that subtracts a value from a register it tells you what the value was before and what it becomes after. This is super useful so you don't have to sit there yourself and track which value goes where.

One problem with this is that it is a lot of information to take in at once and sometimes you simply don't need it. r2 has a nice compromise for this. That is what the `emu.str` variable is for (`asm.emustr` on <= 2.2). Instead of this super verbose output with every register value, this only adds really useful information to the output, e.g., strings that are found at addresses a program uses or whether a jump is likely to be taken or not.

The third important variable is `asm.esil`. This switches your disassembly to no longer show you the actual disassembled instructions, but instead now shows you corresponding ESIL expressions that describe what the instruction does.
So if you want to take a look at how instructions are expressed in ESIL simply set "asm.esil" to true.

```
[0x00000000]> e asm.esil = true
```

In visual mode you can also toggle this by simply typing `O`.

## ESIL Commands

* "ae" : Evaluate ESIL expression.

```
[0x00000000]> "ae 1,1,+"
0x2
[0x00000000]>
```

* "aes" : ESIL Step.

```
[0x00000000]> aes
[0x00000000]>10aes
```
* "aeso" : ESIL Step Over.

```
[0x00000000]> aeso
[0x00000000]>10aeso
```

* "aesu" : ESIL Step Until.

```
[0x00001000]> aesu 0x1035
ADDR BREAK
[0x00001019]>
```

* "ar" : Show/modify ESIL registry.

```
[0x00001ec7]> ar r_00 = 0x1035
[0x00001ec7]> ar r_00
0x00001035
[0x00001019]>
```

### ESIL Instruction Set

Here is the complete instruction set used by the ESIL VM:

ESIL Opcode | Operands | Name | Operation| example
--- | --- | --- | --- | ----------------------------------------------
TRAP  | src | Trap | Trap signal |
**$** | src | Syscall | syscall  |
**$$** | src | Instruction address | Get address of current instruction<br>stack=instruction address |
**==** | src,dst | Compare | stack = (dst == src) ; <br> update_eflags(dst - src) |
**<** | src,dst | Smaller (signed comparison) | stack = (dst < src) ; <br> update_eflags(dst - src) | [0x0000000]> "ae 1,5,<" <br>0x0<br>&gt; "ae 5,5"<br>0x0"
**<=** | src,dst | Smaller or Equal (signed comparison) | stack = (dst <= src) ; <br> update_eflags(dst - src) | [0x0000000]> "ae 1,5,<" <br>0x0<br>&gt; "ae 5,5"<br>0x1"
**>** | src,dst | Bigger (signed comparison) | stack = (dst > src) ; <br> update_eflags(dst - src) | &gt; "ae 1,5,>"<br>0x1<br>&gt; "ae 5,5,>"<br>0x0
 **>=** | src,dst | Bigger or Equal (signed comparison) | stack = (dst >= src) ; <br> update_eflags(dst - src) | &gt; "ae 1,5,>="<br>0x1<br>&gt; "ae 5,5,>="<br>0x1
 **<<** | src,dst | Shift Left | stack = dst << src | &gt; "ae 1,1,<<"<br>0x2<br>&gt; "ae 2,1,<<"<br>0x4
 **>>** | src,dst | Shift Right | stack = dst >> src | &gt; "ae 1,4,>>"<br>0x2<br>&gt; "ae 2,4,>>"<br>0x1
 **<<<** | src,dst | Rotate Left | stack=dst ROL src | &gt; "ae 31,1,<<<"<br>0x80000000<br>&gt; "ae 32,1,<<<"<br>0x1
**>>>** | src,dst | Rotate Right | stack=dst ROR src | &gt; "ae 1,1,>>>"<br>0x80000000<br>&gt; "ae 32,1,>>>"<br>0x1
**&** | src,dst | AND | stack = dst & src | &gt; "ae 1,1,&"<br>0x1<br>&gt; "ae 1,0,&"<br>0x0<br>&gt;  "ae 0,1,&"<br>0x0<br>&gt; "ae 0,0,&"<br>0x0
**&#x7c;** | src,dst | OR | stack = dst &#x7c; src | &gt; "ae 1,1,&#x7c;"<br>0x1<br>&gt; "ae 1,0,&#x7c;"<br>0x1<br>&gt; "ae 0,1,&#x7c;"<br>0x1<br>&gt; "ae 0,0,&#x7c;"<br>0x0
**^** | src,dst | XOR | stack = dst ^src  | &gt; "ae 1,1,^"<br>0x0<br>&gt; "ae 1,0,^"<br>0x1<br>&gt; "ae 0,1,^"<br>0x1<br>&gt; "ae 0,0,^"<br>0x0
**+** | src,dst | ADD | stack = dst + src | &gt; "ae 3,4,+"<br>0x7<br>&gt; "ae 5,5,+"<br>0xa
**-** | src,dst | SUB | stack = dst - src | &gt; "ae 3,4,-"<br>0x1<br>&gt; "ae 5,5,-"<br>0x0<br>&gt; "ae 4,3,-"<br>0xffffffffffffffff
**\*** | src,dst | MUL | stack = dst * src | &gt; "ae 3,4,\*"<br>0xc<br>&gt; "ae 5,5,\*"<br>0x19
**/** | src,dst | DIV | stack = dst / src  | &gt; "ae 2,4,/"<br>0x2<br>&gt; "ae 5,5,/"<br>0x1<br>&gt; "ae 5,9,/"<br>0x1
**%** | src,dst | MOD | stack = dst % src | &gt; "ae 2,4,%"<br>0x0<br>&gt; "ae 5,5,%"<br>0x0<br>&gt; "ae 5,9,%"<br>0x4
**!** | src | NEG | stack = !!!src | &gt; "ae 1,!"<br>0x0<br>&gt; "ae 4,!"<br>0x0<br>&gt; "ae 0,!"<br>0x1<br>
**++** | src | INC | stack = src++ | &gt; ar r_00=0;ar r_00<br>0x00000000<br>&gt; "ae r_00,++"<br>0x1<br>&gt; ar r_00<br>0x00000000<br>&gt; "ae 1,++"<br>0x2
**--** | src | DEC | stack = src-- | &gt; ar r_00=5;ar r_00<br>0x00000005<br>&gt; "ae r_00,--"<br>0x4<br>&gt; ar r_00<br>0x00000005<br>&gt; "ae 5,--"<br>0x4
**=** | src,reg | EQU | reg = src | &gt; "ae 3,r_00,="<br>&gt; aer r_00<br>0x00000003<br>&gt; "ae r_00,r_01,="<br>&gt; aer r_01<br>0x00000003
**+=** | src,reg | ADD eq | reg = reg + src | &gt; ar r_01=5;ar r_00=0;ar r_00<br>0x00000000<br>&gt; "ae r_01,r_00,+="<br>&gt; ar r_00<br>0x00000005<br>&gt; "ae 5,r_00,+="<br>&gt; ar r_00<br>0x0000000a
**-=** | src,reg | SUB eq | reg = reg - src | &gt; "ae r_01,r_00,-="<br>&gt; ar r_00<br>0x00000004<br>&gt; "ae 3,r_00,-="<br>&gt; ar r_00<br>0x00000001
**\*=** | src,reg | MUL eq | reg = reg * src | &gt; ar r_01=3;ar r_00=5;ar r_00<br>0x00000005<br>&gt; "ae r_01,r_00,\*="<br>&gt; ar r_00<br>0x0000000f<br>&gt; "ae 2,r_00,\*="<br>&gt; ar r_00<br>0x0000001e
 **/=** | src,reg | DIV eq | reg = reg / src | &gt; ar r_01=3;ar r_00=6;ar r_00<br>0x00000006<br>&gt; "ae r_01,r_00,/="<br>&gt; ar r_00<br>0x00000002<br>&gt; "ae 1,r_00,/="<br>&gt; ar r_00<br>0x00000002
 **%=** | src,reg | MOD eq | reg = reg % src | &gt;  ar r_01=3;ar r_00=7;ar r_00<br> 0x00000007<br> &gt; "ae r_01,r_00,%="<br> &gt; ar r_00<br> 0x00000001<br> &gt;  ar r_00=9;ar r_00<br> 0x00000009<br> &gt; "ae 5,r_00,%="<br> &gt; ar r_00<br> 0x00000004
**<<=** | src,reg | Shift Left eq | reg = reg << src | &gt; ar r_00=1;ar r_01=1;ar r_01<br>0x00000001<br>&gt; "ae r_00,r_01,<<="<br>&gt; ar r_01<br>0x00000002<br>&gt; "ae 2,r_01,<<="<br>&gt; ar r_01<br>0x00000008
**>>=** | src,reg | Shift Right eq | reg = reg << src | &gt; ar r_00=1;ar r_01=8;ar r_01<br>0x00000008<br>&gt; "ae r_00,r_01,>>="<br>&gt; ar r_01<br>0x00000004<br>&gt; "ae 2,r_01,>>="<br>&gt; ar r_01<br>0x00000001
**&=** | src,reg |  AND eq | reg = reg & src | &gt; ar r_00=2;ar r_01=6;ar r_01<br>0x00000006<br>&gt; "ae r_00,r_01,&="<br>&gt; ar r_01<br>0x00000002<br>&gt; "ae 2,r_01,&="<br>&gt; ar r_01<br>0x00000002<br>&gt; "ae 1,r_01,&="<br>&gt; ar r_01<br>0x00000000
**&#x7c;=** | src,reg | OR eq| reg = reg &#x7c; src | &gt; ar r_00=2;ar r_01=1;ar r_01<br>0x00000001<br>&gt; "ae r_00,r_01,&#x7c;="<br>&gt; ar r_01<br>0x00000003<br>&gt; "ae 4,r_01,&#x7c;="<br>&gt; ar r_01<br>0x00000007
 **^=** | src,reg | XOR eq | reg = reg ^ src | &gt; ar r_00=2;ar r_01=0xab;ar r_01<br>0x000000ab<br>&gt; "ae r_00,r_01,^="<br>&gt; ar r_01<br>0x000000a9<br>&gt; "ae 2,r_01,^="<br>&gt; ar r_01<br>0x000000ab
**++=** | reg | INC eq | reg = reg + 1 | &gt; ar r_00=4;ar r_00<br>0x00000004<br>&gt; "ae r_00,++="<br>&gt; ar r_00<br>0x00000005
**--=** | reg | DEC eq | reg = reg - 1 | &gt; ar r_00=4;ar r_00<br>0x00000004<br>&gt; "ae r_00,--="<br>&gt; ar r_00<br>0x00000003
**!=** | reg | NOT eq | reg = !reg | &gt; ar r_00=4;ar r_00<br>0x00000004<br>&gt; "ae r_00,!="<br>&gt; ar r_00<br>0x00000000<br>&gt; "ae r_00,!="<br>&gt; ar r_00<br>0x00000001
--- | --- | --- | --- | ----------------------------------------------
=[]<br>=[\*]<br>=[1]<br>=[2]<br>=[4]<br>=[8] | src,dst | poke |\*dst=src | <br>&gt; "ae 0xdeadbeef,0x10000,=[4],"<br><br>&gt; pxw 4@0x10000<br>0x00010000  0xdeadbeef                                ....<br><br>&gt; "ae 0x0,0x10000,=[4],"<br><br>&gt; pxw 4@0x10000<br>0x00010000  0x00000000
[]<br>[\*]<br>[1]<br>[2]<br>[4]<br>[8] | src | peek | stack=\*src | <br>&gt; w test@0x10000<br><br>&gt; "ae 0x10000,[4],"<br>0x74736574<br><br>&gt; ar r_00=0x10000<br><br>&gt; "ae r_00,[4],"<br>0x74736574
&#x7c;=[]<br>&#x7c;=[1]<br>&#x7c;=[2]<br>&#x7c;=[4]<br>&#x7c;=[8] | reg | nombre | code | &gt; <br>&gt;
SWAP |  | Swap | Swap two top elements | SWAP
PICK | n | Pick | Pick nth element<br> from the top of the stack | 2,PICK
RPICK | m | Reverse Pick | Pick nth element<br> from the base of the stack | 0,RPICK
DUP | | Duplicate | Duplicate top element in stack | DUP
NUM | | Numeric | If top element is a reference <br> (register name, label, etc),<br> dereference it and push its real value | NUM
CLEAR | | Clear | Clear stack | CLEAR
BREAK | | Break | Stops ESIL emulation | BREAK
GOTO | n | Goto | Jumps to Nth ESIL word | GOTO 5
TODO | | To Do | Stops execution<br> (reason: ESIL expression not completed) | TODO

### ESIL Flags

ESIL VM has an internal state flags that are read-only and can be used to export those values to the underlying target CPU flags. It is because the ESIL VM always calculates all flag changes, while target CPUs only update flags under certain conditions or at specific instructions.

Internal flags are prefixed with `$` character.

```
z      - zero flag, only set if the result of an operation is 0
b      - borrow, this requires to specify from which bit (example: $b4 - checks if borrow from bit 4)
c      - carry, same like above (example: $c7 - checks if carry from bit 7)
o      - overflow
p      - parity
r      - regsize ( asm.bits/8 )
s      - sign
ds     - delay slot state
jt     - jump target
js     - jump target set
[0-9]* - Used to set flags and registers without having any side effects,
         i.e. setting esil_cur, esil_old and esil_lastsz.
         (example: "$0,of,=" to reset the overflow flag)
```

## Syntax and Commands
A target opcode is translated into a comma separated list of ESIL expressions.
```
xor eax, eax    ->    0,eax,=,1,zf,=
```
Memory access is defined by brackets operation:
```
mov eax, [0x80480]   ->   0x80480,[],eax,=
```
Default operand size is determined by size of operation destination.
```
movb $0, 0x80480     ->   0,0x80480,=[1]
```

The `?` operator uses the value of its argument to decide whether to evaluate the expression in curly braces.

1. Is the value zero?      -> Skip it.
2. Is the value non-zero?  -> Evaluate it.

```
cmp eax, 123  ->   123,eax,==,$z,zf,=
jz eax        ->   zf,?{,eax,eip,=,}
```


If you want to run several expressions under a conditional, put them in curly braces:
```
zf,?{,eip,esp,=[],eax,eip,=,$r,esp,-=,}
```

Whitespaces, newlines and other chars are ignored. So the first thing when processing a ESIL program is to remove spaces:
```
esil = r_str_replace (esil, " ", "", R_TRUE);
```

Syscalls need special treatment. They are indicated by '$' at the beginning of an expression. You can pass an optional numeric value to specify a number of syscall. An ESIL emulator must handle syscalls. See (r_esil_syscall).

## Arguments Order for Non-associative Operations

As discussed on IRC, the current implementation works like this:

```
a,b,-      b - a
a,b,/=     b /= a
```
This approach is more readable, but it is less stack-friendly.

### Special Instructions

NOPs are represented as empty strings. As it was said previously, syscalls are marked by '$' command. For example, '0x80,$'. It delegates emulation from the ESIL machine to a callback which implements syscalls for a specific OS/kernel.

Traps are implemented with the `TRAP` command. They are used to throw exceptions for invalid instructions, division by zero, memory read error, or any other needed by specific architectures.

### Quick Analysis

Here is a list of some quick checks to retrieve information from an ESIL string. Relevant information will be probably found in the first expression of the list.
```
indexOf('[')    -> have memory references
indexOf("=[")   -> write in memory
indexOf("pc,=") -> modifies program counter (branch, jump, call)
indexOf("sp,=") -> modifies the stack (what if we found sp+= or sp-=?)
indexOf("=")    -> retrieve src and dst
indexOf(":")    -> unknown esil, raw opcode ahead
indexOf("$")    -> accesses internal esil vm flags ex: $z
indexOf("$")    -> syscall ex: 1,$
indexOf("TRAP") -> can trap
indexOf('++')   -> has iterator
indexOf('--')   -> count to zero
indexOf("?{")   -> conditional
equalsTo("")    -> empty string, aka nop (wrong, if we append pc+=x)
```

Common operations:
 * Check dstreg
 * Check srcreg
 * Get destinaion
 * Is jump
 * Is conditional
 * Evaluate
 * Is syscall

### CPU Flags

CPU flags are usually defined as single bit registers in the RReg profile. They and sometimes found under the 'flg' register type.

### Variables

Properties of the VM variables:

1. They have no predefined bit width. This way it should be easy to extend them to 128, 256 and 512 bits later, e.g. for MMX, SSE, AVX, Neon SIMD.

2. There can be unbound number of variables. It is done for SSA-form compatibility.

3. Register names have no specific syntax. They are just strings.

4. Numbers can be specified in any base supported by RNum (dec, hex, oct, binary ...).

5. Each ESIL backend should have an associated RReg profile to describe the ESIL register specs.

### Bit Arrays

What to do with them? What about bit arithmetics if use variables instead of registers?

### Arithmetics

1. ADD ("+")
2. MUL ("\*")
3. SUB ("-")
4. DIV ("/")
5. MOD ("%")


### Bit Arithmetics

1. AND  "&"
2. OR   "|"
3. XOR  "^"
4. SHL  "<<"
5. SHR  ">>"
6. ROL  "<<<"
7. ROR  ">>>"
8. NEG  "!"

### Floating Point Unit Support

At the moment of this writing, ESIL does not yet support FPU. But you can implement support for unsupported instructions using r2pipe. Eventually we will get proper support for multimedia and floating point.

### Handling x86 REP Prefix in ESIL

ESIL specifies that the parsing control-flow commands must be uppercase. Bear in mind that some architectures have uppercase register names. The corresponding register profile should take care not to reuse any of the following:
```
3,SKIP   - skip N instructions. used to make relative forward GOTOs
3,GOTO   - goto instruction 3
LOOP     - alias for 0,GOTO
BREAK    - stop evaluating the expression
STACK    - dump stack contents to screen
CLEAR    - clear stack
```

#### Usage Example:

rep cmpsb
```
cx,!,?{,BREAK,},esi,[1],edi,[1],==,?{,BREAK,},esi,++,edi,++,cx,--,0,GOTO
```

### Unimplemented/Unhandled Instructions

Those are expressed with the 'TODO' command. They act as a 'BREAK', but displays a warning message describing that an instruction is not implemented and will not be emulated. For example:

```
fmulp ST(1), ST(0)      =>      TODO,fmulp ST(1),ST(0)
```

### ESIL Disassembly Example:

```
[0x1000010f8]> e asm.esil=true
[0x1000010f8]> pd $r @ entry0
0x1000010f8    55           8,rsp,-=,rbp,rsp,=[8]
0x1000010f9    4889e5       rsp,rbp,=
0x1000010fc    4883c768     104,rdi,+=
0x100001100    4883c668     104,rsi,+=
0x100001104    5d           rsp,[8],rbp,=,8,rsp,+=
0x100001105    e950350000   0x465a,rip,= ;[1]
0x10000110a    55           8,rsp,-=,rbp,rsp,=[8]
0x10000110b    4889e5       rsp,rbp,=
0x10000110e    488d4668     rsi,104,+,rax,=
0x100001112    488d7768     rdi,104,+,rsi,=
0x100001116    4889c7       rax,rdi,=
0x100001119    5d           rsp,[8],rbp,=,8,rsp,+=
0x10000111a    e93b350000   0x465a,rip,= ;[1]
0x10000111f    55           8,rsp,-=,rbp,rsp,=[8]
0x100001120    4889e5       rsp,rbp,=
0x100001123    488b4f60     rdi,96,+,[8],rcx,=
0x100001127    4c8b4130     rcx,48,+,[8],r8,=
0x10000112b    488b5660     rsi,96,+,[8],rdx,=
0x10000112f    b801000000   1,eax,=
0x100001134    4c394230     rdx,48,+,[8],r8,==,cz,?=
0x100001138    7f1a         sf,of,!,^,zf,!,&,?{,0x1154,rip,=,} ;[2]
0x10000113a    7d07         of,!,sf,^,?{,0x1143,rip,} ;[3]
0x10000113c    b8ffffffff   0xffffffff,eax,= ;  0xffffffff
0x100001141    eb11         0x1154,rip,= ;[2]
0x100001143    488b4938     rcx,56,+,[8],rcx,=
0x100001147    48394a38     rdx,56,+,[8],rcx,==,cz,?=
```

### Introspection

To ease ESIL parsing we should have a way to express introspection expressions to extract the data that we want. For example, we may want to get the target address of a jump. The parser for ESIL expressions should offer an API to make it possible to extract information by analyzing the expressions easily.

```
>  ao~esil,opcode
opcode: jmp 0x10000465a
esil: 0x10000465a,rip,=
```
We need a way to retrieve the numeric value of 'rip'. This is a very simple example, but there are more complex, like conditional ones. We need expressions to be able to get:

- opcode type
- destination of a jump
- condition depends on
- all regs modified (write)
- all regs accessed (read)

### API HOOKS

It is important for emulation to be able to setup hooks in the parser, so we can extend it to implement analysis without having to change it again and again. That is, every time an operation is about to be executed, a user hook is called. It can be used for example to determine if `RIP` is going to change, or if the instruction updates the stack.
Later, we can split that callback into several ones to have an event-based analysis API that may be extended in JavaScript like this:

```
esil.on('regset', function(){..
esil.on('syscall', function(){esil.regset('rip'
```

For the API, see the functions `hook_flag_read()`, `hook_execute()` and `hook_mem_read()`. A callback should return true or 1 if you want to override the action that it takes. For example, to deny memory reads in a region, or voiding memory writes, effectively making it read-only.
Return false or 0 if you want to trace ESIL expression parsing.

Other operations require bindings to external functionalities to work. In this case, `r_ref` and `r_io`. This must be defined when initializing the ESIL VM.

* Io Get/Set
  ```
  Out ax, 44
  44,ax,:ou
  ```
* Selectors (cs,ds,gs...)
  ```
  Mov eax, ds:[ebp+8]
  Ebp,8,+,:ds,eax,=
  ```
