## Compilation and Portability

Currently the core of radare2 can be compiled on many systems and architectures, but the main development is done on GNU/Linux with GCC, and on MacOS X with clang. Radare is also known to compile on many different systems and architectures (including TCC and SunStudio).

People often want to use radare as a debugger for reverse engineering. Currently, the debugger layer can be used on Windows, GNU/Linux (Intel x86 and x86_64, MIPS, and ARM), OS X, FreeBSD, NetBSD, and OpenBSD (Intel x86 and x86_64)..

Compared to core, the debugger feature is more restrictive portability-wise. If the debugger has not been ported to your favorite platform, you can disable the debugger layer with the --without-debugger `configure` script option when compiling radare2.

Note that there are I/O plugins that use GDB, WinDbg, or Wine as back-ends, and therefore rely on presence of corresponding third-party tools (in case of remote debugging - just on the target machine).

To build on a system using `acr` and `GNU Make` (e.g. on *BSD systems*):

```sh
$ ./configure --prefix=/usr
$ gmake
$ sudo gmake install
```

There is also a simple script to do this automatically:

```sh
$ sys/install.sh
```

### Static Build

You can build radare2 statically along with all other tools with the command:

```sh
$ sys/static.sh
```

### Meson build

You can use meson/ninja to build (or muon/samu):

```sh
$ meson b && ninja -C b
```

There's a helper script in sys/ to make the meson experience a little bit simpler:

```sh
$ sys/meson.py --prefix=/usr --shared --install
```

If you want to build locally:

```sh
$ sys/meson.py --prefix=/home/$USER/r2meson --local --shared --install
```

### Docker

Radare2 repository ships a [Dockerfile](https://github.com/radareorg/radare2/blob/master/dist/docker/Dockerfile) that you can use with Docker.

This dockerfile also used by Remnux distribution from SANS, and is available on the docker [registryhub](https://registry.hub.docker.com/u/remnux/radare2/).

Checkout the Makefile in this directory to understand how to use it without too much hussle

## Cleaning Up Old Radare2 Installations

```sh
./configure --prefix=/old/r2/prefix/installation
make purge
```
