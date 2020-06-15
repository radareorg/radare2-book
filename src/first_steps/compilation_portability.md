## Compilation and Portability

Currently the core of radare2 can be compiled on many systems and architectures, but the main development is done on GNU/Linux with GCC, and on MacOS X with clang. Radare is also known to compile on many different systems and architectures (including TCC and SunStudio).

People often want to use radare as a debugger for reverse engineering. Currently, the debugger layer can be used on Windows, GNU/Linux (Intel x86 and x86_64, MIPS, and ARM), OS X, FreeBSD, NetBSD, and OpenBSD (Intel x86 and x86_64)..

Compared to core, the debugger feature is more restrictive portability-wise. If the debugger has not been ported to your favorite platform, you can disable the debugger layer with the --without-debugger `configure` script option when compiling radare2.

Note that there are I/O plugins that use GDB, WinDbg, or Wine as back-ends, and therefore rely on presence of corresponding third-party tools (in case of remote debugging - just on the target machine).

To build on a system using `acr` and `GNU Make` (e.g. on *BSD systems):
```
$ ./configure --prefix=/usr
$ gmake
$ sudo gmake install
```
There is also a simple script to do this automatically:
```
$ sys/install.sh
```
### Static Build

You can build radare2 statically along with all other tools with the command:
```
$ sys/static.sh
```
### Docker

Radare2 repository ships a [Dockerfile](https://github.com/radareorg/radare2/blob/master/Dockerfile) that you can use with Docker.

This dockerfile is also used by Remnux distribution from SANS, and is available on the docker [registryhub](https://registry.hub.docker.com/u/remnux/radare2/).

## Cleaning Up Old Radare2 Installations
```
./configure --prefix=/old/r2/prefix/installation
make purge
```

