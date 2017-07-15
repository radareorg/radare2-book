Radare2 provides a wide set of a features to automate boring work.
It ranges from the simple sequencing of the commands to the calling
scripts/another programs via IPC (Inter-Process Communication), called r2pipe.

As mentioned a few times before there is an ability to sequence commands
using `;` semicolon operator.
```
[0x00404800]> pd 1 ; ao 1
           0x00404800      b827e66100     mov eax, 0x61e627      ; "tab"
address: 0x404800
opcode: mov eax, 0x61e627
prefix: 0
bytes: b827e66100
ptr: 0x0061e627
refptr: 0
size: 5
type: mov
esil: 6415911,rax,=
stack: null
family: cpu
[0x00404800]>
```
It simply runs the second command after finishing the first one, like in a shell.

The second important way to sequence the commands is with a simple pipe `|`
```
ao|grep address
```
Note, the `|` pipe only can pipe output of r2 commands to external (shell)
commands, like system programs or builtin shell commands.
There is a similar way to sequence r2 commands, using the backtick operator `` `command` ``. The quoted part will undergo command substitution and the output will be used as an argument of the command line.

For example, we want to see a few bytes of the memory at the address referred to
by the 'mov eax, addr' instruction. We can do that without jumping to it, using
a sequence of commands:
```
[0x00404800]> pd 1
              0x00404800      b827e66100     mov eax, 0x61e627      ; "tab"
[0x00404800]> ao
address: 0x404800
opcode: mov eax, 0x61e627
prefix: 0
bytes: b827e66100
ptr: 0x0061e627
refptr: 0
size: 5
type: mov
esil: 6415911,rax,=
stack: null
family: cpu
[0x00404800]> ao~ptr[1]
0x0061e627
0
[0x00404800]> px 10 @ `ao~ptr[1]`
- offset -   0 1  2 3  4 5  6 7  8 9  A B  C D  E F  0123456789ABCDEF
0x0061e627  7461 6200 2e69 6e74 6572                 tab..inter
[0x00404800]>
```
And of course it's possible to redirect the output of an r2 command into a file, using the `>` and `>>`
commands
```
[0x00404800]> px 10 @ `ao~ptr[1]` > example.txt
[0x00404800]> px 10 @ `ao~ptr[1]` >> example.txt
```

The `?$?` command describes several helpful variables you can use to do similar actions even more
easily, like the `$v` "immediate value" variable, or the `$m` opcode memory reference variable.

