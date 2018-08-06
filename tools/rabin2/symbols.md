## Symbols (Exports)

With rabin2, the generated symbols list format is similar to the imports list. Use the `-s` option to get it:

```
rabin2 -s /bin/ls | head
[Symbols]
110 0x000150a0 0x000150a0 GLOBAL FUNC 56 _obstack_allocated_p
111 0x0001f600 0x0021f600 GLOBAL  OBJ  8 program_name
112 0x0001f620 0x0021f620 GLOBAL  OBJ  8 stderr
113 0x00014f90 0x00014f90 GLOBAL FUNC 21 _obstack_begin_1
114 0x0001f600 0x0021f600   WEAK  OBJ  8 program_invocation_name
115 0x0001f5c0 0x0021f5c0 GLOBAL  OBJ  8 alloc_failed_handler
116 0x0001f5f8 0x0021f5f8 GLOBAL  OBJ  8 optarg
117 0x0001f5e8 0x0021f5e8 GLOBAL  OBJ  8 stdout
118 0x0001f5e0 0x0021f5e0 GLOBAL  OBJ  8 program_short_name
```

With the `-sr` option rabin2 produces a radare2 script instead. It can later be passed to the core to automatically flag all symbols and to define corresponding byte ranges as functions and data blocks.

```
$ rabin2 -sr /bin/ls | head
fs symbols
f sym.obstack_allocated_p 56 0x000150a0
f sym.program_invocation_name 8 0x0021f600
f sym.stderr 8 0x0021f620
f sym.obstack_begin_1 21 0x00014f90
f sym.program_invocation_name 8 0x0021f600
f sym.obstack_alloc_failed_handler 8 0x0021f5c0
f sym.optarg 8 0x0021f5f8
f sym.stdout 8 0x0021f5e8
f sym.program_invocation_short_name 8 0x0021f5e0
```

