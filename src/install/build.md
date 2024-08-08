## Portability in Mind

One of the main development principles of radare2 is its portability, therefor radare2 can be compiled on many systems and architectures. In order to achieve that and to extend flexibility we are maintaining two build systems: GNU Make and Meson.

Most contributors use GNU/Linux with GCC or macOS with Clang, so those would be the better supported platforms, or at least the most tested. But it is also possible to build with TinyCC, Emscripten, Microsoft Visual Studio, SunStudio, ...)

The debugger feature can be opt-out at compile time, this is because sometimes you are ont interested in having such feature in a specific build (web assembly, specific sandbox usage, etc) or maybe it is because you are porting radare2 on a platform that the debugger is not yet supported. Use the `--without-debugger` configure flag to do that.

Currently, the debugger layer can be used on Windows, GNU/Linux (Intel x86 and x86_64, MIPS, and ARM), OS X, FreeBSD, NetBSD, and OpenBSD (Intel x86 and x86_64)..

Note that there are I/O plugins that use GDB, WinDbg, or Wine as back-ends, and therefore rely on presence of corresponding third-party tools (in case of remote debugging - just on the target machine).

To build on a system using `acr` and `GNU Make` (e.g. on _BSD systems_):

```console
$ ./configure --prefix=/usr
$ gmake
$ sudo gmake install
```

There is also a simple script to do this automatically:

```console
$ sys/install.sh
```

### Home builds

To build and run radare2 in your home just run the sys/user.sh script.

```console
$ sys/user.sh
```

### Static Build

You can build radare2 statically along with all other tools with the command:

```console
$ sys/static.sh
```

### Meson build

You can use meson/ninja to build (or muon/samu):

```console
$ meson b && ninja -C b
```

There's a helper script in sys/ to make the meson experience a little bit simpler:

```console
$ sys/meson.py --prefix=/usr --shared --install
```

If you want to build locally:

```console
$ sys/meson.py --prefix=/home/$USER/r2meson --local --shared --install
```

### Docker build

Radare2 repository ships a [Dockerfile](https://github.com/radareorg/radare2/blob/master/dist/docker/Dockerfile) that you can use with Docker.

You can read more regarding the this build process as well as how to run it in the [Docker containers section](docker.md#git-version).
