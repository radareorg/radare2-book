## Compilation and portability

The main radare2 development is performed with GNU/Linux and GCC; however, the core of radare2 can be compiled on many systems and architectures (including TCC and Sun Studio).

People often want to use radare as a debugger for reverse engineering. Currently, the debugger layer can be used on Windows, GNU/Linux (Intel x86 and x86-64, MIPS, and ARM), FreeBSD, NetBSD, and OpenBSD (Intel x86 and x86-64). There are plans for supporting Solaris and Mac OS X.

The debugger feature is more restrictive portability wise. If the debugger has not been ported to your favorite platform, please notify me or disable the debugger layer with the --without-debugger configure script option when compiling radare2.

Note that there are some I/O plug-ins to use GDB, GDB Remote, or Wine as back-ends.

To build on a system using ACR/GMAKE:

    $ ./configure --prefix=/usr
    $ gmake
    $ sudo gmake install
    
There is also a simple script to do this automatically:

    $ sys/install.sh
