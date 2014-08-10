## Windows compilation

The easy way to compile things for Windows is using MinGW32. The w32 builds distributed in the radare homepage are generated from a GNU/Linux box using MinGW32 and they are tested with Wine.

To compile type:

    $ CC=i486-mingw32-gcc ./configure --enable-w32 --without-gui
    $ make
    $ make w32dist
    $ zip -r w32-build.zip w32-build

The 'i486-mingw32-gcc' compiler is the one I have in my box, you will probably need to change this. MinGW32 will generate a native console application for Windows.

Another possible way to compile radare2 on w32 is using Cygwin, which I dont really recommend at all because of the problems related to the Cygwin librarires makes the program quite hard to be debugged in case of problems.

There's a script that automates the process of autodetecting the crosscompiler toolchain and build a zip file containing the r2 programs and libraries that should be deployed in a windows box (or wine).

    $ sys/mingw32.sh
