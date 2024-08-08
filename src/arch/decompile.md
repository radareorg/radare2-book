## Decompilation

Radare2, as a tool that focus on extensibility and flexibility provides support for many decompilers.

For historical reasons the decompilers in r2 has been allocated as `pd` subcommands.

* `pdd` - r2dec
* `pdg` - r2ghidra
* ...

By default only the `pdc` pseudodecompiler is shipped within radare2, but you can install any other via `r2pm`, the standard package manager for radare2.

Most decompilers implement all the common subcommands that modify the output:

* pdgo/pddo/pdco -> show offset of instruction associated with each line
* pdga/pdda/pdca -> show two column disasm vs decompilation
* pdgj/pddj/pdcj -> json output to use decompiler info from other tools

### PseudoDecompiler

By combining ESIL emulation, asm.pseudo disassembly and some extra reference processing and function signature, comments and metadata; the `pdc` command provides a quick way to read a function in a higher level representation. It is not really implementing any control flow improvement (like switch, if/else, for/while). Also, no code optimizations or garbage logic is removed.

You may find it's output quite verbose and noisy, but handy and fast, and that serves like a good source to feed language models.

Another benefit of `pdc` is that it is available for ALL architectures supported by r2.

```console
[0x100003a48]> pdc
int sym.func.100003a48 (int x0, int x1) {
        x8 = [x0 + 0x60]  // arg1
        x8 = [x8 + 0x60]
        x9 = [x1 + 0x60]  // arg2
        x9 = [x9 + 0x60]
        (a, b) = compare (x8, x9)
        if (a <= b) goto loc_0x100003a68 // likely
        goto loc_0x100003a60;
    loc_0x100003a68:
        if (a >= b) goto loc_0x100003a74 // likely
        goto loc_0x100003a6c;
    loc_0x100003a74:
        x8 = x1 + 0x68 // arg2
        x1 = x0 + 0x68 // arg1
        x0 = x8
        return sym.imp.strcoll("", "")
    loc_0x100003a60:
        w0 = 1
        return x0;
}
[0x100003a48]>
```

### r2dec

This decompiler is available via `r2pm` and is sits after the `pdd` command. It provides control flow analysis and some code cleanup which makes it easier for the reader to understand what is going on.

This plugin can be configured with the `e r2dec.` variables:

```console
[0x00000000]> e??r2dec.
       r2dec.asm: if true, shows pseudo next to the assembly.
    r2dec.blocks: if true, shows only scopes blocks.
     r2dec.casts: if false, hides all casts in the pseudo code.
     r2dec.debug: do not catch exceptions in r2dec.
 r2dec.highlight: highlights the current address.
     r2dec.paddr: if true, all xrefs uses physical addresses compare.
      r2dec.slow: load all the data before to avoid multirequests to r2.
     r2dec.xrefs: if true, shows all xrefs in the pseudo code.
[0x00000000]>
```

In this example we show how `pdda` works, displaying the two columns:

```console
[0x100003a48]> pdda
; assembly                     | /* r2dec pseudo code output */
                               | /* /bin/ls @ 0x100003a48 */
                               | #include <stdint.h>
                               |
; (fcn) sym.func.100003a48 ()  | uint32_t func_100003a48 (int64_t arg1, int64_t arg2) {
                               |     x0 = arg1;
                               |     x1 = arg2;
0x100003a48 ldr x8, [x0, 0x60] |     x8 = *((x0 + 0x60));
0x100003a4c ldr x8, [x8, 0x60] |     x8 = *((x8 + 0x60));
0x100003a50 ldr x9, [x1, 0x60] |     x9 = *((x1 + 0x60));
0x100003a54 ldr x9, [x9, 0x60] |     x9 = *((x9 + 0x60));
0x100003a58 cmp x8, x9         |
                               |     if (x8 > x9) {
0x100003a5c b.le 0x100003a68   |
0x100003a60 mov w0, 1          |         w0 = 1;
0x100003a64 ret                |         return w0;
                               |     }
                               |     if (x8 < x9) {
0x100003a68 b.ge 0x100003a74   |
0x100003a6c mov w0, -1         |         w0 = -1;
0x100003a70 ret                |         return w0;
                               |     }
0x100003a74 add x8, x1, 0x68   |     x8 = x1 + 0x68;
0x100003a78 add x1, x0, 0x68   |     x1 = x0 + 0x68;
0x100003a7c mov x0, x8         |     x0 = x8;
0x100003a80 b 0x1000077c8      |     return void (*0x1000077c8)() ();
                               | }
[0x100003a48]>
```

### R2Ghidra

The Ghidra tool ships a decompiler as a separate program (written in C++ instead of Java), for r2 purposes the logic from this tool has been massaged to work as a native plugin so it doesn't require the java runtime to work.

Note that the quality of the decompilation of r2ghidra compared to ghidra is not the same, because r2ghidra is not providing the same analysis results that Ghidra would provide, and some other metadata differs, which causes the engine to behave different and probably miss quite a lot of details when handling structures and other complex features.

The plugin can be configured with the `e r2ghidra.` variables:

```console
[0x00000000]> e??r2ghidra.
      r2ghidra.casts: Show type casts where needed
    r2ghidra.cmt.cpp: C++ comment style
 r2ghidra.cmt.indent: Comment indent
     r2ghidra.indent: Indent increment
       r2ghidra.lang: Custom Sleigh ID to override auto-detection (e.g. x86:LE:32:default)
    r2ghidra.linelen: Max line length
 r2ghidra.maximplref: Maximum number of references to an expression before showing an explicit variable.
     r2ghidra.rawptr: Show unknown globals as raw addresses instead of variables
     r2ghidra.roprop: Propagate read-only constants (0,1,2,3,4)
 r2ghidra.sleighhome: SLEIGHHOME
    r2ghidra.timeout: Run decompilation in a separate process and kill it after a specific time
       r2ghidra.vars: Honor local variable / argument analysis from r2 (may cause segfaults if enabled)
    r2ghidra.verbose: Show verbose warning messages while decompiling
[0x00000000]>
```

In this example we see how `pdgo` works, displaying the

```console
[0x100003a48]> pdgo
0x100003a48 |ulong sym.func.100003a48(int64_t param_1, int64_t param_2) {
            |    ulong uVar1;
            |    int64_t iVar2;
            |    int64_t iVar3;
            |
0x100003a4c |    iVar2 = *(*(param_1 + 0x60) + 0x60);
0x100003a54 |    iVar3 = *(*(param_2 + 0x60) + 0x60);
0x100003a5c |    if (iVar2 != iVar3 && iVar3 <= iVar2) {
0x100003a64 |        return 1;
            |    }
0x100003a68 |    if (iVar2 < iVar3) {
0x100003a70 |        return 0xffffffff;
            |    }
0x1000077d4 |    uVar1 = (**(segment.__DATA_CONST + 0x1f0))(param_2 + 0x68, param_1 + 0x68);
0x1000077d4 |    return uVar1;
            |}
[0x100003a48]>
```

### Other

There's support for many other decompilers in radare2, but those are not documented in this book yet, feel free to submit your details, here's the list:

* r2jadx -> java/dalvik decompilation
* ctags -> use source ctags to show the source from disasm
* retdec -> available as a plugin and uses the `pde`
* pickledec -> decompiler for Python pickle blobs
* radeco -> experimental and abandoned esil based decompiler written in Rust
* r2snow -> snowman's decompiler only for intel architectures
* pdq -> r2papi-based decompiler on top of esil and the r2js runtime
