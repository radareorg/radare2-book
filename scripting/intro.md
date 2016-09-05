Radare2 provides a wide set of a features to automate boring work.
It ranges from the simple sequencing of the commands to the calling
scripts/another programs via IPC (Inter-Process Communication), called r2pipe.

As mentioned a few times before there is an ability to sequence commands
using ';' semicolon operator.

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

It simply runs second command after finishing the first one, like in shell.

The second important way to sequence the commands is simple pipe '|'

ao|grep address

Note, the '|' pipe only can pipe output of r2 commands to external (shell)
commands, like system programs or builtin shell commands.
And for providing the similar way to sequence r2 commands, there is
backtick operator '`' as well, working exactly the same way it works in shell.

For example we want to see a few bytes of the memory at the address, referred
by 'mov eax, addr' instruction for further call. We can do that without jumping to it,
we just need to sequence a couple of commands:

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

And of course it's possible to redirect the output of r2 command into file, using '>' and '>>'
commands

[0x00404800]> px 10 @ `ao~ptr[1]` > example.txt
[0x00404800]> px 10 @ `ao~ptr[1]` >> example.txt




