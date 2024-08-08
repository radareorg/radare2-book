## Packaging your plugins

As explained in more detail in the [package manager](../tools/r2pm/intro.md) chapter, it is recommended to use our tooling to make your plugin available for everyone.

All the current packages are located in the [radare2-pm](https://github.com/radareorg/radare2-pm) repository, check some of the already existing ones for inspiration as you will see how easy it's format is:

```sh
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

Add your package in the `/db` directory of radare2-pm repository and send a pull request when it's ready to be shared.

```console
$ r2pm -H R2PM_DBDIR
/Users/pancake/.local/share/radare2/r2pm/git/radare2-pm/db
$
```
