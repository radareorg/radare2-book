#ESIL

*From the wiki page of radare2 at Github*

ESIL stands for 'Evaluable Strings Intermediate Language'. It aims to describe a Forth-like representation for every target CPU opcode semantics. ESIL representations can be evaluated (interpreted) in order to emulate individual instructions. Each command of an ESIL expression is separated by a comma. Its virtual machine can be described as this:
```
   while ((word=haveCommand())) {
     if (word.isKeyword()) {
       esilCommands[word](esil);
     } else {
       esil.push (evaluateToNumber(word));
     }
     nextCommand();
   }
```
ESIL commands are operations that pop values from the stack, perform calculations and push result (if any) to the stack. The aim is to be able to express most of common operations performed by CPUs, like binary arithmetic operations, memory loads and stores, processing syscalls etc.

##Use ESIL
```
[0x00000000]> e asm.esil = true
```
##ESIL Commands 
   * "ae" : Evaluate ESIL expresion.
   
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

###ESIL Instruction Set

Here is the complete instruction set used by the ESIL VM:

ESIL Opcode | Operands | Name | Operation| example
--- | --- | --- | --- | ----------------------------------------------
**$$** | src | Trap |  trap  |   
**$** | src | Syscall | sysccall  |   
**==** | src,dst | Compare | v = dst - src ; update_eflags(v) |   
**<** | src,dst | Smaller | stack = (dst < src) | [0x0000000]> "ae 1,5,<" <br>0x0<br>[0x00000000]> "ae 5,5"<br>0x0"
**<=** | src,dst | Smaller or Equal | stack = (dst <= src) | [0x0000000]> "ae 1,5,<" <br>0x0<br>[0x00000000]> "ae 5,5"<br>0x1"   
**>** | src,dst | Bigger | stack = (dst > src) | [0x00000000]> "ae 1,5,>"<br>0x1<br>[0x00000000]> "ae 5,5,>"<br>0x0
 **>=** | src,dst | Bigger or Equal | stack = (dst > src) | [0x00000000]> "ae 1,5,>="<br>0x1<br>[0x00000000]> "ae 5,5,>="<br>0x1
 **<<** | src,dst | Shift Left | stack = dst << src | [0x00000000]> "ae 1,1,<<"<br>0x2<br>[0x00000000]> "ae 2,1,<<"<br>0x4
 **>>** | src,dst | Shift Right | stack = dst >> src | [0x00000000]> "ae 1,4,>>"<br>0x2<br>[0x00000000]> "ae 2,4,>>"<br>0x1
 **<<<<** | src,dst | Rotate Left | stack=dst ROL src | [0x00000000]> "ae 31,1,<<<<"<br>0x80000000<br>[0x00000000]> "ae 32,1,<<<<"<br>0x1
**>>>>** | src,dst | Rotate Right | stack=dst ROR src | [0x00000000]> "ae 1,1,>>>>"<br>0x80000000<br>[0x00000000]> "ae 32,1,>>>>"<br>0x1
**&** | src,dst | AND | stack = dst & src | [0x00000000]> "ae 1,1,&"<br>0x1<br>[0x00000000]> "ae 1,0,&"<br>0x0<br>[0x00000000]>  "ae 0,1,&"<br>0x0<br>[0x00000000]> "ae 0,0,&"<br>0x0
**`|`** | src,dst | OR | stack = dst `|` src | [0x00000000]> "ae 1,1,`|`"<br>0x1<br>[0x00000000]> "ae 1,0,`|`"<br>0x1<br>[0x00000000]> "ae 0,1,`|`"<br>0x1<br>[0x00000000]> "ae 0,0,`|`"<br>0x0
**^** | src,dst | XOR | stack = dst ^src  | [0x00000000]> "ae 1,1,^"<br>0x0<br>[0x00000000]> "ae 1,0,^"<br>0x1<br>[0x00000000]> "ae 0,1,^"<br>0x1<br>[0x00000000]> "ae 0,0,^"<br>0x0
**+** | src,dst | ADD | stack = dst + src | [0x00000000]> "ae 3,4,+"<br>0x7<br>[0x00000000]> "ae 5,5,+"<br>0xa
**-** | src,dst | SUB | stack = dst - src | [0x00000000]> "ae 3,4,-"<br>0x1<br>[0x00000000]> "ae 5,5,-"<br>0x0<br>[0x00000000]> "ae 4,3,-"<br>0xffffffffffffffff
**`*`** | src,dst | MUL | stack = dst * src | [0x00000000]> "ae 3,4,`*`"<br>0xc<br>[0x00000000]> "ae 5,5,`*`"<br>0x19
**/** | src,dst | DIV | stack = dst / src  | [0x00000000]> "ae 2,4,/"<br>0x2<br>[0x00000000]> "ae 5,5,/"<br>0x1<br>[0x00000000]> "ae 5,9,/"<br>0x1
**%** | src,dst | MOD | stack = dst % src | [0x00000000]> "ae 2,4,%"<br>0x0<br>[0x00000000]> "ae 5,5,%"<br>0x0<br>[0x00000000]> "ae 5,9,%"<br>0x4
**!** | src | NEG | stack = !!!src | [0x00000000]> "ae 1,!"<br>0x0<br>[0x00000000]> "ae 4,!"<br>0x0<br>[0x00000000]> "ae 0,!"<br>0x1<br>
**++** | src | INC | stack = src++ | [0x00000000]> ar r_00=0;ar r_00<br>0x00000000<br>[0x00000000]> "ae r_00,++"<br>0x1<br>[0x00000000]> ar r_00<br>0x00000000<br>[0x00000000]> "ae 1,++"<br>0x2
**--** | src | DEC | stack = src-- | [0x00000000]> ar r_00=5;ar r_00<br>0x00000005<br>[0x00000000]> "ae r_00,--"<br>0x4<br>[0x00000000]> ar r_00<br>0x00000005<br>[0x00000000]> "ae 5,--"<br>0x4
**+=** | src,reg | ADD eq | reg = reg + src | [0x00000000]> ar r_01=5;ar r_00=0;ar r_00<br>0x00000000<br>[0x00000000]> "ae r_01,r_00,+="<br>[0x00000000]> ar r_00<br>0x00000005<br>[0x00000000]> "ae 5,r_00,+="<br>[0x00000000]> ar r_00<br>0x0000000a
**-=** | src,reg | SUB eq | reg = reg - src | [0x00000000]> "ae r_01,r_00,-="<br>[0x00000000]> ar r_00<br>0x00000004<br>[0x00000000]> "ae 3,r_00,-="<br>[0x00000000]> ar r_00<br>0x00000001
**`*=`** | src,reg | MUL eq | reg = reg * src | [0x00000000]> ar r_01=3;ar r_00=5;ar r_00<br>0x00000005<br>[0x00000000]> "ae r_01,r_00,`*`="<br>[0x00000000]> ar r_00<br>0x0000000f<br>[0x00000000]> "ae 2,r_00,`*`="<br>[0x00000000]> ar r_00<br>0x0000001e
 **/=** | src,reg | DIV eq | reg = reg / src | [0x00000000]> ar r_01=3;ar r_00=6;ar r_00<br>0x00000006<br>[0x00000000]> "ae r_01,r_00,/="<br>[0x00000000]> ar r_00<br>0x00000002<br>[0x00000000]> "ae 1,r_00,/="<br>[0x00000000]> ar r_00<br>0x00000002
 **%=** | src,reg | MOD eq | reg = reg % src | [0x00000000]>  ar r_01=3;ar r_00=7;ar r_00<br> 0x00000007<br> [0x00000000]> "ae r_01,r_00,%="<br> [0x00000000]> ar r_00<br> 0x00000001<br> [0x00000000]>  ar r_00=9;ar r_00<br> 0x00000009<br> [0x00000000]> "ae 5,r_00,%="<br> [0x00000000]> ar r_00<br> 0x00000004
**<<=** | src,reg | Shift Left eq | reg = reg << src | [0x00000000]> ar r_00=1;ar r_01=1;ar r_01<br>0x00000001<br>[0x00000000]> "ae r_00,r_01,<<="<br>[0x00000000]> ar r_01<br>0x00000002<br>[0x00000000]> "ae 2,r_01,<<="<br>[0x00000000]> ar r_01<br>0x00000008
**>>=** | src,reg | Shift Right eq | reg = reg << src | [0x00000000]> ar r_00=1;ar r_01=8;ar r_01<br>0x00000008<br>[0x00000000]> "ae r_00,r_01,>>="<br>[0x00000000]> ar r_01<br>0x00000004<br>[0x00000000]> "ae 2,r_01,>>="<br>[0x00000000]> ar r_01<br>0x00000001
**&=** | src,reg |  AND eq | reg = reg & src | [0x00000000]> ar r_00=2;ar r_01=6;ar r_01<br>0x00000006<br>[0x00000000]> "ae r_00,r_01,&="<br>[0x00000000]> ar r_01<br>0x00000002<br>[0x00000000]> "ae 2,r_01,&="<br>[0x00000000]> ar r_01<br>0x00000002<br>[0x00000000]> "ae 1,r_01,&="<br>[0x00000000]> ar r_01<br>0x00000000
**`|`=** | src,reg | OR eq| reg = reg `|` src | [0x00000000]> ar r_00=2;ar r_01=1;ar r_01<br>0x00000001<br>[0x00000000]> "ae r_00,r_01,|="<br>[0x00000000]> ar r_01<br>0x00000003<br>[0x00000000]> "ae 4,r_01,|="<br>[0x00000000]> ar r_01<br>0x00000007
 **^=** | src,reg | XOR eq | reg = reg ^ src | [0x00000000]> ar r_00=2;ar r_01=0xab;ar r_01<br>0x000000ab<br>[0x00000000]> "ae r_00,r_01,^="<br>[0x00000000]> ar r_01<br>0x000000a9<br>[0x00000000]> "ae 2,r_01,^="<br>[0x00000000]> ar r_01<br>0x000000ab
**++=** | reg | INC eq | reg = reg + 1 | [0x00000000]> ar r_00=4;ar r_00<br>0x00000004<br>[0x00000000]> "ae r_00,++="<br>[0x00000000]> ar r_00<br>0x00000005
**--=** | reg | DEC eq | reg = reg - 1 | [0x00000000]> ar r_00=4;ar r_00<br>0x00000004<br>[0x00000000]> "ae r_00,--="<br>[0x00000000]> ar r_00<br>0x00000003
**!=** | reg | NOT eq | reg = !reg | [0x00000000]> ar r_00=4;ar r_00<br>0x00000004<br>[0x00000000]> "ae r_00,!="<br>[0x00000000]> ar r_00<br>0x00000000<br>[0x00000000]> "ae r_00,!="<br>[0x00000000]> ar r_00<br>0x00000001
--- | --- | --- | --- | ----------------------------------------------
`|`=[]<br>`|`=[1]<br>`|`=[2]<br>`|`=[4]<br>`|`=[8] | reg | nombre | reg = !reg | [0x00000000]> <br>[0x00000000]>


###ESIL Flags

ESIL VM has an internal state flags that are read only and can be used to export those values to the underlying target CPU flags. It is because the ESIL VM always calculates all flag changes, while target CPUs only update flags under certain conditions or at specific instructions.

Internal flags are prefixed with '%' character.

```
z - zero flag, only set if the result of an operation is 0
b - borrow, this requires to specify from which bit (example: %b4 - checks if borrow from bit 4)
c - carry, same like above (example: %c7 - checks if carry from bit 7)
p - parity
r - regsize ( asm.bits/8 )
```

##Syntax and Commands
======
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
The `?` command checks whether the rest of the expression after it evaluates to zero or not. If it is zero, the following expression is skipped, otherwise it is evaluated. `%` prefix indicates internal variables.
```
cmp eax, 123  ->   123,eax,==,%z,zf,=
jz eax        ->   zf,?{,eax,eip,=,}
```

If you want to run several expressions under a conditional, put them in curly braces:
```
zf,?{,eip,esp,=[],eax,eip,=,%r,esp,-=,}
```

Whitespaces, newlines and other chars are ignored. So the first thing when processing a ESIL program is to remove spaces:
```
esil = r_str_replace (esil, " ", "", R_TRUE);
```

Syscalls need special treatment. They are indicated by '$' at the beginning of an expression. You can pass an optional numeric value to specify a number of syscall. An ESIL emulator must handle syscalls. See (r_esil_syscall).

## Arguments Order for Non-associative Operations

As discussed on IRC, current implementation works like this:

```
a,b,-      b - a
a,b,/=     b /= a
```
This approach is more readable, but it is less stack-friendly.

###Special Instructions

NOPs are represented as empty strings. As it was said previously, syscalls are marked by '$' command. For example, '0x80,$'. It delegates emulation from the ESIL machine to a callback which implements syscalls for a specific OS/kernel.

Traps are implemented with the `<trap>,<code>,$$` command. They are used to throw exceptions for invalid instructions, division by zero, memory read error, etc.

###Quick Analysis

Here is a list of some quick checks to retrieve information from an ESIL string. Relevant information will be probably found in the first expression of the list.
```
indexOf('[')       ->    have memory references
indexOf("=[")      ->    write in memory
indexOf("pc,=")    ->    modifies program counter (branch, jump, call)
indexOf("sp,=")    ->    modifies the stack (what if we found sp+= or sp-=?)
indexOf("=")       ->    retrieve src and dst
indexOf(":")       ->    unknown esil, raw opcode ahead
indexOf("%")       ->    accesses internal esil vm flags
indexOf("$")       ->    syscall
indexOf("$$")      ->    can trap
indexOf('++')      ->    has iterator
indexOf('--')      ->    count to zero
indexOf("?{")      ->    conditional
indexOf("LOOP")    ->    is a loop (rep?)
equalsTo("")       ->    empty string, means: nop (wrong, if we append pc+=x)
```

Common operations:
 * Check dstreg
 * Check srcreg
 * Get destinaion
 * Is jump
 * Is conditional
 * Evaluate
 * Is syscall

###CPU Flags

CPU flags are usually defined as single bit registers in the RReg profile. They and sometimes found under the 'flg' register type.

###Variables

Properties of the VM variables:
1. They have no predefined bit width. This way it should be easy to extend them to 128, 256 and 512 bits later, e.g. for MMX, SSE, AVX, Neon SIMD.
2. There can be unbound number of variables. It is done for SSA-form compatibility.
3. Register names have no specific syntax. They are just strings.
4. Numbers can be specified in any base supported by RNum (dec, hex, oct, binary ...)
5. Each ESIL backend should have an associated RReg profile to describe the ESIL register specs.

###Bit Arrays

What to do with them? What about bit arithmetics if use variables instead of registers?

###Arithmetics

1. ADD ("+")
2. MUL ("*")
3. SUB ("-")
4. DIV ("/")
5. MOD ("%")


###Bit Arithmetics

1. AND  "&"
2. OR   "|"
3. XOR  "^"
4. SHL  "<<"
5. SHR  ">>"
6. ROL  "<<<"
7. ROR  ">>>"
8. NEG  "!"

###Floating Point Support


_TODO_

###Handling x86 REP Prefix in ESIL

ESIL specifies that the parsing control-flow commands must be uppercase. Bear in mind that some architectures have uppercase register names. The corresponding register profile should take care not to reuse any of the following:
```
3,SKIP   - skip N instructions. used to make relative forward GOTOs
3,GOTO   - goto instruction 3
LOOP     - alias for 0,GOTO
BREAK    - stop evaluating the expression
STACK    - dump stack contents to screen
CLEAR    - clear stack
```

####Usage example:

rep cmpsb
---------
cx,!,?{,BREAK,},esi,[1],edi,[1],==,?{,BREAK,},esi,++,edi,++,cx,--,LOOP


###Unimplemented/unhandled Instructions

Those are expressed with the 'TODO' command. which acts as a 'BREAK', but displays a warning message describing that an instruction is not implemented and will not be emulated. For example:

```
fmulp ST(1), ST(0)      =>      TODO,fmulp ST(1),ST(0)
```

###ESIL Disassembly Example:

```
[0x1000010f8]> e asm.esil=true
[0x1000010f8]> pd $r @ entry0
   ;      [0] va=0x1000010f8 pa=0x000010f8 sz=13299 vsz=13299 rwx=-r-x 0.__text
            ;-- section.0.__text:
            0x1000010f8    55           8,rsp,-=,rbp,rsp,=[8]
            0x1000010f9    4889e5       rsp,rbp,=
            0x1000010fc    4883c768     104,rdi,+=
            0x100001100    4883c668     104,rsi,+=
            0x100001104    5d           rsp,[8],rbp,=,8,rsp,+=                                          ┌─< 0x100001105    e950350000   0x465a,rip,= ;[1]
        │   0x10000110a    55           8,rsp,-=,rbp,rsp,=[8]
        │   0x10000110b    4889e5       rsp,rbp,=                                                       │   0x10000110e    488d4668     rsi,104,+,rax,=
        │   0x100001112    488d7768     rdi,104,+,rsi,=
        │   0x100001116    4889c7       rax,rdi,=
        │   0x100001119    5d           rsp,[8],rbp,=,8,rsp,+=                                         ┌──< 0x10000111a    e93b350000   0x465a,rip,= ;[1]
       ││   0x10000111f    55           8,rsp,-=,rbp,rsp,=[8]
       ││   0x100001120    4889e5       rsp,rbp,=
       ││   0x100001123    488b4f60     rdi,96,+,[8],rcx,=
       ││   0x100001127    4c8b4130     rcx,48,+,[8],r8,=                                              ││   0x10000112b    488b5660     rsi,96,+,[8],rdx,=
       ││   0x10000112f    b801000000   1,eax,= ;  0x00000001
       ││   0x100001134    4c394230     rdx,48,+,[8],r8,==,cz,?=
      ┌───< 0x100001138    7f1a         sf,of,!,^,zf,!,&,?{,0x1154,rip,=,} ;[2]
     ┌────< 0x10000113a    7d07         of,!,sf,^,?{,0x1143,rip,} ;[3]
     ││││   0x10000113c    b8ffffffff   0xffffffff,eax,= ;  0xffffffff                              ┌─────< 0x100001141    eb11         0x1154,rip,= ;[2]
    │└────> 0x100001143    488b4938     rcx,56,+,[8],rcx,=
    │ │││   0x100001147    48394a38     rdx,56,+,[8],rcx,==,cz,?=
```

###Introspection

To ease ESIL parsing we should have a way to express introspection expressions to extract data we want. For example, we may want to get the target address of a jump. The parser for ESIL expressions should offer API to make it possible to extract information by analyzing the expressions easily.

```
>  ao~esil,opcode
opcode: jmp 0x10000465a
esil: 0x10000465a,rip,=
```
We need a way to retrieve the numeric value of 'rip'. This is a very simple example, but there are more complex, like conditional ones. We need expressions to be able to get:

- opcode type
- destination of jump
- condition depends on
- all regs modified (write)
- all regs accessed (read)

###API HOOKS

It is important for emulation to be able to setup hooks in parser, so we can extend it to implement analysis without having to change parser again and again. That is, every time an operation is about to be executed, a user hook is called. It can be used to determine if rip is going to change, or if the instruction updates stack, etc.
Later, we can split that callback into several ones to have an event-based analysis API that may be extended in js like this:
esil.on('regset', function(){..
esil.on('syscall', function(){esil.regset('rip'

For the API, see functions hook_flag_read(), hook_execute(), hook_mem_read(). A callback should return true if you want to override the action taken for a callback. For example, to deny memory reads in a region, or voiding memory writes, effectively making it read-only.
Return false or 0 if you want to trace ESIL expression parsing.

Other operations that require bindings to external functionalities to work. In this case, r_ref and r_io. This must be defined when initializing the esil vm.

* Io Get/Set
    Out ax, 44
    44,ax,:ou
* Selectors (cs,ds,gs...)
   Mov eax, ds:[ebp+8]
   Ebp,8,+,:ds,eax,=
   
