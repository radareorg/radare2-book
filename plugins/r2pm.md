# Creating an r2pm package of the plugin

As you remember radare2 has its own [packaging manager](../tools/r2pm/intro.md) and we can easily
add newly written plugin for everyone to access.

All packages are located in [radare2-pm](https://github.com/radareorg/radare2-pm) repository, and have
very simple text format.

```
R2PM_BEGIN

R2PM_GIT "https://github.com/user/mycpu"
R2PM_DESC "[r2-arch] MYCPU disassembler and analyzer plugins"

R2PM_INSTALL() {
	${MAKE} clean
	${MAKE} all || exit 1
	${MAKE} install R2PM_PLUGDIR="${R2PM_PLUGDIR}"
}

R2PM_UNINSTALL() {
	rm -f "${R2PM_PLUGDIR}/asm_mycpu."*
	rm -f "${R2PM_PLUGDIR}/anal_mycpu."*
}

R2PM_END
```

Then add it in the `/db` directory of radare2-pm repository and send a pull request to the mainline.


