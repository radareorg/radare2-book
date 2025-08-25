## Analysis plugins (deprecated)

This page previously showed how to write separate r_anal plugins for opcode analysis and ESIL uplift. That separation has been removed: r_anal and r_asm were merged into the r_arch plugin interface.

For new architecture work, please:

* Implement analysis, ESIL uplift and opcode handling inside an r_arch plugin.
* Use `r2skel` to scaffold a new plugin project and follow the r_arch template.
* Migrate any existing r_anal plugin logic into the corresponding r_arch plugin.

The old examples and Makefile snippets referencing `r_anal` or `r_asm` are obsolete and retained here for historical reasons only. Prefer the r_arch approach for all new development.
