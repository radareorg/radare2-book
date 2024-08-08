## r2pm

Radare2 has its own **package manager** for managing external plugins or tools that have a close relationship to radare2.

* Most packages are tested and maintained for UNIX systems
* Some of them support Windows and even wasm (r2js)
* Installs by default in your home, use `-g` to do it system-wide
* Most of them are building it from source, but others have support for binary builds

The [radare2-extras](https://github.com/radareorg/radare2-extras) repository contains a lot of third-party packages that aim to be updated and maintained (but also less used than the ones shipped in the main repository), so it's a great place to check before writing your own plugin as maybe this thing was done already by someone else.

### Package Database

```
$ r2pm -U
$R2PM_DBDIR: No such file or directory.
Run 'r2pm init' to initialize the package repository
$ r2pm init
git clone https://github.com/radareorg/radare2-pm
Cloning into 'radare2-pm'...
remote: Counting objects: 147, done.
remote: Compressing objects: 100% (139/139), done.
remote: Total 147 (delta 26), reused 57 (delta 7), pack-reused 0
Receiving objects: 100% (147/147), 42.68 KiB | 48.00 KiB/s, done.
Resolving deltas: 100% (26/26), done.
/home/user/.local/share/radare2/r2pm/git/radare2-pm
r2pm database initialized. Use 'r2pm -U' to update later today
```

The packages database is pulled from the [radare2-pm](https://github.com/radareorg/radare2-pm) repository. At any point of the time we can update the database using `r2pm -U`:

```
r2pm -U
HEAD is now at 7522928 Fix syntax
Updating 7522928..1c139e0
Fast-forward
 db/ldid2 | 18 ++++++++++++++++++
 1 file changed, 18 insertions(+)
 create mode 100644 db/ldid2
Updating /home/user/.local/share/radare2/r2pm/db ...
Already up to date.
```

There are many commands available to let you install or uninstall anything easily:

```
$ r2pm -h
Usage: r2pm [-flags] [pkgs...]
Commands:
 -a [repository]   add or -delete external repository
 -c ([git/dir])    clear source cache (R2PM_GITDIR)
 -ci <pkgname>     clean + install
 -cp               clean the user's home plugin directory
 -d,doc [pkgname]  show documentation and source for given package
 -e [pkgname]      edit using $EDITOR the given package script
 -f                force operation (Use in combination of -U, -i, -u, ..)
 -gi <pkg>         global install (system-wide)
 -h                display this help message
 -H variable       show the value of given internal environment variable (See -HH)
 -HH               show all the internal environment variable values
 -i <pkgname>      install/update package and its dependencies (see -c, -g)
 -I                information about repository and installed packages
 -l                list installed packages
 -q                be quiet
 -r [cmd ...args]  run shell command with R2PM_BINDIR in PATH
 -s [<keyword>]    search available packages in database matching a string
 -t [YYYY-MM-DD]   force a moment in time to pull the code from the git packages
 -u <pkgname>      r2pm -u baleful (See -f to force uninstall)
 -uci <pkgname>    uninstall + clean + install
 -ui <pkgname>     uninstall + install
 -U                initialize/update database and upgrade all outdated packages
 -v                show version
```

### Sample Session

For example `lang-python3` (which is used for writing r2 plugins in Python):

```
$ r2pm -i lang-python3
...
mkdir -p ~/.config/radare2/plugins
cp -f lang_python3.so ~/.config/radare2/plugins
...
```

Note that if we used `-i` switch it installs the plugin in the `$HOME` directory, in case of `-gi` it will install the plugin systemwide (requires `sudo`).

After this we will be able to see the plugin in the list of installed:

```bash
$ r2pm -l
lang-python3
```

To uninstall the plugin just simply run

```bash
$ r2pm -u lang-python3
```
