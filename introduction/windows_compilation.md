## Windows compilation

The easy way to compile things for Windows is using MinGW32. The w32 builds distributed in the radare homepage are generated from a GNU/Linux box using MinGW32 and they are tested with Wine. Also keep in mind, that MinGW-w64 wan't tested, so no guarantees here.

Be sure to setup your MinGW32 to compile with **thread model: win32**, not **posix**, and target should be **mingw32**.

To compile type:
```sh
CC=i486-mingw32-gcc ./configure
make
make w32dist
zip -r w32-build.zip w32-build
```
The 'i486-mingw32-gcc' compiler is the one I have in my box, you will probably need to change this. MinGW32 will generate a native console application for Windows.

Another possible way to compile radare2 on w32 is using Cygwin, which I dont really recommend at all because of the problems related to the Cygwin librarires makes the program quite hard to be debugged in case of problems.
But using Cygwin compiled binary will allow you to use the Unicode in the console, and to use 256 colors.

There's a script that automates the process of autodetecting the crosscompiler toolchain and build a zip file containing the r2 programs and libraries that should be deployed in a windows box (or wine).
```sh
sys/mingw32.sh
``` 
## Mingw-W64

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
 - Update the packages: 
```sh
pacman --needed -Sy bash pacman pacman-mirrors msys2-runtime
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
