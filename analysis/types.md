Radare2 supports the C-syntax data types description.
Those types are parsed by C11-compatible parser and stored in
the internal SDB, thus introspectable with `k` command.

Most of the related commands are located in `t` namespace:
```
[0x000051c0]> t?
|Usage: t # cparse types commands
| t                       List all loaded types
| t <type>                Show type in 'pf' syntax
| t*                      List types info in r2 commands
| t- <name>               Delete types by its name
| t-*                     Remove all types
| ta <type>               Mark immediate as a type offset
| tb <enum> <value>       Show matching enum bitfield for given number
| tc ([cctype])           calling conventions listing and manipulations
| te[?]                   List all loaded enums
| te <enum> <value>       Show name for given enum number
| td[?] <string>          Load types from string
| tf                      List all loaded functions signatures
| tk <sdb-query>          Perform sdb query
| tl[?]                   Show/Link type to an address
| tn[?] [-][addr]         manage noreturn function attributes and marks
| to -                    Open cfg.editor to load types
| to <path>               Load types from C header file
| tos <path>              Load types from parsed Sdb database
| tp <type>  = <address>  cast data at <address> to <type> and print it
| ts[?]                   print loaded struct types
| tu[?]                   print loaded union types
[0x000051c0]>
```

Note, that the basic (atomic) types are not those from C standard -
not `char`, `_Bool`, or `short`. Because those types can be different
from one platform to another, radare2 uses `definite` types like as
`int8_t` or `uint64_t` and will convert `int` to `int32_t` or `int64_t`
depending on the binary or debugee platform/compiler.

Basic types can be listed using `t` command, for the structured types
you need to use `ts`, `tu` or `te` for enums:

```
[0x000051c0]> t
char
char *
int
int16_t
int32_t
int64_t
int8_t
long
long long
...
[0x000051c0]> ts
S1
[0x000051c0]>
```

There are three easy ways to define a new types: directly from the string
using `td` command, from the file using `to` command or open an `$EDITOR`
to type the definitions in place.
```
[0x000051c0]> cat ~/radare2-regressions/bins/headers/s3.h
struct S1 {
    int x[3];
    int y[4];
    int z;
};
[0x000051c0]> to ~/radare2-regressions/bins/headers/s3.h
[0x000051c0]> ts
S1
[0x000051c0]> tp S1
 x : 0x000051c0 = [ 2303323441, 2303221457, 3833809122 ]
 y : 0x000051cc = [ 1280594160, 42599821, 2370306049, 16913165 ]
 z : 0x000051dc = 1032669184
[0x000051c0]>
```

Notice here we used `tp` command, which is basically converts
the C type description (or to be precise it's SDB representation)
into the the sequence of `pf` commands. See more about [print format](basic_commands/print_modes.md).

This command performs just temporary "cast". But if we want to link some address or variable
with the chosen type, we can use `tl` command to store the relationship in SDB.
```
[0x000051c0]> tl S1 = 0x51cf
[0x000051c0]> tl
(S1)
 x : 0x000051cf = [ 2315619660, 1207959810, 34803085 ]
 y : 0x000051db = [ 2370306049, 4293315645, 3860201471, 4093649307 ]
 z : 0x000051eb = 4464399
[0x000051c0]>
```
Moreover, the link will be shown in the disassembly output or visual mode:
```
[0x000051c0 15% 300 /bin/ls]> pd $r @ entry0
            ;-- entry0:
            0x000051c0      31ed           xor ebp, ebp
            0x000051c2      4989d1         mov r9, rdx
            0x000051c5      5e             pop rsi
            0x000051c6      4889e2         mov rdx, rsp
            0x000051c9      4883e4f0       and rsp, 0xfffffffffffffff0
            0x000051cd      50             push rax
            0x000051ce      54             push rsp
(S1)
 x : 0x000051cf = [ 2315619660, 1207959810, 34803085 ]
 y : 0x000051db = [ 2370306049, 4293315645, 3860201471, 4093649307 ]
 z : 0x000051eb = 4464399
            0x000051f0      488d3d51a021.  lea rdi, loc._edata         ; 0x21f248
            0x000051f7      55             push rbp
            0x000051f8      488d0549a021.  lea rax, loc._edata         ; 0x21f248
            0x000051ff      4839f8         cmp rax, rdi
            0x00005202      4889e5         mov rbp, rsp
```

Note, that if you define one of the fields as a `char`, you will get UTF8/ASCII string printed:
```
[0x00000000]> px 32
- offset -   0 1  2 3  4 5  6 7  8 9  A B  C D  E F  0123456789ABCDEF
0x00000000  2261 7364 6173 6422 1010 2349 8473 5733  "asdasd"..#I.sW3
0x00000010  9420 3492 3400 0000 0000 0000 0000 0000  . 4.4...........
[0x00000000]> "td struct mystruct { char str[6]; int b; void *p; uint64_t l;};"
[0x00000000]> ts
mystruct
[0x00000000]> tp mystruct
 str : 0x00000000 = "asdas
   b : 0x00000006 = 269492836
   p : 0x0000000a = (qword)0x2094335773844923
   l : 0x00000012 = (qword)0x0000000000349234
[0x00000000]>
```

To see the internal representation of the structure you can use `tk` command:
```
[0x000051c0]> tk~S1
S1=struct
struct.S1=x,y,z
struct.S1.x=int32_t,0,3
struct.S1.x.meta=4
struct.S1.y=int32_t,12,4
struct.S1.y.meta=4
struct.S1.z=int32_t,28,0
struct.S1.z.meta=0
[0x000051c0]>
```

There is one more important aspect of using types in radare2 - it
can change the immediate in the opcode to the structure offset.
Lets see a simple example of [R]SI-relative addressing
```
[0x000052f0]> pd 1
            0x000052f0      488b4608       mov rax, qword [rsi + 8]    ; [0x8:8]=0
[0x000052f0]>
```
Here `8` - is some offset in the memory, where `rsi` probably holds
some structure pointer. Imagine that we have the following structures
```
[0x000052f0]> "td struct ms { char b[8]; int member1; int member2; };"
[0x000052f0]> "td struct ms1 { uint64_t a; int member1; };"
[0x000052f0]> "td struct ms2 { uint16_t a; int64_t b; int member1; };"
```
Now we need to set the proper structure member offset instead of `8` in this instruction.
At first we need to list available types matching this offset:
```
[0x000052f0]> tas 8
ms.member1
ms1.member1
[0x000052f0]>
```
Note, that `ms2` is not listed, because it has no members with offset `8`.
After listing available options we can link it to the chosen offset at
the current address:
```
[0x000052f0]> ta ms1.member1
[0x000052f0]> pd 1
            0x000052f0      488b4608       mov rax, qword [rsi + ms1.member1]    ; [0x8:8]=0
[0x000052f0]>
```

## Internal representation

Defining primitive types requires understanding of basic pf formats,
you can find the whole list of format specifier in `pf??`:
```
-----------------------------------------------------------------
|  format specifier  | explanation                              |
|---------------------------------------------------------------|
|         b          |  byte (unsigned)                         |
|         c          |  char (signed byte)                      |
|         d          |  0x%%08x hexadecimal value (4 bytes)     |
|         f          |  float value (4 bytes)                   |
|         i          |  %%i integer value (4 bytes)             |
|         o          |  0x%%08o octal value (4 byte)            |
|         p          |  pointer reference (2, 4 or 8 bytes)     |
|         q          |  quadword (8 bytes)                      |
|         s          |  32bit pointer to string (4 bytes)       |
|         S          |  64bit pointer to string (8 bytes)       |
|         t          |  UNIX timestamp (4 bytes)                |
|         T          |  show Ten first bytes of buffer          |
|         u          |  uleb128 (variable length)               |
|         w          |  word (2 bytes unsigned short in hex)    |
|         x          |  0x%%08x hex value and flag (fd @ addr)  |
|         X          |  show formatted hexpairs                 |
|         z          |  \0 terminated string                    |
|         Z          |  \0 terminated wide string               |
-----------------------------------------------------------------
```
there are basically 3 mandatory keys for defining basic data types:
`X=type`
`type.X=format_specifier`
`type.X.size=size_in_bits`
For example, lets define `UNIT`, according to [Microsoft documentation](https://msdn.microsoft.com/en-us/library/windows/desktop/aa383751(v=vs.85).aspx#UINT)
`UINT` is just equivalent of standard C `unsigned int` (or `uint32_t` in terms of TCC engine).
It will be defined as:
```
UINT=type
type.UINT=d
type.UINT.size=32
```
Now there is an optional entry:

`X.type.pointto=Y`

This one may only be used in case of pointer `type.X=p`, one good example is LPFILETIME definition,
it is pointer to `_FILETIME` which happens to be a structure.
Assuming that we are targeting only 32 bit windows machine, it will be defined as the following:
```
LPFILETIME=type
type.LPFILETIME=p
type.LPFILETIME.size=32
type.LPFILETIME.pointto=_FILETIME
```
that last field is not mandatory because some times the data structure
internals will be property, and we will not have a clean representation for it.

There is also one more optional entry:
```
type.UINT.meta=4
```
This entry is for integration with C parser and carry the type class information:
integer size, signed/unsigned, etc.

### Structures

Those are the basic keys for structs (with just two elements):

```
X=struct
struct.X=a,b
struct.X.a=a_type,a_offset,a_number_of_elements
struct.X.b=b_type,b_offset,b_number_of_elements
```
The first line is used to define a structure called `X`, second line
defines the elements of `X` as comma separated values. After that we just define each element info.

for example we can have struct like this one:
```
struct _FILETIME {
	DWORD dwLowDateTime;
	DWORD dwHighDateTime;
}
```
assuming we have `DWORD` defined, the struct will look like this
```
 _FILETIME=struct
struct._FILETIME=dwLowDateTime,dwHighDateTime
struct._FILETIME.dwLowDateTime=DWORD,0,0
struct._FILETIME.dwHighDateTime=DWORD,4,0
```
Note that the number of elements filed is used in case of arrays only
to identify how many elements are in arrays, other than that it is zero by default.

### Unions

Unions are defined exactly like structs the only difference is that you will replace the word `struct` with the word `union`.

### Function prototypes

Function prototypes representation is the most detail oriented and the most important one one of them all. Actually this is the one used directly for type matching

```
X=func
func.X.args=NumberOfArgs
func.x.arg0=Arg_type,arg_name
.
.
.
func.X.ret=Return_type
func.X.cc=calling_convention
```
It should be self explanatory lets do strncasecmp as an example for x86 arch for linux machines According to man pages, strncasecmp is defined as the following:
```
int strcasecmp(const char *s1, const char *s2);
```
When converting it into its sdb representation it will looks like the following:
```
strcasecmp=func
func.strcasecmp.args=3
func.strcasecmp.arg0=char *,s1
func.strcasecmp.arg1=char *,s2
func.strcasecmp.arg2=size_t,n
func.strcasecmp.ret=int
func.strcasecmp.cc=cdecl
```

Note that the `.cc` part is optional and if it didn't exist the default calling convention for your target architecture will be used instead.
Their is one extra optional key

```
func.x.noreturn=true/false
```
This key is used to mark functions that will not return once called like `exit` and `_exit`.


