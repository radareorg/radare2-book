## Windows compilation

We recommend using MinGW32 to compile radare for Windows. The 32-bit Windows builds distributed on the radare homepage are generated from a GNU/Linux system using MinGW32. They are tested with Wine.

The following is an example of compiling with MinGW32 (you need to have installed **zip** for Windows):

    $ ./configure
    $ make
    $ make w32dist
    $ zip -r w32-build.zip w32-build

This generates a native, 32-bit console application for Windows.

Cygwin is another possibility; however, issues relating to Cygwin libraries can make debugging difficult in case of problems.

Please, be sure to build radare2 from that environment you're going to use r2 in. If you're going to use r2 in MinGW32 shell or cmd.exe - you need to build r2 in the MinGW32 environment. And if you're going to use r2 in Cygwin - you need to build from the Cygwin shell as well. Since Cygwin is more UNIX compatible radare2 supports here more colors and unicode symbols.

There's a script that automates the process of autodetecting the crosscompiler toolchain and build a zip file containing the r2 programs and libraries that should be deployed in a windows box (or wine).

    $ sys/mingw32.sh

## Bindings

To build radare2 bindings you will need to install [Vala (valac) for Windows](https://wiki.gnome.org/Projects/Vala/ValaOnWindows)

Then download [valabind](https://github.com/radare/valabind) and build it:

    $ git clone https://github.com/radare/valabind.git valabind
    $ cd valabind
    $ make
    $ make install

After you installed valabind, you can build radare2-bindings, for example for Python and Perl:

    $ git clone https://github.com/radare/radare2-bindings.git radare2-bindings
    $ cd radare2-bindings
    $ ./configure --enable=python,perl
    $ make
    $ make install

