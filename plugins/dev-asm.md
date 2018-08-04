## Implementing a new disassembly plugin

Pushing a new architecture into the main branch of r2 requires to modify several files in order to make it fit into the way the rest of plugins are built.

List of affected files:

* `plugins.def.cfg` : add the `asm.mycpu` plugin name string in there
* `libr/asm/p/mycpu.mk` : build instructions
* `libr/asm/p/asm_mycpu.c` : implementation
* `libr/include/r_asm.h` : add the struct definition in there

Check out how the NIOS II CPU disassembly plugin was implemented by reading those commits:

Implement RAsm plugin:
https://github.com/radare/radare2/commit/933dc0ef6ddfe44c88bbb261165bf8f8b531476b

Implement RAnal plugin:
https://github.com/radare/radare2/commit/ad430f0d52fbe933e0830c49ee607e9b0e4ac8f2


