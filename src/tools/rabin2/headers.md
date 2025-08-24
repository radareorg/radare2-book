### Headers

Rabin2's `-I` option is the quick, one-shot way to get a summary of a file's binary headers and general properties without fully loading it into radare2. This is especially handy for scripting, triage, or replacing small helper scripts (for example, checksec.sh) because it exposes security-relevant attributes (NX, RELRO, stack canary, PIE/pic, etc.) and other metadata (architecture, bits, interpreter, section layout hints).

The output is intentionally concise and machine-friendly — each property is printed as a key/value pair. Example:

```console
$ rabin2 -I /bin/ls
arch     x86
binsz    128456
bintype  elf
bits     64
canary   true
class    ELF64
crypto   false
endian   little
havecode true
intrp    /lib64/ld-linux-x86-64.so.2
lang     c
linenum  false
lsyms    false
machine  AMD x86-64 architecture
maxopsz  16
minopsz  1
nx       true
os       linux
pcalign  0
pic      true
relocs   false
relro    partial
rpath    NONE
static   false
stripped true
subsys   linux
va       true
```

What each important field means

- arch / asm.arch: CPU architecture family (x86, arm, mips, etc.).
- bits / asm.bits: Operand width / target word size (32 / 64).
- bintype / file.type: Binary container format (elf / pe / mach0 / coredump / raw).
- class: More specific layout (e.g. ELF64 / ELF32).
- endian: Endianness (little / big).
- machine: Human-readable CPU model.
- intrp: Program interpreter (dynamic loader), e.g. /lib64/ld-linux-x86-64.so.2.
- binsz: Total size of the file in bytes.
- havecode: Whether executable code sections have been detected.
- nx: NX / DEP enabled (true when the binary's segments are not executable where they shouldn't be).
- canary: Stack-smashing protector (stack canary) presence (true/false).
- relro: RELocation Read-Only status; typical values: none / partial / full.
- pic / pie: Whether the binary is position-independent (PIC for shared objects, PIE for executables). In rabin2 output `pic` is generally true for shared objects and position-independent executables.
- rpath / runpath: RPATH or RUNPATH embedded in the binary (or NONE).
- static: Whether the binary is fully statically linked.
- stripped: Symbols stripped from the binary (true when symbol tables are absent).
- lsyms / linenum: Presence of local symbols or line number info (debug information).
- pcalign: Preferred alignment for program/sections.
- minopsz / maxopsz: Minimum/maximum instruction sizes reported for the architecture.
- va: Whether the binary uses virtual addresses (i.e. mapped with proper virtual addressing).
- crypto: Whether cryptographic features (or crypto sections) were detected — usually false for normal binaries.

Mapping rabin2 fields to checksec output

The popular checksec.sh script reports security hardening flags like RELRO, Canary, NX, PIE, RPATH/RUNPATH. You can get the same information with rabin2 -I:

- RELRO: map to `relro` (none / partial / full)
- Stack Canary: `canary` (true/false)
- NX / DEP: `nx` (true/false)
- PIE: `pic` / `asm.bits` + PIE detection (rabin2 reports `pic`/`pie`-related properties)
- RPATH/RUNPATH: `rpath` (or `runpath` if present)

Because rabin2 reads binary headers directly (using rbin), it can be used as a drop-in replacement for checksec in many cases. Example quick checks:

```console
$ rabin2 -I /path/to/bin | rg -E "relro|canary|nx|pic|rpath|static"
relro    partial
canary   true
nx       true
pic      true
rpath    NONE
static   false
```

Or produce a JSON-like output suitable for automation (combine with other rabin2 options):

```console
# Get headers + libs + imports in a compact form
$ rabin2 -I -l -i /path/to/bin
# Or generate radare2 script-style settings to import into r2 session (-Ir)
$ rabin2 -Ir /path/to/bin
```

Using the -Ir option

Passing `-Ir` to rabin2 emits radare2 config commands (``e key=value``) so you can import the detected properties directly into a running radare2 session or into an r2 script. This is what radare2 uses internally when it loads an external header parser. Example:

```console
$ rabin2 -Ir /bin/ls
e cfg.bigendian=false
e asm.bits=64
e asm.dwarf=true
e bin.lang=c
e file.type=elf
e asm.os=linux
e asm.arch=x86
e asm.pcalign=0
```

These `e` assignments help radare2 set its environment (architecture, endianness, bits, file type, and assembler options) automatically.

Relating rabin2 to radare2 interactive info commands (iH / ih)

Inside a radare2 session there are a family of `i` (info) commands that provide quick access to the same metadata you get from rabin2. The most relevant to headers are the header/info variants — typically invoked as `iH` (capital H) and `ih` in the radare2 command set. They expose the binary header metadata, interpreter, section/segment hints, security flags, and other attributes discovered by the rbin subsystem.

- `iH` / `ih`: show headers and high-level metadata when working interactively. They allow you to inspect the same security flags (NX, RELRO, canary, PIE) and interpreter/settings without leaving an r2 session.
- `i?` family: there are many other `i*` commands (imports, exports, libraries, sections, strings) that correlate directly to rabin2's `-i`, `-E`, `-l`, `-S`, `-z`, etc.

Note: rabin2 is essentially a command-line wrapper around the rbin API — the same code radare2 uses internally — so the information is consistent between both tools. Use rabin2 for quick external inspection and `iH`/`ih` when you are already inside an r2 analysis session.

Examples and practical tips

- Quick security triage of a binary (checksec replacement):

```console
# single-line human friendly output (tailor with awk/rg/jq)
$ rabin2 -I /bin/ls

# script-friendly (radare2 commands):
$ rabin2 -Ir /bin/ls

# combine with other rabin2 flags for more info
$ rabin2 -I -S -l -i -z /bin/ls
```

- Automating checks (example shell snippet):

```bash
#!/usr/bin/env bash
file=${1:-/bin/ls}
info=$(rabin2 -I "$file")
relro=$(echo "$info" | awk '/^relro/{print $2}')
canary=$(echo "$info" | awk '/^canary/{print $2}')
nx=$(echo "$info" | awk '/^nx/{print $2}')
pic=$(echo "$info" | awk '/^pic/{print $2}')
printf "RELRO=%s\nCANARY=%s\nNX=%s\nPIC=%s\n" "$relro" "$canary" "$nx" "$pic"
```

- When you want r2 to automatically adopt the environment detected by rabin2 (arch/endianness/bits), pipe `rabin2 -Ir` output into radare2 or include it in your r2 session scripts.

Summary

Rabin2 `-I` is a fast, scriptable way to extract header metadata and security-relevant flags from binaries. It maps cleanly to the properties reported by tools such as checksec.sh and the interactive radare2 `iH`/`ih` info commands. Use `-Ir` when you want to import settings into radare2 automatically.
