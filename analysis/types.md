# Types

Radare2 supports the C-syntax data types description.
Those types are parsed by a C11-compatible parser and stored in
the internal SDB, thus are introspectable with `k` command.

Most of the related commands are located in `t` namespace:

```
[0x000051c0]> t?
| Usage: t   # cparse types commands
| t                          List all loaded types
| tj                         List all loaded types as json
| t <type>                   Show type in 'pf' syntax
| t*                         List types info in r2 commands
| t- <name>                  Delete types by its name
| t-*                        Remove all types
| tail [filename]            Output the last part of files
| tc [type.name]             List all/given types in C output format
| te[?]                      List all loaded enums
| td[?] <string>             Load types from string
| tf                         List all loaded functions signatures
| tk <sdb-query>             Perform sdb query
| tl[?]                      Show/Link type to an address
| tn[?] [-][addr]            manage noreturn function attributes and marks
| to -                       Open cfg.editor to load types
| to <path>                  Load types from C header file
| toe [type.name]            Open cfg.editor to edit types
| tos <path>                 Load types from parsed Sdb database
| tp  <type> [addr|varname]  cast data at <address> to <type> and print it (XXX: type can contain spaces)
| tpv <type> @ [value]       Show offset formatted for given type
| tpx <type> <hexpairs>      Show value for type with specified byte sequence (XXX: type can contain spaces)
| ts[?]                      Print loaded struct types
| tu[?]                      Print loaded union types
| tx[f?]                     Type xrefs
| tt[?]                      List all loaded typedefs
```

Note that the basic (atomic) types are not those from C standard -
not `char`, `_Bool`, or `short`. Because those types can be different
from one platform to another, radare2 uses `definite` types like as
`int8_t` or `uint64_t` and will convert `int` to `int32_t` or `int64_t`
depending on the binary or debuggee platform/compiler.

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
```

### Loading types

There are three easy ways to define a new type:
* Directly from the string using `td` command
* From the file using `to <filename>` command
* Open  an `$EDITOR` to type the definitions in place using `to -`

```
[0x000051c0]> "td struct foo {char* a; int b;}"
[0x000051c0]> cat ~/radare2-regressions/bins/headers/s3.h
struct S1 {
    int x[3];
    int y[4];
    int z;
};
[0x000051c0]> to ~/radare2-regressions/bins/headers/s3.h
[0x000051c0]> ts
foo
S1
```

Also note there is a config option to specify include directories for types parsing

```
[0x00000000]> e??~dir.type
dir.types: Default path to look for cparse type files
[0x00000000]> e dir.types
/usr/include
```

### Printing types

Notice below we have used `ts` command, which basically converts
the C type description (or to be precise it's SDB representation)
into the sequence of `pf` commands. See more about [print format](../basic_commands/print_modes.md).

The `tp` command uses the `pf` string to print all the members of type at the current offset/given address:

```
[0x000051c0]> ts foo
pf zd a b
[0x000051c0]> tp foo
 a : 0x000051c0 = 'hello'
 b : 0x000051cc = 10
[0x000051c0]> tp foo 0x000053c0
 a : 0x000053c0 = 'world'
 b : 0x000053cc = 20
```

Also, you could fill your own data into the struct and print it using `tpx` command

```
[0x000051c0]> tpx foo 4141414144141414141442001000000
 a : 0x000051c0 = AAAAD.....B
 b : 0x000051cc = 16
```

### Linking Types

The `tp` command just performs a temporary cast. But if we want to link some address or variable
with the chosen type, we can use `tl` command to store the relationship in SDB.

```
[0x000051c0]> tl S1 = 0x51cf
[0x000051c0]> tll
(S1)
 x : 0x000051cf = [ 2315619660, 1207959810, 34803085 ]
 y : 0x000051db = [ 2370306049, 4293315645, 3860201471, 4093649307 ]
 z : 0x000051eb = 4464399
```

Moreover, the link will be shown in the disassembly output or visual mode:

```
[0x000051c0 15% 300 /bin/ls]> pd $r @ entry0
 ;-- entry0:
 0x000051c0      xor ebp, ebp
 0x000051c2      mov r9, rdx
 0x000051c5      pop rsi
 0x000051c6      mov rdx, rsp
 0x000051c9      and rsp, 0xfffffffffffffff0
 0x000051cd      push rax
 0x000051ce      push rsp
(S1)
 x : 0x000051cf = [ 2315619660, 1207959810, 34803085 ]
 y : 0x000051db = [ 2370306049, 4293315645, 3860201471, 4093649307 ]
 z : 0x000051eb = 4464399
 0x000051f0      lea rdi, loc._edata         ; 0x21f248
 0x000051f7      push rbp
 0x000051f8      lea rax, loc._edata         ; 0x21f248
 0x000051ff      cmp rax, rdi
 0x00005202      mov rbp, rsp
```

Once the struct is linked, radare2 tries to propagate structure offset in the function at current offset, to run this analysis on whole program or at any targeted functions after all structs are linked you have `aat` command:

```
[0x00000000]> aa?
| aat [fcn]           Analyze all/given function to convert immediate to linked structure offsets (see tl?)
```

Note sometimes the emulation may not be accurate, for example as below :

````
|0x000006da  push rbp
|0x000006db  mov rbp, rsp
|0x000006de  sub rsp, 0x10
|0x000006e2  mov edi, 0x20               ; "@"
|0x000006e7  call sym.imp.malloc         ;  void *malloc(size_t size)
|0x000006ec  mov qword [local_8h], rax
|0x000006f0  mov rax, qword [local_8h]

````

The return value of `malloc` may differ between two emulations, so you have to set the hint for return value manually using `ahr` command, so run `tl` or `aat` command after setting up the return value hint.

```
[0x000006da]> ah?
| ahr val            set hint for return value of a function
```

### Structure Immediates

There is one more important aspect of using types in radare2 - using `aht` you
can change the immediate in the opcode to the structure offset.
Lets see a simple example of [R]SI-relative addressing

```
[0x000052f0]> pd 1
0x000052f0      mov rax, qword [rsi + 8]    ; [0x8:8]=0
```
Here `8` - is some offset in the memory, where `rsi` probably holds
some structure pointer. Imagine that we have the following structures
```

[0x000052f0]> "td struct ms { char b[8]; int member1; int member2; };"
[0x000052f0]> "td struct ms1 { uint64_t a; int member1; };"
[0x000052f0]> "td struct ms2 { uint16_t a; int64_t b; int member1; };"
```
Now we need to set the proper structure member offset instead of `8` in this instruction.
At first, we need to list available types matching this offset:

```
[0x000052f0]> ahts 8
ms.member1
ms1.member1
```

Note, that `ms2` is not listed, because it has no members with offset `8`.
After listing available options we can link it to the chosen offset at
the current address:

```
[0x000052f0]> aht ms1.member1
[0x000052f0]> pd 1
0x000052f0      488b4608       mov rax, qword [rsi + ms1.member1]    ; [0x8:8]=0
```

### Managing enums

* Printing all fields in enum using `te` command

```
[0x00000000]> "td enum Foo {COW=1,BAR=2};"
[0x00000000]> te Foo
COW = 0x1
BAR = 0x2
```

* Finding matching enum member for given bitfield and vice-versa

```
[0x00000000]> te Foo 0x1
COW
[0x00000000]> teb Foo COW
0x1
```

## Internal representation

To see the internal representation of the types you can use `tk` command:

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

Defining primitive types requires an understanding of basic `pf` formats,
you can find the whole list of format specifier in `pf??`:

```
-----------------------------------------------------
| format | explanation                              |
|---------------------------------------------------|
|  b     |  byte (unsigned)                         |
|  c     |  char (signed byte)                      |
|  d     |  0x%%08x hexadecimal value (4 bytes)     |
|  f     |  float value (4 bytes)                   |
|  i     |  %%i integer value (4 bytes)             |
|  o     |  0x%%08o octal value (4 byte)            |
|  p     |  pointer reference (2, 4 or 8 bytes)     |
|  q     |  quadword (8 bytes)                      |
|  s     |  32bit pointer to string (4 bytes)       |
|  S     |  64bit pointer to string (8 bytes)       |
|  t     |  UNIX timestamp (4 bytes)                |
|  T     |  show Ten first bytes of buffer          |
|  u     |  uleb128 (variable length)               |
|  w     |  word (2 bytes unsigned short in hex)    |
|  x     |  0x%%08x hex value and flag (fd @ addr)  |
|  X     |  show formatted hexpairs                 |
|  z     |  \0 terminated string                    |
|  Z     |  \0 terminated wide string               |
-----------------------------------------------------

```
there are basically 3 mandatory keys for defining basic data types:
`X=type`
`type.X=format_specifier`
`type.X.size=size_in_bits`
For example, let's define `UNIT`, according to [Microsoft documentation](https://msdn.microsoft.com/en-us/library/windows/desktop/aa383751%28v=vs.85%29.aspx#UINT)
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
it is a pointer to `_FILETIME` which happens to be a structure.
Assuming that we are targeting only 32-bit windows machine, it will be defined as the following:

```
LPFILETIME=type
type.LPFILETIME=p
type.LPFILETIME.size=32
type.LPFILETIME.pointto=_FILETIME
```

This last field is not mandatory because sometimes the data structure
internals will be proprietary, and we will not have a clean representation for it.

There is also one more optional entry:

```
type.UINT.meta=4
```

This entry is for integration with C parser and carries the type class information:
integer size, signed/unsigned, etc.

### Structures

Those are the basic keys for structs (with just two elements):

```
X=struct
struct.X=a,b
struct.X.a=a_type,a_offset,a_number_of_elements
struct.X.b=b_type,b_offset,b_number_of_elements
```

The first line is used to define a structure called `X`, the second line
defines the elements of `X` as comma separated values. After that, we just define each element info.

For example. we can have a struct like this one:

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

Note that the number of elements field is used in case of arrays only
to identify how many elements are in arrays, other than that it is zero by default.

### Unions

Unions are defined exactly like structs the only difference is that you will replace the word `struct` with the word `union`.

### Function prototypes

Function prototypes representation is the most detail oriented and the most important one of them all. Actually, this is the one used directly for type matching

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

It should be self-explanatory. Let's do strncasecmp as an example for x86 arch for Linux machines. According to man pages, strncasecmp is defined as the following:

```
int strcasecmp(const char *s1, const char *s2, size_t n);
```

When converting it into its sdb representation it will look like the following:

```
strcasecmp=func
func.strcasecmp.args=3
func.strcasecmp.arg0=char *,s1
func.strcasecmp.arg1=char *,s2
func.strcasecmp.arg2=size_t,n
func.strcasecmp.ret=int
func.strcasecmp.cc=cdecl
```

Note that the `.cc` part is optional and if it didn't exist the default calling-convention for your target architecture will be used instead.
There is one extra optional key

```
func.x.noreturn=true/false
```

This key is used to mark functions that will not return once called, such as `exit` and `_exit`.
