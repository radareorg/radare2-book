## Getting radare2

You can get radare from the website http://radare.org/ or Github repo https://github.com/radare/radare2

There are binary packages for multiple operating systems and GNU/Linux distributions (Ubuntu, Maemo, Gentoo, Windows, iPhone, etc..) But I hardly encourage you to get the sources and compile them yourself to better understand the dependencies and have the source code and examples more accessible.

I try to publish a new stable release every month and sometimes publish nightly tarballs.

But as always the best way to use a software is to go upstream and pull the development repository which in the case of radare is commonly more stable than the 'stable' releases O:)

To do this you will need Git and type:

    $ git clone https://github.com/radare/radare2.git

This will probably take a while, so take a coffee break and continue reading this paper.

To update your local copy of the repository you will have to type the following command in the root of the recently created 'radare2' directory.

    $ git pull

If you have local modifications of the source, you can revert them with:

    $ git reset --hard HEAD

Or just feed me with a patch

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