##1.5 Windows compilation

The easy way to compile things for windows is using mingw32. The w32 builds distributed in the radare homepage are generated from a GNU/Linux box using mingw32 and they are tested with wine.

To compile type:

    $ CC=i486-mingw32-gcc ./configure --enable-w32 --without-gui
    $ make
    $ make w32dist
    $ zip -r w32-build.zip w32-build

The 'i486-mingw32-gcc' compiler is the one I have in my box, you will probably need to change this. Mingw32 will generate a native console application for windows.

Another possible way to compile radare on w32 is using cygwin, which I dont really recommend at all because of the problems related to the cygwin librarires makes the program quite hard to be debugged in case of problems.
