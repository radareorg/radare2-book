## Compilation on Windows

### Meson (MSVC)

**WARNING: This build is not being tested for the moment. Note that it does not compile every plugins available in radare2 yet. Refer to the [Mingw32](#mingw32) build below for a more stable and complete build.**

The most native way to compile radare2 under Windows is to use meson + msvc. First you need **python3** to be installed on your computer. Once this is done, you can install the meson build system using `pip3 install meson` (with Administrator privileges).
Now navigate to your Python installation folder, and copy the `meson.py` from `.\Scripts` subfolder into your radare2 folder.

Meson also requires Ninja. You can download it from [here](https://ninja-build.org/). Copy `ninja.exe` binary into your radare2 folder. Then run `meson.bat` and wait until compilation is done.

#### Compiling

* At first you will need to run `vsvarsall.bat` from the corresponding directory of your Visual Studio or Visual C++ Build Tools.
In case of VS2015 it is located usually in `C:\\Program Files (x86)\\Microsoft Visual Studio 14.0\\VC\\` or
in `C:\\Program Files (x86)\\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat`

* Using **Ninja build system**: This is the easiest, simply run `meson.bat` and wait until the compilation is complete.

* Using **Visual Studio**: If you want to work on radare2 in Visual Studio, you can simply run the command `meson.bat -p` which will generate a Visual Studio 2015 project. Then you can load it from `build\radare2.sln`.

* Using **MSBuild**: Call `meson.bat --msbuild`

In any case, everything is generated in the `build` folder by default, except for the sdb files generation process.

#### Installing

Now that you successfully compiled radare2, you might want to gather every executables and requirements into the same place. The command below will collect everything and put it into the `dist` folder.
```bat
E:\radare2>sys\meson_install.bat dist
```

You can now test your newly crafted radare2 binary easily:
```bat
E:\radare2>cd dist
E:\radare2\dist>radare2.exe -v
```

### Mingw32

The easy way to compile things for Windows is using Mingw32. The w32 builds distributed from the radare homepage are generated from a GNU/Linux box using Mingw32 and they are tested with Wine. Also keep in mind, that Mingw-w64 isn't tested, so no guarantees here.

Be sure to setup your Mingw32 to compile with **thread model: win32**, not **posix**, and target should be **mingw32**.
Before the starting of compilation you need to setup git first, for a proper automatic fetching of capstone:
```sh
git config --global core.autocrlf true
git config --global core.filemode false
```

The following is an example of compiling with MinGW32 (you need to have installed **zip** for Windows):
```sh
CC=i486-mingw32-gcc ./configure
make
make w32dist
zip -r w32-build.zip w32-build
```
This generates a native, 32-bit console application for Windows.
The 'i486-mingw32-gcc' compiler is the one I have in my box, you will probably need to change this.

To simplify the building under Windows/Mingw32 there is a script in radare2 sources:
`sys/mingw32.bat`. Simply run it from the cmd.exe (or ConEmu/cmd.exe).
It assumes that you have Mingw32 installed in `C:\Mingw` and Git in `C:\Program Files (x86)\Git`. If you want to
use another installations, just set `MINGW_PATH` and `GIT_PATH` variables correspondingly:
```
set MINGW_PATH=D:\Mingw32
set "GIT_PATH=E:\Program and Stuff\Git"
sys\mingw32.bat
```

Please, note, that this script should be run from radare2 directory.


There is a script that automates process of detecting the crosscompiler toolchain configuration, and builds a zip file containing r2 programs and libraries that can be deployed on Windows or Wine:

```sh
sys/mingw32.sh
```

### Cygwin

Cygwin is another possibility; however, issues related to Cygwin libraries can make debugging difficult. But using binary compiled for Cygwin will allow you to use Unicode in the Windows console, and to have 256 colors.

Note, Cygwin build require exactly the opposite git configuration, so setup git first, for a proper automatic fetching of capstone:
```sh
git config --global core.autocrlf false
```

Please, be sure to build radare2 from the same environment you're going to use r2 in. If you are going to use r2 in MinGW32 shell or cmd.exe — you should build r2 in the MinGW32 environment. And if you are going to use r2 in Cygwin — you have to build r2 from the Cygwin shell. Since Cygwin is more UNIX-compatible than MinGW, the radare2 supports more colors and Unicode symbols if build using the former one.



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
pacman -S git make zip gcc patch
```
 - Compile the radare2:
```sh
./configure --with-ostype=windows ; make ; make w32dist
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
