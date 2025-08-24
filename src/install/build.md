## Building From Source

One of radare2's core development principles is portability; consequently, it can be compiled on many systems and architectures. To achieve this and maximize flexibility, we maintain two build systems: **GNU Make** and **Meson**.

Most contributors work on GNU/Linux with GCC or macOS with Clang, so those platforms are the most thoroughly tested and best supported. However, builds are also possible with TinyCC, Emscripten, Microsoft Visual Studio, Sun Studio, etc.

Some features can be disabled at compile time to get your radare2 builds:

* Choose the plugins
* Choose third-party replacements
* Disable Debugger features
* Fork/system code execution
* Threading support

The debugger can be disabled at compile time — useful for WebAssembly builds, sandboxed environments, or ports to platforms where the debugger is not supported. Use the `--without-debugger` configure flag to disable it.

Currently, the debugger layer is available on:

- Windows (intel or arm architectures)
- GNU/Linux — Intel x86/x86-64, S390x, PowerPC, RISCV, MIPS, and ARM
- Apple - macOS, iOS, appleTV, AppleWatch, ..
- FreeBSD, NetBSD, and OpenBSD — Intel x86/x86-64

Some I/O plugins are OS-specific and may require third-party tools such as GDB, WinDBG, or Wine.

### Build Systems

The project supports and maintains **GNU Make** and **Meson** scripts to build from source. This allows enough flexibility to cover all the needs for portability and developer friendliness to make it comfortable for everyone to contribute and run the project.

Both build systems have advantages and disadvantages. On Unix-like systems, make is often a good choice; on Windows, or when integrating radare2 with third-party projects or paths that contain spaces, meson is usually the better option.

When we talk about meson builds, we mean meson used together with ninja. That said, you don't need to rely on the Python/C++ projects — the plain C reimplementations (Muon and Samu) are also supported and tested in the CI.

### Building with GNU Make

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

### Cleaning Up

Cleaning up the source tree is important to avoid problems like linking to old objects files or not updating objects after an ABI change.

The following commands may help you to get your git clone up to date:

```console
$ git clean -xdf
$ git reset --hard origin/master
$ git pull
```

If you want to remove previous installations from your system, you must run the following commands:

```console
$ ./configure --prefix=/usr/local
$ make purge
```

### Home builds

If you want to build radare2 in your home directory without touching the system installation, use this script:

```console
$ sys/user.sh
```

If you'd rather just test a build without installing it, run `env.sh` — it sets the `LD_LIBRARY_PATH` and other environment variables so the build runs in place. Alternatively, you can install into /tmp using symlinks, or create static builds that bundle all plugins and libraries into a single binary.

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

### Building with meson + ninja

Using clang and ld.gold makes the build faster:

```bash
CC=clang LDFLAGS=-fuse-ld=gold meson . release --buildtype=release --prefix ~/.local/stow/radare2/release
ninja -C release
# ninja -C release install
```

### Docker build

Radare2 repository ships a [Dockerfile](https://github.com/radareorg/radare2/blob/master/dist/docker/Dockerfile) that you can use with Docker.

You can read more regarding the this build process as well as how to run it in the [Docker containers section](docker.md#git-version).

See `dist/docker` for more details.
