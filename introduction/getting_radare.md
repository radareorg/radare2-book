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

