## Windows

Radare2 relies on the Meson build system generator to support compilation on all platforms, including Windows. Meson will generate a Visual Studio Solution, all the necessary project files, and wire up the Microsoft Visual C++ compiler for you.

> **tip** You can download nightly binaries from https://bin.rada.re.

### Prerequisites

* Visual Studio 2015 (or higher)
* Python 3
* Meson
* Git

### Step-by-Step

#### Install Visual Studio 2015 (or higher)

Visual Studio must be installed with a Visual C++ compiler, supporting C++ libraries, and the appropriate Windows SDK for the target platform version.

* In the Visual Studio 2015 installer, ensure `Programming Languages > Visual C++` is selected
* In the Visual Studio 2017+ installers, ensure the `Desktop development with C++` workload is selected

If you need a copy of Visual Studio, the Community versions are free and work great.

* [Download Visual Studio 2015 Community (registration required)](https://my.visualstudio.com/Downloads?q=Visual%20Studio%202015%20with%20Update%203)
* [Download Visual Studio 2017 Community](https://visualstudio.microsoft.com/downloads/)

#### Install Python 3 and Meson via Conda
It is strongly recommended you install Conda — a Python environment management system — when working with Python on the Windows platform. This will isolate the Radare2 build environment from other installed Python versions and minimize potential conflicts.

##### Set Up Conda:
1. Download the appropriate Conda (Python 3.x) for your platform (https://conda.io/miniconda.html)
2. Install Conda with the recommended defaults

##### Create a Python Environment for Radare2
Follow these steps to create and activate a Conda environment named *r2*. All instructions from this point on will assume this name matches your environment, but you may change this if desired.

1. Start > Anaconda Prompt
2. `conda create -n r2 python=3`
3. `activate r2`

Any time you wish to enter this environment, open the Anaconda Prompt and re-issue `activate r2`. Conversely, `deactivate` will leave the environment.

##### Install Meson
> **tip** All versions of Meson at or below 0.47.1 have a bug that prevent normal use on Windows. Because there's no official release with the fixes available, you must install from sources. The following steps will walk you through this. We will update this documentation as soon as 0.48 is officially released.

1. Enter the Radare2 Conda environment, if needed (`activate r2`)
2. Download https://github.com/mesonbuild/meson/archive/master.zip
3. `pip install \path\to\downloaded\master.zip`
4. Verify Meson is version 0.48 or higher (`meson -v`)

#### Install Git for Windows
All Radare2 code is managed via the Git version control system and [hosted on GitHub](https://github.com/radare).

Follow these steps to install Git for Windows.

1. Download Git for Windows (https://git-scm.com/download/win)

  As you navigate the install wizard, we recommend you set these options when they appear:
    * Use a TrueType font in all console windows
    * Use Git from the Windows Command Prompt
    * Use the native Windows Secure Channel library (instead of OpenSSL)
    * Checkout Windows-style, commit Unix-style line endings (core.autocrlf=true)
    * Use Windows' default console window (instead of Mintty)

2. Close any previously open console windows and re-open them to ensure they receive the new PATH
3. Ensure `git --version` works

#### Get Radare2 Code
Follow these steps to clone the Radare2 git repository.

1. In your Radare2 Conda environment, navigate to a location where the code will be saved and compiled. This location needs approximately **3-4GiB** of space
2. Clone the repository with `git clone https://github.com/radare/radare2.git`

#### Compile Radare2 Code
Follow these steps to compile a debug 32-bit (x86) version of Radare2. (If you want to build a 64-bit (x64) version of Radare2, replace all instances of `x86` with `x64`. Similarly, if you want to build a release version, replace all instances of `debug` with `release`.)

Compiled binaries will be installed into the `dest` folder.

1. Enter the Radare2 Conda environment
2. Navigate to the root of the Radare2 sources (`cd radare2`)
3. Initialize Visual Studio tooling by executing the command that matches the version of Visual Studio installed on your machine:

    * Visual Studio 2015:
    `"%ProgramFiles(x86)%\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x86`

    * Visual Studio 2017:
    `"%ProgramFiles(x86)%\Microsoft Visual Studio\2017\Enterprise\VC\Auxiliary\Build\vcvars32.bat"`

    * Visual Studio Preview:
    `"%ProgramFiles(x86)%\Microsoft Visual Studio\Preview\Enterprise\VC\Auxiliary\Build\vcvars32.bat"`

4. Generate the build system with Meson: `meson build --buildtype debug --backend vs2015 --prefix %cd%\dest`
  
    Meson currently requires `--prefix` to point to an absolute path. We use the %CD% pseudo-variable to get the absolute path to the current working directory.

5. Start a build: `msbuild build\radare2.sln /p:Configuration=Debug /m`
  
    The `/m[axcpucount]` switch creates one MSBuild worker process per logical processor on your machine. You can specify a numeric value (e.g. `/m:2`) to limit the number of worker processes if needed. (This should not be confused with the Visual C++ Compiler switch `/MP`.)

6. Install into your destination folder: `meson install -C build --no-rebuild`
7. Check your Radare2 version: `dest\bin\radare2.exe -v`
