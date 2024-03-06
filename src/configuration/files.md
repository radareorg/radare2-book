## Files

Use `r2 -H` to list all the environment variables that matter to know where it will
be looking for files. Those paths depend on the way (and operating system) you have
built r2 for.

```sh
$ r2 -H
R2_VERSION=5.8.9
R2_PREFIX=/usr/local
R2_MAGICPATH=/usr/local/share/radare2/5.8.9/magic
R2_INCDIR=/usr/local/include/libr
R2_BINDIR=/usr/local/bin
R2_LIBDIR=/usr/local/lib
R2_LIBEXT=dylib
R2_RCFILE=/Users/pancake/.radare2rc
R2_RDATAHOME=/Users/pancake/.local/share/radare2
R2_HISTORY=/Users/pancake/.cache/radare2/history
R2_CONFIG_HOME=/Users/pancake/.config//radare2
R2_CACHE_HOME=/Users/pancake/.cache/radare2
R2_LIBR_PLUGINS=/usr/local/lib/radare2/5.8.9
R2_USER_PLUGINS=/Users/pancake/.local/share/radare2/plugins
R2_ZIGNS_HOME=/Users/pancake/.local/share//radare2/zigns
```

## RC Files

RC files are r2 scripts that are loaded at startup time. Those files must be in 3 different places:

### System

radare2 will first try to load `/usr/share/radare2/radare2rc`

### Your Home

Each user in the system can have its own r2 scripts to run on startup to select the color scheme, and other custom options by having r2 commands in there.

* ~/.radare2rc
* ~/.config/radare2/radare2rc
* ~/.config/radare2/radare2rc.d/

The path to this file can be also specified with the `R2_RCFILE` environment variable.

### Target file

If you want to run a script everytime you open a file, just create a file with the same name of the file but appending `.r2` to it.
