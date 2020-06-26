# Loops

One of the most common task in automation is looping through something,
there are multiple ways to do this in radare2.

We can loop over flags:
```
@@ flagname-regex
```

For example, we want to see function information with `afi` command:
```
[0x004047d6]> afi
#
offset: 0x004047d0
name: entry0
size: 42
realsz: 42
stackframe: 0
call-convention: amd64
cyclomatic-complexity: 1
bits: 64
type: fcn [NEW]
num-bbs: 1
edges: 0
end-bbs: 1
call-refs: 0x00402450 C
data-refs: 0x004136c0 0x00413660 0x004027e0
code-xrefs:
data-xrefs:
locals:0
args: 0
diff: type: new
[0x004047d6]>
```
Now let's say, for example, that we'd like see a particular field from this output for all functions found by analysis. We can do that with a loop over all function flags (whose names begin with `fcn.`):
```
[0x004047d6]> afi @@ fcn.* ~name
```
This command will extract the `name` field from the `afi` output of every flag with a name
matching the regexp `fcn.*`.

We can also loop over a list of offsets, using the following syntax:
```
@@=1 2 3 ... N
```
For example, say we want to see the opcode information for 2 offsets: the current one, and at current + 2:

```
[0x004047d6]> ao @@=$$ $$+2
address: 0x4047d6
opcode: mov rdx, rsp
prefix: 0
bytes: 4889e2
refptr: 0
size: 3
type: mov
esil: rsp,rdx,=
stack: null
family: cpu
address: 0x4047d8
opcode: loop 0x404822
prefix: 0
bytes: e248
refptr: 0
size: 2
type: cjmp
esil: 1,rcx,-=,rcx,?{,4212770,rip,=,}
jump: 0x00404822
fail: 0x004047da
stack: null
cond: al
family: cpu
[0x004047d6]>
```
Note we're using the `$$` variable which evaluates to the current offset. Also note
that `$$+2` is evaluated before looping, so we can use the simple arithmetic expressions.

A third way to loop is by having the offsets be loaded from a file. This file should contain
one offset per line.
```
[0x004047d0]> ?v $$ > offsets.txt
[0x004047d0]> ?v $$+2 >> offsets.txt
[0x004047d0]> !cat offsets.txt
4047d0
4047d2
[0x004047d0]> pi 1 @@.offsets.txt
xor ebp, ebp
mov r9, rdx
```

radare2 also offers various `foreach` constructs for looping. One of the most useful is for looping through all the instructions of a function:
```
[0x004047d0]> pdf
╒ (fcn) entry0 42
│; UNKNOWN XREF from 0x00400018 (unk)
│; DATA XREF from 0x004064bf (sub.strlen_460)
│; DATA XREF from 0x00406511 (sub.strlen_460)
│; DATA XREF from 0x0040b080 (unk)
│; DATA XREF from 0x0040b0ef (unk)
│0x004047d0  xor ebp, ebp
│0x004047d2  mov r9, rdx
│0x004047d5  pop rsi
│0x004047d6  mov rdx, rsp
│0x004047d9  and rsp, 0xfffffffffffffff0
│0x004047dd  push rax
│0x004047de  push rsp
│0x004047df  mov r8, 0x4136c0
│0x004047e6  mov rcx, 0x413660      ; "AWA..AVI..AUI..ATL.%.. "
0A..AVI..AUI.
│0x004047ed  mov rdi, main          ; "AWAVAUATUH..S..H...." @
0
│0x004047f4  call sym.imp.__libc_start_main
╘0x004047f9  hlt
[0x004047d0]> pi 1 @@i
mov r9, rdx
pop rsi
mov rdx, rsp
and rsp, 0xfffffffffffffff0
push rax
push rsp
mov r8, 0x4136c0
mov rcx, 0x413660
mov rdi, main
call sym.imp.__libc_start_main
hlt
```
In this example the command `pi 1` runs over all the instructions in the current function (entry0).
There are other options too (not complete list, check `@@?` for more information):
 - `@@k sdbquery` - iterate over all offsets returned by that sdbquery
 - `@@t`- iterate over on all threads (see dp)
 - `@@b` - iterate over all basic blocks of current function (see afb)
 - `@@f` - iterate over all functions (see aflq)

The last kind of looping lets you loop through predefined iterator types:

 - symbols
 - imports
 - registers
 - threads
 - comments
 - functions
 - flags

This is done using the `@@@` command. The previous example of listing information about functions can also be done using the `@@@` command:

```
[0x004047d6]> afi @@@ functions ~name
```
This will extract `name` field from `afi` output and will output a huge list of
function names. We can choose only the second column, to remove the redundant `name:` on every line:
```
[0x004047d6]> afi @@@ functions ~name[1]
```

**Beware, @@@ is not compatible with JSON commands.**

