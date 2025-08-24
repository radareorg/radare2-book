## Windows Binaries

To build **radare2** on Windows, use the Meson build system. While you can compile r2 on Windows with Cygwin, MinGW, or WSL using the acr/make build system, those methods are unofficial and unsupported and may produce unexpected results.

Prebuilt binaries are available on the [releases page](https://github.com/radareorg/radare2/releases). If you're signed in to GitHub, you can also access the CI artifacts that are produced for every commit.

### Prerequisites

Building radare2 on Windows using Microsoft Visual Studio involves setting up several key tools. You will need to install Microsoft Visual Studio, along with the Meson build system and the Ninja build tool. These tools are necessary for configuring and compiling the radare2 source code.

These are the requirements to get the environment ready.

* 3 GB of free disk space
* Visual Studio (2019 or higher)
* Python 3
* Meson
* Ninja
* Git

### Downloading the code

You may need to download the zip/tgz or git clone the last code from git in order to run the following commands in cmd or powershell:

```sh
git clone https://github.com/radareorg/radare2
```

Follow these instructions to pull the build tools to get the project compiled on your Windows machine.

### Native Windows Compilation

The build process on windows has been heavily simplified to just 3 batch scripts, which is how it's executed in the CI and the recommended workflow.

The scripts expect you to have Python, Git, Visual Studio and PowerShell, and will run Meson and Ninja by default (both easily installable via pip).

* `preconfigure.bat` find vscode, checks for python installation etc
* `configure.bat` run the meson configure step.
* `make.bat` build radare2 and create `prefix` directory with the distributable folder.

You can run `make` from the root directory of the project everytime you modify the source code in case you are developing to get the new binaries ready to test in that directory.

The configure.bat will also create a directory named `vs` which contains the visual studio project in case you want to use the graphical IDE for developing or debugging. This `.vsproj` is constructed automatically from the meson scripts, this simplifies maintainance.

### Crosscompiling

MinGW builds are supported, but official releases are compiled with the Microsoft compiler to maximize compatibility and standardization with other Windows software.

If you want to build r2 for Windows on Linux, use the `sys/mingw32.sh` script — it autodetects the MinGW toolchain on your system and produces the `.exe` and `.dll` files.

Note: Cygwin support was largely removed several years ago. Today, most users prefer WSL or native builds, which also work well on ReactOS and across many Windows versions without requiring extra compatibility software.

### Installing Build Dependencies

Take the following sub-sections of this chapter as a guide to install the software you need to compile radare2 from source:

#### Install Visual Studio 2019 (or higher)

Please install Visual Studio with the Visual C++ toolset (MSVC), the C++ libraries, and the Windows SDK that matches your target platform.

You can download Visual Studio — the Community editions are free — from here:

* [Download Visual Studio 2019](https://visualstudio.microsoft.com/downloads/)

#### Install Python3

Conda is probably the best Python distribution for Windows, but if you already have Python installed you can skip the following steps.

You can install Python3 on Windows using [Conda](https://docs.conda.io/projects/conda/en/stable/), the [official installer](https://www.python.org/downloads/), or package managers like choco or winget — pick whichever you prefer.

After installing, make sure the Python bin directory is on your PATH so you can open CMD or PowerShell and run `python3` (for example, `python3 --version` to verify).

Now you are ready to install meson and ninja — the radare2 build tools — using pip:

```sh
pip install ninja
pip install meson
```

#### Install Git for Windows

All Radare2 code is managed with Git and hosted on [GitHub](https://github.com/radareorg).

To install Git for Windows, follow these steps:

1. Download Git for Windows from the [official website](https://git-scm.com/download/win).

2. In the installation wizard, choose the following options:

* Use a TrueType font in all console windows
* Use Git from the Windows Command Prompt
* Use the native Windows Secure Channel library (instead of OpenSSL)
* Checkout Windows-style, commit Unix-style line endings (`core.autocrlf=true`)
* Use Windows' default console window (instead of Mintty)

3. After installation, verify it by running `git --version` to confirm Git is available.
