## Working with data types

Radare2 can also work with data types. You can use standard C data types or define your own using C. Currently, there is a support for structs, unions, function signatures, and enums.

```
[0x00000000]> t?
Usage: t   # cparse types commands
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

### Defining new types

There are three different methods to define new types:

1. Defining a new type from r2 shell immediately, to do this you will use `td` command, and put the whole line between double quotes. For example:

`"td struct person {int age; char *name; char *address;};"`

2. You can also use `to -` to open a text editor and write your own types in there. This is preferable when you got too many types to define.

3. Radare2 also supports loading header files using the command `to` followed by a path to the header file you want to load.

You can View loaded types in r2 using `ts` for structures, `tu` for unions, `tf` for function signatures, `te` for enums.

You can also cast pointers to data types and view data in there accordingly with `tp`. EX:

```
[0x00400511]> tp person = 0x7fff170a46b0
       age : 0x7fff170a46b0 = 20
       name : (*0x4005b0) 0x7fff170a46b4 = My name
       address : (*0x4005b8) 0x7fff170a46bc = My age
[0x00400511]> 
```

