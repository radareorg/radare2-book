# Managing variables

Radare2 allows managing local variables, no matter their location, stack or registers.
The variables' auto analysis is enabled by default but can be disabled with `anal.vars`
configuration option.

The main variables commands are located in `afv` namespace:

```
Usage: afv  [rbs]
| afv*                          output r2 command to add args/locals to flagspace
| afv-([name])                  remove all or given var
| afv=                          list function variables and arguments with disasm refs
| afva                          analyze function arguments/locals
| afvb[?]                       manipulate bp based arguments/locals
| afvd name                     output r2 command for displaying the value of args/locals in the debugger
| afvf                          show BP relative stackframe variables
| afvn [new_name] ([old_name])  rename argument/local
| afvr[?]                       manipulate register based arguments
| afvR [varname]                list addresses where vars are accessed (READ)
| afvs[?]                       manipulate sp based arguments/locals
| afvt [name] [new_type]        change type for given argument/local
| afvW [varname]                list addresses where vars are accessed (WRITE)
| afvx                          show function variable xrefs (same as afvR+afvW)
```

`afvr`, `afvb` and `afvs` commands are uniform but allow manipulation of
register-based arguments and variables, BP/FP-based arguments and variables,
and SP-based arguments and variables respectively.
If we check the help for `afvr` we will get the way two others commands works too:

```
|Usage: afvr [reg] [type] [name]
| afvr                        list register based arguments
| afvr*                       same as afvr but in r2 commands
| afvr [reg] [name] ([type])  define register arguments
| afvrj                       return list of register arguments in JSON format
| afvr- [name]                delete register arguments at the given index
| afvrg [reg] [addr]          define argument get reference
| afvrs [reg] [addr]          define argument set reference
```

Like many other things variables detection is performed by radare2 automatically, but results
can be changed with those arguments/variables control commands. This kind of analysis
relies heavily on preloaded function prototypes and the calling-convention, thus loading symbols
can improve it. Moreover, after changing something we can rerun variables analysis with
`afva` command. Quite often variables analysis is accompanied with
[types analysis](types.md), see `afta` command.

The most important aspect of reverse engineering - naming things. Of course, you can rename
variable too, affecting all places it was referenced. This can be achieved with `afvn` for
_any_ type of argument or variable. Or you can simply remove the variable or argument with
`afv-` command.

As mentioned before the analysis loop relies heavily on types information while performing
variables analysis stages. Thus comes next very important command - `afvt`, which
allows you to change the type of variable:

```
[0x00003b92]> afvs
var int local_8h @ rsp+0x8
var int local_10h @ rsp+0x10
var int local_28h @ rsp+0x28
var int local_30h @ rsp+0x30
var int local_32h @ rsp+0x32
var int local_38h @ rsp+0x38
var int local_45h @ rsp+0x45
var int local_46h @ rsp+0x46
var int local_47h @ rsp+0x47
var int local_48h @ rsp+0x48
[0x00003b92]> afvt local_10h char*
[0x00003b92]> afvs
var int local_8h @ rsp+0x8
var char* local_10h @ rsp+0x10
var int local_28h @ rsp+0x28
var int local_30h @ rsp+0x30
var int local_32h @ rsp+0x32
var int local_38h @ rsp+0x38
var int local_45h @ rsp+0x45
var int local_46h @ rsp+0x46
var int local_47h @ rsp+0x47
var int local_48h @ rsp+0x48
```

Less commonly used feature, which is still under heavy development - distinction between
variables being read and written. You can list those being read with `afvR` command and those
being written with `afvW` command. Both commands provide a list of the places those operations
are performed:

```
[0x00003b92]> afvR
local_48h  0x48ee
local_30h  0x3c93,0x520b,0x52ea,0x532c,0x5400,0x3cfb
local_10h  0x4b53,0x5225,0x53bd,0x50cc
local_8h  0x4d40,0x4d99,0x5221,0x53b9,0x50c8,0x4620
local_28h  0x503a,0x51d8,0x51fa,0x52d3,0x531b
local_38h
local_45h  0x50a1
local_47h
local_46h
local_32h  0x3cb1
[0x00003b92]> afvW
local_48h  0x3adf
local_30h  0x3d3e,0x4868,0x5030
local_10h  0x3d0e,0x5035
local_8h  0x3d13,0x4d39,0x5025
local_28h  0x4d00,0x52dc,0x53af,0x5060,0x507a,0x508b
local_38h  0x486d
local_45h  0x5014,0x5068
local_47h  0x501b
local_46h  0x5083
local_32h
[0x00003b92]>
```

## Type inference

The type inference for local variables and arguments is well integrated with the command `afta`.

Let's see an example of this with a simple [hello_world](https://github.com/radareorg/radare2book/tree/master/examples/hello_world) binary

```
[0x000007aa]> pdf
|           ;-- main:
/ (fcn) sym.main 157
| sym.main ();
| ; var int local_20h @ rbp-0x20
| ; var int local_1ch @ rbp-0x1c
| ; var int local_18h @ rbp-0x18
| ; var int local_10h @ rbp-0x10
| ; var int local_8h @ rbp-0x8
| ; DATA XREF from entry0 (0x6bd)
| 0x000007aa  push rbp
| 0x000007ab  mov rbp, rsp
| 0x000007ae  sub rsp, 0x20
| 0x000007b2  lea rax, str.Hello          ; 0x8d4 ; "Hello"
| 0x000007b9  mov qword [local_18h], rax
| 0x000007bd  lea rax, str.r2_folks       ; 0x8da ; " r2-folks"
| 0x000007c4  mov qword [local_10h], rax
| 0x000007c8  mov rax, qword [local_18h]
| 0x000007cc  mov rdi, rax
| 0x000007cf  call sym.imp.strlen         ; size_t strlen(const char *s)
```

* After applying `afta`

```
[0x000007aa]> afta
[0x000007aa]> pdf
| ;-- main:
| ;-- rip:
/ (fcn) sym.main 157
| sym.main ();
| ; var size_t local_20h @ rbp-0x20
| ; var size_t size @ rbp-0x1c
| ; var char *src @ rbp-0x18
| ; var char *s2 @ rbp-0x10
| ; var char *dest @ rbp-0x8
| ; DATA XREF from entry0 (0x6bd)
| 0x000007aa  push rbp
| 0x000007ab  mov rbp, rsp
| 0x000007ae  sub rsp, 0x20
| 0x000007b2  lea rax, str.Hello          ; 0x8d4 ; "Hello"
| 0x000007b9  mov qword [src], rax
| 0x000007bd  lea rax, str.r2_folks       ; 0x8da ; " r2-folks"
| 0x000007c4  mov qword [s2], rax
| 0x000007c8  mov rax, qword [src]
| 0x000007cc  mov rdi, rax                ; const char *s
| 0x000007cf  call sym.imp.strlen         ; size_t strlen(const char *s)
```

It also extracts type information from format strings like `printf ("fmt : %s , %u , %d", ...)`, the format specifications are extracted from `anal/d/spec.sdb`

You could create a new profile for specifying a set of format chars depending on different libraries/operating systems/programming languages like this :

```
win=spec
spec.win.u32=unsigned int
```
Then change your default specification to newly created one using this config variable `e anal.spec = win`

For more information about primitive and user-defined types support in radare2 refer to [types](types.md) chapter.
