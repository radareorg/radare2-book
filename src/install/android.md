## Compiling for Android

Radare2 can be cross-compiled for other architectures/systems as well, like Android.

### Prerequisites

* Python 3
* Make
* Git
* Binutils
* pkg-config

### Step-by-step

#### Download and Install Termux App

Download the Termux application from the [official site](https://github.com/termux/termux-app/releases) and install it.

#### Update & Upgrade

First time installation of termux require updating the repo to fetch all the available packages.
```
$ pkg update && pkg upgrade -y
```

##### Install required packages


```
$ pkg install build-essential git python3 binutils wget pkg-config
```
build-essential contains all the required build tool like make,gcc etc.

##### Download or clone Radare2 repo

```
$ git clone https://github.com/radareorg/radare2
```
If you are limited by disk space, you can either clone the repository with a depth of 1 by adding `--depth 1` in clone command or build from a tarball. Cloning the repository provides the most up-to-date code, whereas tarballs are only generated during releases, which may not contains latest update and bug fixes.


##### Build and Installation

Building and installing Radare2 after cloning the repository is straightforward using the following commands:

```
cd radare2
sh sys/install.sh
```
It will install required packages if you already didn't and start the installation.

```
~/radare2 $ sh sys/install.sh
/data/data/com.termux/files/home/radare2
Termux environment detected. Installing necessary packages
No mirror or mirror group selected. You might want to select one by running 'termux-change-repo'
Checking availability of current mirror:
[*] https://packages-cf.termux.dev/apt/termux-main: ok
Hit:1 https://packages-cf.termux.dev/apt/termux-main stable InRelease
Reading package lists... Done
Building dependency tree... Done
Reading state information... Done
All packages are up to date.
No mirror or mirror group selected. You might want to select one by running 'termux-change-repo'
Checking availability of current mirror:
[*] https://packages-cf.termux.dev/apt/termux-main: ok
Reading package lists... Done
Building dependency tree... Done
Reading state information... Done
git is already the newest version (2.45.2).
build-essential is already the newest version (4.1).
binutils is already the newest version (2.42).
pkg-config is already the newest version (0.29.2-2).
0 upgraded, 0 newly installed, 0 to remove and 0 not upgraded.
/data/data/com.termux/files/home/radare2
From https://github.com/radareorg/radare2
 * branch            master     -> FETCH_HEAD
Already up to date.
[*] Finding make is /data/data/com.termux/files/usr/bin/make OK
[*] Configuring the build system ... OK
[*] Checking out capstone... OK
[*] Checking out vector35-arm64... OK
[*] Checking out vector35-armv7... OK
[*] Running configure... OK
[*] Ready. You can now run 'make'.
configure-plugins: Loading ./plugins.cfg ..
configure-plugins: Generating libr/config.h
configure-plugins: Generating libr/asm/d/config.inc
configure-plugins: Generating libr/config.mk
........
........
cd "/data/data/com.termux/files/usr/lib/radare2/" && rm -f last && ln -fs 5.9.3 last
cd "/data/data/com.termux/files/usr/share/radare2/" && rm -f last && ln -fs 5.9.3 last
mkdir -p "/data/data/com.termux/files/usr/share/radare2/5.9.3/"
/data/data/com.termux/files/usr/bin/sh ./configure-plugins --rm-static //data/data/com.termux/files/usr/lib/radare2/last/
configure-plugins: Loading ./plugins.cfg ..
Removed 0 shared plugins that are already static

~/radare2 $ r2 -v
radare2 5.9.3 275 @ linux-arm-64
birth: git.5.9.2-146-g13ea460 2024-06-28__20:22:10
commit: 13ea460b3ea28ef37361eb1d679561037c521d27
options: gpl -O? cs:5 cl:2 make

```
