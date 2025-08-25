## Implementing a new architecture

radare2 splits the logic of a CPU into several modules. You should write more than one plugin to get full support for a specific arch. Let's see which are those:

* `r_arch` architecture plugin (assemble, disassemble, analyze, ESIL uplift and register profiles)
* `r_anal` types, code and data analysis plugins
* `r_reg` registers arena definitions
* `r_syscall` system calls databases
* `r_debug` debugger and emulation runtimes

The most basic feature you usually want to support from a specific architecture is the disassembler. You first need to read into a human readable form the bytes in there.

Bear in mind that plugins can be compiled static or dynamically, this means that the arch will be embedded inside the core libraries or it will distributed as a separated shared library.

To configure which plugins you want to compile use the `./configure-plugins` script which accepts the flags --shared and --static to specify them. You can also add it manually inside the `plugins.def.cfg` and then remove the `plugins.cfg` and run `./configure-plugins` again to update the `libr/config.mk` and `libr/config.h`.

You may find some examples of external plugins in [radare2-extras](https://github.com/radareorg/radare2-extras) repository.

### Writing the RArch plugin

Note: As of recent versions, the `r_asm` and `r_anal` plugin interfaces were merged into a unified `r_arch` API. New architectures (CPUs) and full architecture support (disassembly plus analysis/uplift) must be implemented as `r_arch` plugins.

The recommended way to start a new plugin project is to use r2skel to scaffold the basic directory layout and files for an `r_arch` plugin. r2skel will create a ready-to-edit skeleton including build files and a minimal source that links against the `r_arch` API.

General guidance:

* Implement architecture-specific assembly/disassembly, analysis/op detection, ESIL uplift and register profile handlers inside the `r_arch` plugin.
* Prefer packaging your plugin as an external repository (third-party plugin) so it can be maintained independently from the core tree.
* Use pkg-config with the `r_arch` package for compile and link flags, for example:

```Makefile
CFLAGS=-g -fPIC $(shell pkg-config --cflags r_arch)
LDFLAGS=-shared $(shell pkg-config --libs r_arch)
```

For examples and inspiration, check existing architecture plugins in the radare2 repository and the radare2-extras collection. If you need a minimal starting point, run `r2skel` and choose the architecture/plugin template.

**Deprecated**: The old separate `r_asm` (disassembler) and `r_anal` (analysis) plugin examples are obsolete and should not be used for implementing new architectures. Use `r_arch` + r2skel instead.
