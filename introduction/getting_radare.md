## Getting radare2

You can get radare from the website, [http://radare.org/](http://radare.org/), or the GitHub repository, [https://github.com/radare/radare2](https://github.com/radare/radare2).

Binary packages are available for a number of operating systems (Ubuntu, Maemo, Gentoo, Windows, iPhone, and so on). I highly encourage you to get the source and compile it yourself, to better understand the dependencies and make the examples more accessible.

A new stable release is typically published every month. Nightly tarballs are sometimes available [http://bin.rada.re/](http://bin.rada.re/).

The radare development repository is often more stable than the 'stable' releases. To obtain the latest version:

    $ git clone https://github.com/radare/radare2.git

This will probably take a while, so take a coffee break and continue reading this paper.

To update your local copy of the repository, use git pull in the root of the radare2 directory:

    $ git pull

If you have local modifications of the source, you can revert them with:

    $ git reset --hard HEAD

Or send me a patch:

    $ git diff > radare-foo.patch

The most common way to get r2 updated and installed system wide is by using:

    $ sys/install.sh

Helper scripts
--------------

Take a look at the sys/* scripts, those are used to automatize stuff related to syncing, building and installing r2 and its bindings.

The most important one is sys/install.sh. It will pull, clean, build and symstall r2 system wide.

Symstalling is the process of installing all the programs, libraries, documentation and data files using symlinks instead of copying the files.

By default it will be installed in /usr, but you can define a new prefix as argument.

This is useful for developers, because it permits them to just run 'make' and try the changes without having to run make install again.

Cleaning up
-----------

Cleaning up the source tree is important to avoid problems like linking to old objects or not updating objects after an abi change.

The following commands may help you to get your git clone up to date:

    $ git clean -xdf
    $ git reset --hard @~10
    $ git pull

If you want to remove previous installations from your system you must run the following commands:

    $ ./configure --prefix=/usr/local
    $ make purge