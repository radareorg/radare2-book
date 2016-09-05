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

