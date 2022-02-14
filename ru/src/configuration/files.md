## Files

Use `r2 -H` to list all the environment variables that matter to know where it will
be looking for files. Those paths depend on the way (and operating system) you have
built r2 for.

```
R2_PREFIX=/usr
MAGICPATH=/usr/share/radare2/2.8.0-git/magic
PREFIX=/usr
INCDIR=/usr/include/libr
LIBDIR=/usr/lib64
LIBEXT=so
RCONFIGHOME=/home/user/.config/radare2
RDATAHOME=/home/user/.local/share/radare2
RCACHEHOME=/home/user/.cache/radare2
LIBR_PLUGINS=/usr/lib/radare2/2.8.0-git
USER_PLUGINS=/home/user/.local/share/radare2/plugins
USER_ZIGNS=/home/user/.local/share/radare2/zigns
```

## RC Files

RC files are r2 scripts that are loaded at startup time. Those files must be in 3 different places:

### System

radare2 will first try to load /usr/share/radare2/radare2rc

### Your Home

Each user in the system can have its own r2 scripts to run on startup to select the color scheme, and other custom options by having r2 commands in there.

* ~/.radare2rc
* ~/.config/radare2/radare2rc
* ~/.config/radare2/radare2rc.d/

### Target file

If you want to run a script everytime you open a file, just create a file with the same name of the file
but appending `.r2` to it.