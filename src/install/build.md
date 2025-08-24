## Building From Source

One of radare2's core development principles is portability; consequently, it can be compiled on many systems and architectures. To achieve this and maximize flexibility, we maintain two build systems: **GNU Make** (classic autotools style ./configure + make) and **Meson** (modern ninja-based builds).

Most contributors work on GNU/Linux with GCC or macOS with Clang, so those platforms are the most thoroughly tested and best supported. However, builds are also possible with TinyCC, Emscripten, Microsoft Visual Studio, Sun Studio, etc.

Some features can be disabled at compile time to tailor the binary for your environment:

* Pick which plugins are built (static vs shared)
* Choose system or bundled third-party libraries (capstone, openssl, zlib, etc.)
* Disable the native debugger
* Disable fork/exec support
* Disable threading support

The debugger can be disabled at configure time — useful for WebAssembly builds, sandboxed environments, or ports to platforms where the debugger is not supported. Use the `--disable-debugger` configure flag (or set `-Ddebugger=false` with meson) to disable it.

Currently, the debugger layer is available on many platforms (Windows, Linux, macOS, BSDs, etc.). Some I/O plugins are OS-specific and may require third-party tools such as GDB, WinDBG, or Wine.

### Build systems overview

* GNU Make (./configure + make): traditional, flexible, well suited for Unix-like systems and advanced customization via environment variables.
* Meson + Ninja: faster incremental builds, better Windows support, declarative options. We expose most important knobs as meson options (see below).

Choose the system you prefer; both can be used to produce equivalent results.

### Selecting plugins: ./configure-plugins (Make-based workflow)

radare2 ships with a small helper script named `configure-plugins` (found in the repository root). Its purpose is to generate `plugins.cfg` and the generated build headers/make fragments that control which plugins are built as static (compiled into the libraries) or shared (built as separate .so/.dylib files).

Key points:

* Default plugin list is taken from `dist/plugins-cfg/plugins.def.cfg`. When `plugins.cfg` is missing, `configure-plugins` copies the default file there.
* `plugins.cfg` contains two shell variables: `STATIC` and `SHARED`, which list plugin names in the form `lib.plugin` (for example `io.debug`, `asm.x86`).
* `./configure-plugins` reads `plugins.cfg` and generates:
  * `libr/config.h` (macros describing which plugins are static)
  * `libr/asm/d/config.inc` (gperf registrations for static asm plugins)
  * `libr/config.mk` (Makefile fragments used by the build)

Usage and useful flags (from the script help):

* `./configure-plugins --shared name1 name2` — mark the named plugins as shared (remove from STATIC and add to SHARED).
* `./configure-plugins --static name1 name2` — mark the named plugins as static (compiled into the relevant library).
* `./configure-plugins --list` — print the current static / shared lists.
* `./configure-plugins --rm-static <dir>` — when provided a directory (e.g. an installation lib dir), remove shared plugin files that correspond to plugins marked as static (helper when switching a build to static plugins to avoid stale shared objects).
* `./configure-plugins --without-gpl` — use `plugins.nogpl.cfg` defaults instead of the distributed one (useful to avoid GPL-only plugins).
* `./configure-plugins -n` — do nothing (don't generate files, useful to inspect what would happen).

Notes:

* After changing plugins with `configure-plugins`, re-run `./configure` and rebuild (or re-run the Make build) so the generated headers and make fragments are picked up.
* For Meson builds, plugin selection works differently (see the Meson options section). Meson has an option `-Dplugins='comma,separated,list'` to build only a specific set of plugins.

If you want to customize further or need help choosing specific plugin names, check `dist/plugins-cfg/plugins.def.cfg` for the canonical default list, and `src/plugins` documentation (see `src/plugins/dev.md`) for guidance on plugin naming and structure.

### The most relevant ./configure options (Make)

When using `./configure` there are many options; these are the most commonly useful ones and how to use them.

* Installation directories
  * `--prefix=/path` — set installation prefix (default `/usr/local`).
  * `--exec-prefix=EPREFIX` — install arch-dependent files in EPREFIX.

* Tuning the install paths
  * `--bindir`, `--libdir`, `--datadir`, `--sysconfdir`, `--mandir`, etc. — set specific target directories.

* Cross-build settings
  * `--host=HOST` and `--build=BUILD` — useful when cross-compiling.

* Feature toggles (picks the most important ones)
  * `--disable-debugger` — build without native debugger support.
  * `--disable-threads` — build without threading APIs.
  * `--disable-loadlibs` — disable runtime loading of plugin libraries (forces static behaviour in some cases).
  * `--without-qjs` — build without QuickJS.
  * `--without-dylink` — disable dynamic loading of plugins.
  * `--without-fork` — disable fork support.
  * `--without-ptrace-wrap` — do not build ptrace-wrap support (used by some debugger backends).
  * `--without-gpl` — avoid building GPL-only components.

* Third-party libraries and system vs bundled
  * `--with-capstone4`, `--with-capstone5`, `--with-capstone-next` — choose which bundled capstone branch to build.
  * `--with-syscapstone` / `--without-syscapstone` — force use (or avoid) system capstone.
  * `--with-syslz4`, `--with-syszip`, `--with-sysxxhash` — force use of system libraries when available.
  * `--with-ssl` / `--with-ssl-crypto` — enable SSL support if available.
  * `--with-libuv` — prefer libuv when available.

* Misc
  * `--with-rpath` — set rpath handling in built binaries.
  * `--with-new-abi` — enable the new/experimental ABI changes.
  * `--with-compiler=...` or set `CC=` environment variable to choose compiler.

Environment variables you will commonly set when invoking configure or make:

* `CC` — C compiler (e.g., `CC=clang`).
* `CFLAGS` — extra compiler flags (e.g., `CFLAGS='-O2 -g'`).
* `LDFLAGS` — linker flags (`LDFLAGS='-L/opt/local/lib'`).
* `CPPFLAGS` — preprocessor flags (`CPPFLAGS='-I/opt/local/include'`).

Examples (Make):

* Build for local prefix and disable debugger:

  ./configure --prefix=$HOME/.local --disable-debugger
  make
  make install

* Cross-compile example (simple):

  ./configure --host=arm-linux-gnueabihf --prefix=/usr
  make

After running `./configure` (and/or `configure-plugins`) build with `make` or `gmake` depending on your platform.

### Meson: options and how to use them

The Meson build exposes many options (see `meson_options.txt`). Meson options are passed via `-Dname=value` when configuring the build directory. Some common options and their meanings:

* General build options
  * `-Dlocal=true|false` — prepare a local/side-by-side installation (sets rpath if needed).
  * `-Dstatic_runtime=true` — link all libraries statically into the binary.
  * `-Dblob=true` — build a single dispatching binary (calls different handlers depending on argv[0]).
  * `-Dwant_threads=true|false` — enable threading support.
  * `-Ddebugstuff=true|false` — enable handling of debug hooks and logs.
  * `-Dchecks_level=<int>` — control assert levels (0..3).

* Plugin selection
  * `-Dplugins='comma,separated,list'` — explicitly build only the named plugins (plugin names without `p/` prefix; use `lib.plugin` format, e.g. `io.debug,asm.x86`).
  * `-Duse_dylink=true|false` — enable (default true) building of shared plugin libraries.

* Capstone / third-party selection
  * `-Dwant_capstone=true|false` — build capstone support.
  * `-Duse_sys_capstone=true|false` — use system capstone instead of bundled.
  * `-Duse_capstone_version=v4|v5|next` — select which capstone branch to use.
  * `-Duse_sys_magic=true|false`, `-Duse_sys_zlib=true|false`, `-Duse_sys_lz4=true|false`, `-Duse_sys_xxhash=true|false` — prefer system libraries.

* Debugger / runtime
  * `-Ddebugger=true|false` — build with debugger support.
  * `-Dwant_qjs=true|false` — enable QuickJS runtime.
  * `-Dwant_ptrace_wrap=true|false` — build ptrace wrapper support.

* Tests and extras
  * `-Denable_tests=true|false` — build unit tests (test/unit).
  * `-Denable_libfuzzer=true|false` — build libFuzzer targets.
  * `-Denable_r2r=true|false` — build the r2r regression executable.

A few additional Meson-specific options you will find handy:

* `-Dstatic_themes=true` — compile default themes into the binary so they are available even if not installed on disk.
* `-Dnogpl=true` — avoid GPL-only components.
* `-Duse_webui=true` — build/install bundled WebUIs.

Examples (Meson):

* Basic configure + build (default options):

```console
meson setup builddir
ninja -C builddir
```

* Configure with options and a prefix:

```
meson setup builddir --prefix=/usr -Dlocal=true -Dwant_qjs=false -Duse_capstone_version=v5
ninja -C builddir
ninja -C builddir install
```

* Single-line example using environment variables and a release build:

```
CC=clang LDFLAGS='-fuse-ld=gold' meson setup release --buildtype=release --prefix=$HOME/.local -Dlocal=true -Dstatic_runtime=false
ninja -C release
```

Notes on plugin selection parity between Make and Meson:

* Make uses `configure-plugins` and the `plugins.cfg` mechanism.
* Meson accepts a `-Dplugins` option and also exposes flags for static/shared linking via `-Duse_dylink` and `-Dstatic_runtime`. The semantics are slightly different but accomplish the same goal: control which plugins are built and how they are linked.

### Quick workflow examples

Build with GNU Make, local prefix, and static plugins for smaller distribution:

```console
./configure-plugins --static io.bind asm.mycpu
./configure --prefix=$HOME/.local --disable-debugger --without-qjs
make -j$(nproc)
make install
```

The Make build system also permits doing installations with symlinks, so you don't have to reinstall after recompiling because all system-wide files will be pointing to the ones in the builddir.

Build with Meson, minimal plugin set, release build:

```console
meson setup release --buildtype=release --prefix=$HOME/.local -Dplugins='io.debug,asm.x86' -Dwant_qjs=false -Duse_sys_capstone=true
ninja -C release
ninja -C release install
```

### Cleaning and housekeeping

To ensure a clean tree before rebuilding:

```console
git clean -xdf
git reset --hard origin/master
git pull
```

If you need to remove previously installed files when switching prefixes, run the `make purge` step after configuring with the intended prefix.
