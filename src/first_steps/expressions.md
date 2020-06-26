## Expressions

Expressions are mathematical representations of 64-bit numerical values.
They can be displayed in different formats, be compared or used with all commands
accepting numeric arguments. Expressions can use traditional arithmetic operations,
as well as binary and boolean ones.
To evaluate mathematical expressions prepend them with command `?`:
```
[0xb7f9d810]> ?vi 0x8048000
134512640
[0xv7f9d810]> ?vi 0x8048000+34
134512674
[0xb7f9d810]> ?vi 0x8048000+0x34
134512692
[0xb7f9d810]> ? 1+2+3-4*3
hex     0xfffffffffffffffa
octal   01777777777777777777772
unit    17179869184.0G
segment fffff000:0ffa
int64   -6
string  "\xfa\xff\xff\xff\xff\xff\xff\xff"
binary  0b1111111111111111111111111111111111111111111111111111111111111010
fvalue: -6.0
float:  nanf
double: nan
trits   0t11112220022122120101211020120210210211201
```
Supported arithmetic operations are:

 *  \+ : addition
 *  \- : subtraction
 *  \* : multiplication
 *  / : division
 *  % : modulus
 *  \> : shift right
 *  < : shift left

```
[0x00000000]> ?vi 1+2+3
6
```

To use of logical OR should quote the whole command to avoid executing the `|` pipe:
```
[0x00000000]> "? 1 | 2"
hex     0x3
octal   03
unit    3
segment 0000:0003
int32   3
string  "\x03"
binary  0b00000011
fvalue: 2.0
float:  0.000000f
double: 0.000000
trits   0t10
```

Numbers can be displayed in several formats:
```
0x033   : hexadecimal can be displayed
3334    : decimal
sym.fo  : resolve flag offset
10K     : KBytes  10*1024
10M     : MBytes  10*1024*1024
```

You can also use variables and seek positions to build complex expressions.

Use the `?$?` command to list all the available commands or read the refcard chapter of this book.

```
$$    here (the current virtual seek)
$l    opcode length
$s    file size
$j    jump address (e.g. jmp 0x10, jz 0x10 => 0x10)
$f    jump fail address (e.g. jz 0x10 => next instruction)
$m    opcode memory reference (e.g. mov eax,[0x10] => 0x10)
$b    block size
```

Some more examples:
```
[0x4A13B8C0]> ? $m + $l
140293837812900 0x7f98b45df4a4 03771426427372244 130658.0G 8b45d000:04a4 140293837812900 10100100 140293837812900.0 -0.000000
```
```
[0x4A13B8C0]> pd 1 @ +$l
0x4A13B8C2   call 0x4a13c000
```
