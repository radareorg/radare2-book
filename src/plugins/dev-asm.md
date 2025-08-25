## Arch Plugins (deprecated pages)

This page contained historical examples showing how to implement separate r_asm (disassembly) plugins. Since the r_asm and r_anal interfaces were merged into the unified r_arch API, these separate examples are obsolete.

For implementing new architectures (disassembly, analysis and ESIL uplift), please:

* Use the r_arch plugin interface (r_asm and r_anal are merged into r_arch).
* Scaffold a project with `r2skel` and choose the architecture/plugin template to get a working skeleton.
* Check existing r_arch plugins in the main radare2 tree or radare2-extras for reference implementations.

If you need to migrate an old asm/anal plugin, port its logic into an r_arch plugin and use the new r_arch pkg-config flags for compilation.
