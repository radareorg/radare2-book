## Files

Use `r2 -H` to list all the environment variables that matter to know where it will
be looking for files. Those paths depend on the way (and operating system) you have
built r2 for.


	$ r2 -h
	...
	Scripts:
	 system       ${R2_PREFIX}/share/radare2/radare2rc
	 user         ~/.radare2rc ~/.config/radare2/radare2rc (and ~/.config/radare2/radare2rc.d/)
	 file         ${filename}.r2
	...

You can also get the environment vars to reuse from shellscripts or so:

	R2_PREFIX=/usr/local
	MAGICPATH=share/radare2/2.8.0-git/magic
	PREFIX=/usr/local
	INCDIR=/usr/local/include/libr
	LIBDIR=/usr/local/lib
	LIBEXT=dylib
	RCONFIGHOME=/Users/xxx/.config/radare2
	RDATAHOME=/Users/xxx/.local/share/radare2
	RCACHEHOME=/Users/xxx/.cache/radare2
	LIBR_PLUGINS=/usr/local/lib/radare2/last
	USER_PLUGINS=/Users/xxx/.local/share/radare2/plugins
	USER_ZIGNS=/Users/xxx/.local/share/radare2/zigns

## RC Files

RC files are r2 scripts that are loaded at startup time. Those files must be in 3 different places:

### System

radare2 will try to load the /usr/share/radare2/radaer2rc file first

### Your Home

Each user in the system can have its own r2 scripts to run on startup to select color scheme, and other
custom options by having r2 commands in there.

	~/.radare2rc
	~/.config/radare2/radare2rc
	~/.config/radare2/radare2rc.d/

### Target file

If you want to run a script everytime you open a file, just create a file with the same name of the file
but appending `.r2` to it.
