## Windows Builds

To build `r2` on Windows you have to use the Meson build system. Despite being able to build r2 on Windows using cygwin, mingw or WSL using the acr/make build system it is not the recommended/official/supported method and may result on unexpected results.

Binary builds can be downloaded from the [release page](https://github.com/radareorg/radare2/releases) or when logged in, access the github CI artifacts that are built on every commit.

### Crosscompiling

The mingw builds are also possible, but release builds are made with the Microsoft compiler for maximum compatibility and standarization with other software you can use.

If you want to build r2 for windows on linux you can use the `sys/mingw32.sh` script that will autodetect the mingw toolchain from your system and build all the .exe and .dll.

Note that cygwin support was kind of removed some years ago and nowadays most users will opt for WSL or purely native builds that will work well even on ReactOS and many different versions of windows without special software installed.

### Prerequisites

Building radare2 on Windows using Microsoft Visual Studio involves setting up several key tools. You will need to install Microsoft Visual Studio, along with the Meson build system and the Ninja build tool. These tools are necessary for configuring and compiling the radare2 source code.

These are the requirements to get the environment ready.

* 3 GB of free disk space
* Visual Studio (2019 or higher)
* Python 3
* Meson
* Ninja
* Git

### Step-by-Step

Follow these instructions to pull the build tools to get the project compiled on your Windows machine.

#### Install Visual Studio 2019 (or higher)

Visual Studio must be installed with a Visual C++ compiler, supporting C++ libraries, and the appropriate Windows SDK for the target platform version.

You can find a copy of Visual Studio (the Community versions are free for download) in here:

* [Download Visual Studio 2019](https://visualstudio.microsoft.com/downloads/)

#### Install Python 3

Conda is our probably the best Python distribution for Windows. But you can skip the next steps if you have Python installed already

You can install Python3 for Windows using [Conda](https://docs.conda.io/projects/conda/en/stable/), but also the [standard distribution](https://www.python.org/downloads/), choco, winget, etc. Choose the one you like the most

When python is installed ensure to set the bin directory in your PATH, so you can launch a CMD or Powershell and run `python3`.

Now you are ready to install meson and ninja, the radare2 build tools with pip:

```sh
pip install ninja
pip install meson
```

#### Install Git for Windows

All Radare2 code is managed via the Git version control system and [hosted on GitHub](https://github.com/radareorg).

Follow these steps to install Git for Windows.

Download Git for Windows from the [official website](https://git-scm.com/download/win)

Check the following options during the Wizard steps.

* Use a TrueType font in all console windows
* Use Git from the Windows Command Prompt
* Use the native Windows Secure Channel library (instead of OpenSSL)
* Checkout Windows-style, commit Unix-style line endings (core.autocrlf=true)
* Use Windows' default console window (instead of Mintty)
* Ensure `git --version` works after install

#### Pull the code

Follow these steps to clone the Radare2 git repository.

```sh
git clone https://github.com/radareorg/radare2
```

#### Compile Radare2 Code

The build process on windows has been heavily simplified to just 3 batch scripts:

* preconfigure.bat : find vscode, checks for python installation etc
* configure.bat : run the meson configure step.
* make.bat : build radare2 and create `prefix` directory with the distributable folder.

You can run `make` from the root directory of the project everytime you modify the source code in case you are developing to get the new binaries ready to test in that directory.
