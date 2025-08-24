## Downloading

You can get radare from the GitHub repository: [https://github.com/radareorg/radare2](https://github.com/radareorg/radare2)

Binary packages are available for a number of operating systems (Ubuntu, Maemo, Gentoo, Windows, iPhone, and so on). But you are highly encouraged to get the source and compile it yourself to better understand the dependencies, to make examples more accessible and, of course, to have the most recent version.

Downloadable versions of the latest releases of Radare2 are available in [https://github.com/radareorg/radare2/releases](GitHub) itself.

For distributions we aim to recommend the use of release source tarballs to build the program. But users may probably want to use the last code from **git** that's why we always mention "use r2 from git", because we don't aim to fix bugs in releases, always test your bug or problems in the last development branch `master`.

The radare development repository is often more stable than the 'stable' releases. To obtain the latest version:

```console
$ git clone https://github.com/radareorg/radare2.git
```

This will probably take a while, so take a coffee break and continue reading this book.

To update your local copy of the repository, use `git pull` anywhere in the radare2 source code tree:

```console
$ git pull
```

The most common way to get r2 updated and installed system wide is by using, but we may learn more about building

```console
$ sys/install.sh
```

### r2env

r2env is a lightweight tool version managers: either clone the repo and add its `bin` to your PATH and enable shell integration, or install via pip. The simplest option:

```bash
pip install r2env
```

This makes the `r2env` command available and sets up shims for `r2` / `radare2` so the active version is used automatically.

Replace `<version-or-ref>` with a tag, branch, or other reference you want to install.

```bash
r2env install <version-or-ref>
```

To remove a build later, use `r2env uninstall` (see the repo README for the exact syntax and any extra flags). If anything behaves unexpectedly, consult the r2env GitHub README for up-to-date flags, usage details, and platform-specific dependencies.

### Helper Scripts

Take a look at the scripts in `sys/`, they are used to automate stuff related to syncing, building and installing r2 and its bindings.

The most important one is `sys/install.sh`. It will pull, clean, build and symstall r2 system wide.

Symstalling is the process of installing all the programs, libraries, documentation and data files using symlinks instead of copying the files.

By default it will be installed in `/usr/local`, but you can specify a different prefix using the argument `--prefix`.

This is useful for developers, because it permits them to just run 'make' and try changes without having to run make install again.
