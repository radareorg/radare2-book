## Compilation on Windows

The easy way to compile things for Windows is using MinGW32. The w32 builds distributed from the radare homepage are generated from a GNU/Linux box using MinGW32 and they are tested with Wine. Also keep in mind, that MinGW-w64 wan't tested, so no guarantees here.

Be sure to setup your MinGW32 to compile with **thread model: win32**, not **posix**, and target should be **mingw32**.

The following is an example of compiling with MinGW32 (you need to have installed **zip** for Windows):
```sh
CC=i486-mingw32-gcc ./configure
make
make w32dist
zip -r w32-build.zip w32-build
```
This generates a native, 32-bit console application for Windows.
The 'i486-mingw32-gcc' compiler is the one I have in my box, you will probably need to change this.

Cygwin is another possibility; however, issues related to Cygwin libraries can make debugging difficult. But using binary compiled for Cygwin will allow you to use Unicode in the Windows console, and to have 256 colors.

Please, be sure to build radare2 from the same environment you're going to use r2 in. If you are going to use r2 in MinGW32 shell or cmd.exe — you should build r2 in the MinGW32 environment. And if you are going to use r2 in Cygwin — you have to build r2 from the Cygwin shell. Since Cygwin is more UNIX-compatible than MinGW, the radare2 supports more colors and Unicode symbols if build using the former one.

There is a script that automates process of detecting the crosscompiler toolchain configuration, and builds a zip file containing r2 programs and libraries that can be deployed on Windows or Wine:

```sh
sys/mingw32.sh
```

### Mingw-W64

 - Download the MSYS2 distribution from the official site: http://msys2.github.io/
 - Setup the proxy (if needed):
```sh
export http_proxy=<myusername>:<mypassword>@zz-wwwproxy-90-v:8080
export https_proxy=$http_proxy
export ftp_proxy=$http_proxy
export rsync_proxy=$http_proxy
export rsync_proxy=$http_proxy
export no_proxy="localhost,127.0.0.1,localaddress,.localdomain.com"
```
 - Update packages:
```sh
pacman --needed -Sy bash pacman pacman-mirrors msys2-runtime mingw-w64-x86_64-toolchain
```
 - Close MSYS2, run it again from Start menu and update the rest with
```sh
pacman -Su
```
 - Install the building essentials:
```sh
pacman -S git make zip gcc
```
 - Compile the radare2:
```sh
./configure ; make ; make w32dist
```
### Bindings

To build radare2 bindings, you will need to install [Vala (valac) for Windows](https://wiki.gnome.org/Projects/Vala/ValaOnWindows)

Then download [valabind](https://github.com/radare/valabind) and build it:
```sh
git clone https://github.com/radare/valabind.git valabind
cd valabind
make
make install
```

After you installed valabind, you can build radare2-bindings, for example for Python and Perl:
```sh
git clone https://github.com/radare/radare2-bindings.git radare2-bindings
cd radare2-bindings
./configure --enable=python,perl
make
make install
```
