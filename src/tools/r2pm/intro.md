## Radare2 package manager

Radare2 has its own package manager for managing external plugins (mainly from
[radare2-extras](https://github.com/radareorg/radare2-extras)). As most of the package managers it allows you to install, remove or update packages (plugins). There is one restriction though - currently it supports only the compilation from sources, so providing the toolchan, necessary build tools and dependencies for each plugin is the user responsibility. It might change in the future.

```
$ r2pm -h
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
r2pm database initialized. Use 'r2pm update' to update later today
```

As you noticed, the packages database located at [radare2-pm](https://github.com/radareorg/radare2-pm)
repository. At any point of the time we can update the database using `r2pm update`:

```
r2pm update
HEAD is now at 7522928 Fix syntax
Updating 7522928..1c139e0
Fast-forward
 db/ldid2 | 18 ++++++++++++++++++
 1 file changed, 18 insertions(+)
 create mode 100644 db/ldid2
Updating /home/user/.local/share/radare2/r2pm/db ...
Already up to date.
```

There are many commands available now:
```
r2pm -h
Usage: r2pm [init|update|cmd] [...]
Commands:
 -I,info                     show information about a package
 -i,install <pkgname>        install package in your home (pkgname=all)
 -gi,global-install <pkg>    install package system-wide
 -gu,global-uninstall <pkg>  uninstall pkg from systemdir
 -u,uninstall <pkgname>      r2pm -u baleful (-uu to force)
 -l,list                     list installed pkgs
 -r,run [cmd ...args]        run shell command with R2PM_BINDIR in PATH
 -s,search [<keyword>]       search in database
 -t,test FX,XX,BR BID        check in Travis regressions
 -v,version                  show version
 -h,help                     show this message
 -H variable                 show value of given variable
 -c,clean ([git/dir])        clear source cache (GITDIR)
 -ci (pkgname)               clean install of given package
 -cp                         clean the user's home plugin directory
 -d,doc [pkgname]            show documentation for given package
 -w <pkgname>                what/where is installed
 init | update ..            initialize/update database
 cd [git/dir]                cd into given git (see 'r2pm ls')
 ls                          ls all cloned git repos in GITDIR
 suicide                     self remove all (home + system) installations of r2
 cache                       cache contents of r2 -H to make r2pm r2-independent
Environment:
 SUDO=sudo                    use this tool as sudo
 R2PM_PLUGDIR=~/.local/share/radare2/plugins   # default value, home-install for plugins
 R2PM_BINDIR=~/.local/share/radare2/prefix/bin # bindir in r2's PATH
 R2PM_PLUGDIR=/usr/lib/radare2/last/           # for system-wide plugin installs
 R2PM_DBDIR=~/.local/share/radare2/r2pm/db     # location of the db
 R2PM_GITDIR=~/.local/share/radare2/r2pm/git   # cloned git repositories
```

The usual scenario of using it is to install new plugins, for example `lang-python3` (which is used
for writing r2 plugins in Python):

```
$ r2pm -i lang-python3
...
mkdir -p ~/.config/radare2/plugins
cp -f lang_python3.so ~/.config/radare2/plugins
...
```

Note that if we used `-i` switch it installs the plugin in the `$HOME` directory, in case of `-gi`
it will install the plugin systemwide (requires `sudo`).

After this we will be able to see the plugin in the list of installed:

```
$ r2pm -l
lang-python3
```

To uninstall the plugin just simply run

```
$ r2pm -u lang-python3
```

