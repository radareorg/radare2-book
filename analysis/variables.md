Radare2 allows to manage local variables, no matter of their location, stack or registers.
The variables autoanalysis is enabled by default, but can be disabled with `anal.vars`
configuration option.

The main variables commands are located in `afv` namespace:
```
|Usage: afv[rbs]
| afvr[?]                     manipulate register based arguments
| afvb[?]                     manipulate bp based arguments/locals
| afvs[?]                     manipulate sp based arguments/locals
| afv*                        output r2 command to add args/locals to flagspace
| afvR [varname]              list addresses where vars are accessed (READ)
| afvW [varname]              list addresses where vars are accessed (WRITE)
| afva                        analyze function arguments/locals
| afvd name                   output r2 command for displaying the value of args/locals in the
debugger
| afvn [old_name] [new_name]  rename argument/local
| afvt [name] [new_type]      change type for given argument/local
| afv-([name])                remove all or given var
```

`afvr`, `afvb` and `afvs` commands are uniform but allows manipulation of
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

Like many other things variables detection performed by radare2 automatically, but results
can be changed with those arguments/variables control commands. This kind of analysis
relies heavily on preloaded function prototypes and calling convention, thus loading symbols
can improve it. Moreover, after changing something we can rerun variables analysis with
`afva` command. Quite often variables analysis is accompanied with
[types analysis](analysis/types.md), see `afta` command.

The most important aspect of reverse engineering - naming things. Of course you can rename
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
variables being read and write. You can list those being read with `afvR` command and those
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
