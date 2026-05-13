## ROP Gadgets

The `/g` command searches for gadgets: short instruction sequences ending in a control-flow transfer that can be chained together to build exploits. Historically this was the `/R` command, but it has been renamed to `/g` and the old form now just prints a hint:

```console
[0x00000000]> /R pop
ERROR: /R is now known as /g
```

The grammar is the same as before. You pass a list of opcode substrings separated by semicolons, and radare2 scans the search range for terminator instructions (return, indirect call or indirect jump), then walks back up to `gadget.len` instructions from each terminator to collect every sequence whose disassembly matches the given filters in order:

```console
[0x00000000]> /g pop;ret
  0x00000000             4889e5  mov rbp, rsp
  0x00000003             8b4508  mov eax, dword [rbp + 8]
  0x00000006               31c9  xor ecx, ecx
  0x00000008                 5d  pop rbp
  0x00000009                 c3  ret
...
```

An empty filter (`/g`) lists every gadget found within `gadget.len` instructions of a terminator. Output modes follow the usual conventions: `/gj` for JSON, `/gq` for a quiet `address size` listing, and `/g*` for radare2 commands suitable for piping back in.

### Gadget Families

Until recently the only gadget terminator radare2 considered was `ret`. The new `/g` engine generalizes this with three sibling commands that change the terminator type:

| Command | Terminator           | Acronym                           |
| ------- | -------------------- | --------------------------------- |
| `/gR`   | return instructions  | ROP (Return-Oriented Programming) |
| `/gC`   | call instructions    | COP (Call-Oriented Programming)   |
| `/gJ`   | jump instructions    | JOP (Jump-Oriented Programming)   |
| `/g`    | any of the three     | (combined)                        |

All four accept the same opcode-grep syntax and the same `/jq*` output modifiers, so `/gJj jmp` lists JOP gadgets as JSON.

### Configuration Variables

The behavior of the gadget search is controlled by a group of `gadget.*` evars. The defaults are conservative; enable the rest as the analysis demands:

```console
[0x00000000]> e?? gadget.
     gadget.comments: display comments in gadget search output
         gadget.cond: include conditional jump, calls and returns in gadget search
           gadget.db: categorize gadgets in sdb
         gadget.esil: use ESIL to resolve and classify conditional gadgets
          gadget.len: maximum gadget length
          gadget.sdb: cache results in sdb (experimental)
    gadget.subchains: display every length gadget from gadget.len=X to 2 in /gl
```

* `gadget.len` (default `5`) caps how many instructions can precede the terminator. Increase it to find longer chains, decrease it when you only care about the tail.
* `gadget.cond` widens the terminator set to include conditional returns, calls and jumps (`cret`, `ccall`, `cjmp`). Useful on ARM/Thumb and on x86 binaries where the only available pivots are `je`/`jne`.
* `gadget.esil` runs the ESIL emulator on each candidate to evaluate side effects and the branch condition. It enables the richer classification described below.
* `gadget.db` (on by default) and `gadget.sdb` push results into SDB namespaces so they can be queried with `/gk` without re-running the search.
* `gadget.subchains` makes the quiet (`/gq`) output emit every prefix of a hit, from `gadget.len` instructions down to 2, instead of just the longest match.
* `gadget.comments` interleaves any user-defined comments on the gadget's addresses into the listing.

### Classification

When `/g` finishes it tags each hit with a set of classes. The JSON form makes them explicit:

```console
[0x00000000]> /gj pop;ret
[{"opcodes":[...,{"opcode":"pop rbp"},{"opcode":"ret"}],
  "retaddr":9,"size":10,
  "classes":["ret","mov","ldconst","logic"]}, ...]
```

The class set captures both the terminator family and the primitive operations the gadget performs on the way to it:

| Class             | Meaning                                                                |
| ----------------- | ---------------------------------------------------------------------- |
| `ret`             | gadget ends in a return                                                |
| `jop`             | gadget ends in an indirect/computed jump                               |
| `cop`             | gadget ends in an indirect/computed call                               |
| `syscall`         | gadget contains a syscall/swi instruction                              |
| `signal`          | gadget triggers a signal-like trap (`int`, `ud2`, ...)                 |
| `pivot`           | gadget modifies the stack pointer (useful for stack pivoting)          |
| `memread`         | gadget reads from memory                                               |
| `memwrite`        | gadget writes to memory                                                |
| `rww`             | gadget can be used as a "read-what-where" primitive                    |
| `www`             | gadget can be used as a "write-what-where" primitive                   |
| `mov`             | gadget moves data between registers                                    |
| `ldconst`         | gadget loads a constant into a register                                |
| `arithm`          | gadget performs arithmetic (add, sub, mul, div)                        |
| `logic`           | gadget performs bitwise logic (and, or, xor)                           |
| `shift`           | gadget performs a shift or rotate                                      |
| `cmp`             | gadget performs a comparison                                           |
| `cond.always`     | (`gadget.esil`) the conditional terminator is always taken             |
| `cond.controlled` | (`gadget.esil`) the condition depends on attacker-controlled registers |

The first three (`ret`, `jop`, `cop`) overlap with the family commands above. The `cond.*` classes are only set when `gadget.esil=true`, since they require evaluating the branch under symbolic register values to decide whether the path is forced, dead, or attacker-influenceable.

A handful of legacy classes (`nop`, `mov`, `const`, `arithm`, `arithm_ct`) are still produced by the older rop classifier and stored in the `rop` SDB namespace for backwards compatibility.

### Querying Stored Gadgets with `/gk`

Whenever `gadget.db` is enabled, every gadget the search finds is indexed in the `gadget` SDB namespace, keyed by class. The `/gk` command queries that index without re-walking the file:

```console
[0x00000000]> /gk?
Usage: /gk  query stored gadgets
| /gk [ret|jop|cop|syscall|pivot|memread|memwrite|www]  show gadgets
| /gk [rww|signal|mov|ldconst|arithm|logic|shift|cmp]   show primitive classes
| /gk [cond.always|cond.controlled]                     show ESIL-classified conditional gadgets
| /gk [nop|mov|const|arithm|arithm_ct]                  show legacy rop classes
| /gkj                                                  json output
| /gkq                                                  list Gadgets offsets
```

A typical workflow looks like this. Run a wide search once, then narrow the results by class:

```console
[0x00000000]> e gadget.esil=true
[0x00000000]> /g
... (collects every gadget under gadget.len) ...
[0x00000000]> /gk ret
ret.0x00000000=0xa ret
ret.0x00000001=0x9 ret
ret.0x00000002=0x8 ret
ret.0x00000005=0x5 ret
[0x00000000]> /gk memwrite
memwrite.0x00000002=0x8 memwrite
memwrite.0x00000005=0x5 memwrite
```

The value column is `size class`, where `size` is the gadget length in bytes. `/gkj` returns the same information as a JSON array of `{address, size, type, effect}` objects, and `/gkq` lists just the addresses, ready to be fed into a loop with `@@=`.

### A Complete Example

Combining everything, here is a session that locates write-what-where ROP gadgets in a binary, considering both regular returns and conditional ones, with ESIL-based reachability:

```console
[0x00000000]> e gadget.len=6
[0x00000000]> e gadget.cond=true
[0x00000000]> e gadget.esil=true
[0x00000000]> /g
[0x00000000]> /gk www
[0x00000000]> /gk cond.controlled
```

The first `/g` populates the SDB; the two `/gk` queries then enumerate write-what-where primitives and any conditional terminators whose taken/not-taken state can be steered by controlling the input registers.
