##1.3 Getting radare2

You can get radare from the website http://radare.org/ or Github repo https://github.com/radare/radare2

There are binary packages for multiple operating systems and GNU/Linux distributions (ubuntu, maemo, gentoo, windows, iphone, etc..) But I hardly encourage you to get the sources and compile them yourself to better understand the dependencies and have the source code and examples more accessible.

I try to publish a new stable release every month and sometimes publish nightly tarballs.

But as always the best way to use a software is to go upstream and pull the development repository which in radare is commonly more stable than the 'stable' releases O:)

To do this you will need Git and type:

    $ git clone https://github.com/radare/radare2.git

This will probably take a while, so take a coffee and continue reading this paper.

To update your local copy of the repository you will have to type this command in the root of the recently created 'radare' directory.

    $ git pull

If you have local modifications of the source, you can revert them with:

    $ git reset --hard HEAD

Or just feed me with a patch

    $ git diff > radare-foo.patch

