## Compilation and portability

Currently the core of radare2 can be compiled on many systems and architectures but the main development is done on GNU/Linux and GCC and OSX with clang. But it is also known to compile on many different systems and architectures (including TCC and SunStudio).

People often want to use radare as a debugger for reverse engineering. Currently, the debugger layer can be used on Windows, GNU/Linux (Intel x86 and x86-64, MIPS, and ARM), FreeBSD, NetBSD, and OpenBSD (Intel x86 and x86-64). There are plans for supporting Solaris and Mac OS X.

The debugger feature is more restrictive portability wise. If the debugger has not been ported to your favorite platform, please notify me or disable the debugger layer with the --without-debugger configure script option when compiling radare2.

Note that there are some I/O plug-ins to use GDB, GDB Remote, or Wine as back-ends.

To build on a system using ACR/GMAKE (e.g. on *BSD systems):

    $ ./configure --prefix=/usr
    $ gmake
    $ sudo gmake install

There is also a simple script to do this automatically:

    $ sys/install.sh

## Static build

You can build statically radare2 and all the tools with the command:

    $ sys/static.sh

## Docker

Radare2 repository ships a [Dockerfile](https://github.com/radare/radare2/blob/master/doc/Dockerfile) that you can use with Docker.

This dockerfile is also use by Remnux distribution from SANS and is available on the docker [registryhub](https://registry.hub.docker.com/u/remnux/radare2/).

## Cleaning up old r2 installations

    ./configure --prefix=/old/r2/prefix/installation
    make purge
