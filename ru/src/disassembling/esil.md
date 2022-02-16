# ESIL

ESIL расшифровывается как "Evaluable Strings Intermediate Language". Технология использует описание на языке, подобном [Forth](https://en.wikipedia.org/wiki/Forth_%28programming_language%29), семантики оп-кодов операций целевого процессора. Представления ESIL интерпретируются, имитируя отдельные инструкции. Команды выражения ESIL разделяются запятыми. Его виртуальная машина описывается так:
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
Как мы видим, ESIL использует интерпретатор на основе стека, аналогичный тому, который обычно используется для калькуляторов. Есть две категории входных данных: значения и операторы. Значение просто помещается в стек, затем оператор извлекает значения (если хотите, его аргументы) из стека, выполняет операцию и помещает результаты (если они есть) обратно в стек. ESIL использует постфиксную нотацию операций, которые мы хотим выполнить.

Посмотрим пример:
```
4,esp,-=,ebp,esp,=[4]
```
Догадываетесь, что это такое? Если возьмем запись и преобразуем ее обратно в инфиксную, получим
```
esp -= 4
4bytes(dword) [esp] = ebp
```
Теперь видно, что это выражение соответствует инструкции x86. `push ebp`! Круто, да?
Цель проекта состоит в том выражить большинство операций, выполняемых процессорами: двоичные арифметические операции, загрузка и сохранение ячеек памяти, обработка системных вызовов. Преобразовав инструкции в ESIL, будет увидно, что делает программа во время исполнения. Технология позволяет моделировать самые загадочные архитектуры, или, если у вас нет устройства для отладки.

## Использование ESIL

Визуальный режим r2 отлично подходит для использования ESIL.

Есть три переменные среды r2, управляющие формой представления процесса исполнения программы:
```
[0x00000000]> e emu.str = true
```

`asm.emu` настраивает r2, таким образом, чтобы отображалась информация ESIL. Если установлено значение true, справа от вашего дизассемблированного кода добавятся комментарии, показывающие изменения содержимого регистров и адресов памяти в результате исполнения соответсвующей инструкции. Например, для инструкции, вычитающей значение из регистра, ESIL сообщит, каким было значение до и каким оно становится после исполнения инструкции. Теперь не нужно отслеживать, какое значение куда идет.

Правда при этом приходится воспринимать сразу много информации, что иногда просто не нужно. В r2 есть хороший компромисс. Для этого есть `emu.str` (`asm.emustr` для версии r2 <= 2.2). Вместо сверхподробного вывода с каждым значением регистра, настройка добавляет к коду только действительно полезную информацию, например, строки, найденные по адресам, или вероятность того, что переход будет выполнен или нет.

Третья важная переменная - `asm.esil`. Она переключает дизассемблирование таким образом, что оно больше не показывает фактически дизассемблированные инструкции, а вместо этого печатает соответствующие выражения ESIL, описывающие, что делает инструкция.
Если хотите посмотреть, как инструкции выражаются в ESIL, просто установите значение переменной «asm.esil» в true.

```
[0x00000000]> e asm.esil = true
```

В визуальном режиме также можно переключить эти варианты представления кода, просто нажав `О` .

## ESIL-команды

* "ae" : Вычислить выражение ESIL.

```
[0x00000000]> "ae 1,1,+"
0x2
[0x00000000]>
```

* "aes" : Шаг ESIL.

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

* "ar" : Показать/задать регистр ESIL.

```
[0x00001ec7]> ar r_00 = 0x1035
[0x00001ec7]> ar r_00
0x00001035
[0x00001019]>
```

### Набор инструкций ESIL

Вот полный набор инструкций, используемый виртуальной машиной ESIL:

| Код операции ESIL | Операнды | Название | Операция | пример |
--- | --- | --- | --- | ----------------------------------------------
| TRAP | src | Trap | Trap сигнал |
| **$** | src | Interrupt | interrupt | 0x80,$ |
| **()** | src | Syscall | syscall | rax,() |
| **$$** | src | Адрес инструкции | Получить адрес текущей инструкции <br>стек=адрес инструкции |
| **==** | src,dst | Compare | stack = (dst == src) ; <br> update_eflags(dst - src) |
| **<** | src,dst | Smaller (сравнение с учетом знака) | stack = (dst < src) ; <br> update_eflags(dst - src) | [0x0000000]> "ae 1,5,<" <br>0x0<br>&gt; "ae 5,5"<br>0x0" |
| **<=** | src,dst | Smaller or Equal (сравнение с учетом знака) | stack = (dst <= src) ; <br> update_eflags(dst - src) | [0x0000000]> "ae 1,5,<" <br>0x0<br>&gt; "ae 5,5"<br>0x1" |
| **>** | src,dst | Bigger (сравнение с учетом знака) | stack = (dst > src) ; <br> update_eflags(dst - src) | &gt; "ae 1,5,>"<br>0x1<br>&gt; "ae 5,5,>"<br>0x0 |
| **>=** | src,dst | Bigger or Equal (сравнение с учетом знака) | stack = (dst >= src) ; <br> update_eflags(dst - src) | &gt; "ae 1,5,>="<br>0x1<br>&gt; "ae 5,5,>="<br>0x1 |
| **<<** | src,dst | Shift Left | stack = dst << src | &gt; "ae 1,1,<<"<br>0x2<br>&gt; "ae 2,1,<<"<br>0x4 |
| **>>** | src,dst | Shift Right | stack = dst >> src | &gt; "ae 1,4,>>"<br>0x2<br>&gt; "ae 2,4,>>"<br>0x1 |
| **<<<** | src,dst | Rotate Left | stack=dst ROL src | &gt; "ae 31,1,<<<"<br>0x80000000<br>&gt; "ae 32,1,<<<"<br>0x1 |
| **>>>** | src,dst | Rotate Right | stack=dst ROR src | &gt; "ae 1,1,>>>"<br>0x80000000<br>&gt; "ae 32,1,>>>"<br>0x1 |
| **&** | src,dst | AND | stack = dst & src | &gt; "ae 1,1,&"<br>0x1<br>&gt; "ae 1,0,&"<br>0x0<br>&gt;  "ae 0,1,&"<br>0x0<br>&gt; "ae 0,0,&"<br>0x0 |
| **&#x7c;** | src,dst | OR | stack = dst &#x7c; src | &gt; "ae 1,1,&#x7c;"<br>0x1<br>&gt; "ae 1,0,&#x7c;"<br>0x1<br>&gt; "ae 0,1,&#x7c;"<br>0x1<br>&gt; "ae 0,0,&#x7c;"<br>0x0 |
| **^** | src,dst | XOR | stack = dst ^src | &gt; "ae 1,1,^"<br>0x0<br>&gt; "ae 1,0,^"<br>0x1<br>&gt; "ae 0,1,^"<br>0x1<br>&gt; "ae 0,0,^"<br>0x0 |
| **+** | src,dst | ADD | stack = dst + src | &gt; "ae 3,4,+"<br>0x7<br>&gt; "ae 5,5,+"<br>0xa |
| **-** | src,dst | SUB | stack = dst - src | &gt; "ae 3,4,-"<br>0x1<br>&gt; "ae 5,5,-"<br>0x0<br>&gt; "ae 4,3,-"<br>0xffffffffffffffff |
| **\*** | src,dst | MUL | stack = dst * src | &gt; "ae 3,4,\*"<br>0xc<br>&gt; "ae 5,5,\*"<br>0x19 |
| **/** | src,dst | DIV | stack = dst / src | &gt; "ae 2,4,/"<br>0x2<br>&gt; "ae 5,5,/"<br>0x1<br>&gt; "ae 5,9,/"<br>0x1 |
| **%** | src,dst | MOD | stack = dst % src | &gt; "ae 2,4,%"<br>0x0<br>&gt; "ae 5,5,%"<br>0x0<br>&gt; "ae 5,9,%"<br>0x4 |
| **~** | bits,src | SIGNEXT | stack = src sign extended | &gt; "ae 8,0x80,~"<br>0xffffffffffffff80 |
| **~/** | src,dst | SIGNED DIV | stack = dst / src (signed) | &gt; "ae 2,-4,~/"<br>0xfffffffffffffffe |
| **~%** | src,dst | SIGNED MOD | stack = dst % src (signed) | &gt; "ae 2,-5,~%"<br>0xffffffffffffffff |
| **!** | src | NEG | stack = !!!src | &gt; "ae 1,!"<br>0x0<br>&gt; "ae 4,!"<br>0x0<br>&gt; "ae 0,!"<br>0x1<br> |
| **++** | src | INC | stack = src++ | &gt; ar r_00=0;ar r_00<br>0x00000000<br>&gt; "ae r_00,++"<br>0x1<br>&gt; ar r_00<br>0x00000000<br>&gt; "ae 1,++"<br>0x2 |
| **--** | src | DEC | stack = src-- | &gt; ar r_00=5;ar r_00<br>0x00000005<br>&gt; "ae r_00,--"<br>0x4<br>&gt; ar r_00<br>0x00000005<br>&gt; "ae 5,--"<br>0x4 |
| **=** | src,reg | EQU | reg = src | &gt; "ae 3,r_00,="<br>&gt; aer r_00<br>0x00000003<br>&gt; "ae r_00,r_01,="<br>&gt; aer r_01<br>0x00000003 |
| **:=** | src,reg | weak EQU | reg = src without side effects | &gt; "ae 3,r_00,:="<br>&gt; aer r_00<br>0x00000003<br>&gt; "ae r_00,r_01,:="<br>&gt; aer r_01<br>0x00000003 |
| **+=** | src,reg | ADD eq | reg = reg + src | &gt; ar r_01=5;ar r_00=0;ar r_00<br>0x00000000<br>&gt; "ae r_01,r_00,+="<br>&gt; ar r_00<br>0x00000005<br>&gt; "ae 5,r_00,+="<br>&gt; ar r_00<br>0x0000000a |
| **-=** | src,reg | SUB eq | reg = reg - src | &gt; "ae r_01,r_00,-="<br>&gt; ar r_00<br>0x00000004<br>&gt; "ae 3,r_00,-="<br>&gt; ar r_00<br>0x00000001 |
| **\*=** | src,reg | MUL eq | reg = reg * src | &gt; ar r_01=3;ar r_00=5;ar r_00<br>0x00000005<br>&gt; "ae r_01,r_00,\*="<br>&gt; ar r_00<br>0x0000000f<br>&gt; "ae 2,r_00,\*="<br>&gt; ar r_00<br>0x0000001e |
| **/=** | src,reg | DIV eq | reg = reg / src | &gt; ar r_01=3;ar r_00=6;ar r_00<br>0x00000006<br>&gt; "ae r_01,r_00,/="<br>&gt; ar r_00<br>0x00000002<br>&gt; "ae 1,r_00,/="<br>&gt; ar r_00<br>0x00000002 |
| **%=** | src,reg | MOD eq | reg = reg % src | &gt;  ar r_01=3;ar r_00=7;ar r_00<br> 0x00000007<br> &gt; "ae r_01,r_00,%="<br> &gt; ar r_00<br> 0x00000001<br> &gt;  ar r_00=9;ar r_00<br> 0x00000009<br> &gt; "ae 5,r_00,%="<br> &gt; ar r_00<br> 0x00000004 |
| **<<=** | src,reg | Shift Left eq | reg = reg << src | &gt; ar r_00=1;ar r_01=1;ar r_01<br>0x00000001<br>&gt; "ae r_00,r_01,<<="<br>&gt; ar r_01<br>0x00000002<br>&gt; "ae 2,r_01,<<="<br>&gt; ar r_01<br>0x00000008 |
| **>>=** | src,reg | Shift Right eq | reg = reg << src | &gt; ar r_00=1;ar r_01=8;ar r_01<br>0x00000008<br>&gt; "ae r_00,r_01,>>="<br>&gt; ar r_01<br>0x00000004<br>&gt; "ae 2,r_01,>>="<br>&gt; ar r_01<br>0x00000001 |
| **&=** | src,reg | AND eq | reg = reg & src | &gt; ar r_00=2;ar r_01=6;ar r_01<br>0x00000006<br>&gt; "ae r_00,r_01,&="<br>&gt; ar r_01<br>0x00000002<br>&gt; "ae 2,r_01,&="<br>&gt; ar r_01<br>0x00000002<br>&gt; "ae 1,r_01,&="<br>&gt; ar r_01<br>0x00000000 |
| **&#x7c;=** | src,reg | OR eq | reg = reg &#x7c; src | &gt; ar r_00=2;ar r_01=1;ar r_01<br>0x00000001<br>&gt; "ae r_00,r_01,&#x7c;="<br>&gt; ar r_01<br>0x00000003<br>&gt; "ae 4,r_01,&#x7c;="<br>&gt; ar r_01<br>0x00000007 |
| **^=** | src,reg | XOR eq | reg = reg ^ src | &gt; ar r_00=2;ar r_01=0xab;ar r_01<br>0x000000ab<br>&gt; "ae r_00,r_01,^="<br>&gt; ar r_01<br>0x000000a9<br>&gt; "ae 2,r_01,^="<br>&gt; ar r_01<br>0x000000ab |
| **++=** | reg | INC eq | reg = reg + 1 | &gt; ar r_00=4;ar r_00<br>0x00000004<br>&gt; "ae r_00,++="<br>&gt; ar r_00<br>0x00000005 |
| **--=** | reg | DEC eq | reg = reg - 1 | &gt; ar r_00=4;ar r_00<br>0x00000004<br>&gt; "ae r_00,--="<br>&gt; ar r_00<br>0x00000003 |
| **!=** | reg | NOT eq | reg = !reg | &gt; ar r_00=4;ar r_00<br>0x00000004<br>&gt; "ae r_00,!="<br>&gt; ar r_00<br>0x00000000<br>&gt; "ae r_00,!="<br>&gt; ar r_00<br>0x00000001 |
| --- | --- | --- | --- | ---------------------------------------------- |
| =[]<br>=[\*]<br>=[1]<br>=[2]<br>=[4]<br>=[8] | src,dst | poke | \*dst=src | <br>&gt; "ae 0xdeadbeef,0x10000,=[4],"<br><br>&gt; pxw 4@0x10000<br>0x00010000  0xdeadbeef                                ....<br><br>&gt; "ae 0x0,0x10000,=[4],"<br><br>&gt; pxw 4@0x10000<br>0x00010000  0x00000000 |
| []<br>[\*]<br>[1]<br>[2]<br>[4]<br>[8] | src | peek | stack=\*src | <br>&gt; w test@0x10000<br><br>&gt; "ae 0x10000,[4],"<br>0x74736574<br><br>&gt; ar r_00=0x10000<br><br>&gt; "ae r_00,[4],"<br>0x74736574 |
| &#x7c;=[]<br>&#x7c;=[1]<br>&#x7c;=[2]<br>&#x7c;=[4]<br>&#x7c;=[8] | reg | nombre | code | &gt; <br>&gt; |
| SWAP |  | Swap | Поменять местами два верхних элемента | SWAP |
| DUP | | Duplicate | Дублировать верхний элемент в стеке | DUP |
| NUM | | Numeric | Если верхний элемент является ссылкой <br> (регистровое имя, метка и т.д.),<br> разыменовать его и положить на стек его реальное значение | NUM |
| CLEAR | | Clear | Очистить стек | CLEAR |
| BREAK | | Break | Останавливает эмуляцию ESIL | BREAK |
| GOTO | n | Goto | Переход к N-му слову ESIL | GOTO 5 |
| TODO | | To Do | Останавливает выполнение <br> (причина: выражение ESIL не завершено) | TODO |

### Флаги ESIL

ESIL VM по умолчанию предоставляет набор вспомогательных операций для вычисления флагов.
Они выполняют свою задачу, сравнивая старое и новое значение операнда dst последней выполненной операции eq.
На каждой операции eq (например, `==`) ESIL сохраняет старое и новое значение операнда dst.
Обратите внимание, что существуют также слабые операции eq (например,`:=`), которые не влияют на операции с флагами.
Операция `==` влияет на операции с флагами, несмотря на то, что она не является операцией eq.
Операции с флагами задаются префиксным символом `$`.

```
z      - zero flag, only set if the result of an operation is 0
b      - borrow, this requires to specify from which bit (example: 4,$b - checks if borrow from bit 4)
c      - carry, same like above (example: 7,$c - checks if carry from bit 7)
o      - overflow
p      - parity
r      - regsize ( asm.bits/8 )
s      - sign
ds     - delay slot state
jt     - jump target
js     - jump target set
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

NOPs are represented as empty strings. As it was said previously, interrupts are marked by '$' command. For example, '0x80,$'. It delegates emulation from the ESIL machine to a callback which implements interrupt handler for a specific OS/kernel/platform.

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

CPU flags are usually defined as single bit registers in the RReg profile. They are sometimes found under the 'flg' register type.

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

Those are expressed with the 'TODO' command. They act as a 'BREAK', but displays a warning message describing that an instruction is not implemented and will not be emulated. Например:

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
