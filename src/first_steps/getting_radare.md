## Downloading radare2

You can get radare from the website, [http://radare.org](http://radare.org),
or the GitHub repository: [https://github.com/radareorg/radare2](https://github.com/radareorg/radare2)


Binary packages are available for a number of operating systems (Ubuntu, Maemo, Gentoo, Windows, iPhone, and so on). But you are highly encouraged to get the source and compile it yourself to better understand the dependencies, to make examples more accessible and, of course, to have the most recent version.

A new stable release is typically published every month. Nightly tarballs are sometimes available at [http://bin.rada.re/](http://bin.rada.re/).

The radare development repository is often more stable than the 'stable' releases. To obtain the latest version:
```
$ git clone https://github.com/radareorg/radare2.git
```
This will probably take a while, so take a coffee break and continue reading this book.

To update your local copy of the repository, use `git pull` anywhere in the radare2 source code tree:
```
$ git pull
```
If you have local modifications of the source, you can revert them (and lose them!) with:
```
$ git reset --hard HEAD
```
Or send us a patch:
```
$ git diff > radare-foo.patch
```
The most common way to get r2 updated and installed system wide is by using:
```
$ sys/install.sh
```
### Building with meson + ninja

There is also a work-in-progress support for Meson.

Using clang and ld.gold makes the build faster:
```bash
CC=clang LDFLAGS=-fuse-ld=gold meson . release --buildtype=release --prefix ~/.local/stow/radare2/release
ninja -C release
# ninja -C release install
```

### Helper Scripts

Take a look at the scripts in `sys/`, they are used to automate stuff related to syncing, building and installing r2 and its bindings.

The most important one is `sys/install.sh`. It will pull, clean, build and symstall r2 system wide.

Symstalling is the process of installing all the programs, libraries, documentation and data files using symlinks instead of copying the files.

By default it will be installed in `/usr/local`, but you can specify a different prefix using the argument `--prefix`.

This is useful for developers, because it permits them to just run 'make' and try changes without having to run make install again.

### Cleaning Up

Cleaning up the source tree is important to avoid problems like linking to old objects files or not updating objects after an ABI change.

The following commands may help you to get your git clone up to date:
```
$ git clean -xdf
$ git reset --hard @~10
$ git pull
```
If you want to remove previous installations from your system, you must run the following commands:
```
$ ./configure --prefix=/usr/local
$ make purge
```
