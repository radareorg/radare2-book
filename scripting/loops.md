One of the most common task in automatization is looping through something.
There are multiple ways to loop over something. First, most known one - loop
over flags:
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
But we need to see some field of it for all functions found by analysis, for example.
We can do that with loop:
```
[0x004047d6]> afi @@ fcn.* ~name
```
This command will extract `name` field from `afi` output of every flag with name
matching regexp `fcn.*`.

The second way to use loops - is to set array of the offsets, using syntax
```
@@=1 2 3 ... N
```
For example we want to see opcode information for 2 offsets: current one, and current + 2:

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
Note, we're using `$$` variable which evaluates in the current offset. Also note,
that `$$+2` evaluates before looping, so we can use the simple arithmetic expressions.

Also, those offsets can be loaded from some file. This file should be formatted
with one offset per line.
```
[0x004047d0]> ?X $$ > offsets.txt
[0x004047d0]> ?X $$+2 >> offsets.txt
[0x004047d0]> !cat offsets.txt
4047d0
4047d2
[0x004047d0]> pi @@.offsets.txt
xor ebp, ebp
mov r9, rdx
```

There are different `foreach` types. One of the most useful - looping through instructions
of the selected function:
```
[0x004047d0]> pdf
╒ (fcn) entry0 42
│                ; UNKNOWN XREF from 0x00400018 (unk)
│                ; DATA XREF from 0x004064bf (sub.strlen_460)
│                ; DATA XREF from 0x00406511 (sub.strlen_460)
│                ; DATA XREF from 0x0040b080 (unk)
│                ; DATA XREF from 0x0040b0ef (unk)
│                0x004047d0      31ed           xor ebp, ebp
│                0x004047d2      4989d1         mov r9, rdx
│                0x004047d5      5e             pop rsi
│                0x004047d6      4889e2         mov rdx, rsp
│                0x004047d9      4883e4f0       and rsp, 0xfffffffffffffff0
│                0x004047dd      50             push rax
│                0x004047de      54             push rsp
│                0x004047df      49c7c0c03641.  mov r8, 0x4136c0
│                0x004047e6      48c7c1603641.  mov rcx, 0x413660      ; "AWA..AVI..AUI..ATL.%.. "
0x00413660  ; "AWA..AVI..AUI..ATL.%.. "
│                0x004047ed      48c7c7e02740.  mov rdi, main          ; "AWAVAUATUH..S..H...." @
0x4027e0
│                0x004047f4      e857dcffff     call sym.imp.__libc_start_main
╘                0x004047f9      f4             hlt
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
In this example command `pi 1` runs over all instructions in the current function (entry0).

The third way to use loops less flexible but still useful - we can loop through
predefined iterator types:

 - symbols
 - imports
 - registers
 - threads
 - comments
 - functions
 - flags

This is possible with `@@@` command. E.g., the previous example of listing
information about functions can be done with it:

```
[0x004047d6]> afi @@@ functions ~name
```
This will extract `name` field from `afi` output and will output the huge list of
function names. We can choose only second column, to remove redundant `name:` on every line:
```
[0x004047d6]> afi @@@ functions ~name[1]
```

