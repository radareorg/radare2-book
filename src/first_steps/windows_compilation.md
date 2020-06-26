## Windows

Radare2 relies on the Meson build system generator to support compilation on all platforms, including Windows. Meson will generate a Visual Studio Solution, all the necessary project files, and wire up the Microsoft Visual C++ compiler for you.

> **tip** You can download nightly binaries from https://ci.appveyor.com/project/radareorg/radare2/history. Be sure to download only from `master` branch!

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
2. Clone the repository with `git clone https://github.com/radareorg/radare2.git`

#### Compile Radare2 Code
Follow these steps to compile the Radare2 Code.

Compiled binaries will be installed into the `dest` folder.

1. Enter the Radare2 Conda environment
2. Navigate to the root of the Radare2 sources (`cd radare2`)
3. Initialize Visual Studio tooling by executing the command below that matches the version of Visual Studio installed on your machine and the version of Radare2 you wish to install:

    * **Visual Studio 2015:**

        Note: For the 64-bit version change only the `x86` at the very end of the command below to `x64`.

        `"%ProgramFiles(x86)%\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x86`

    * **Visual Studio 2017:**

        Note 1: Change `Community` to either `Professional` or `Enterprise` in the command below depending on the version installed.

        Note 2: Change `vcvars32.bat` to `vcvars64.bat` in the command below for the 64-bit version.

         `"%ProgramFiles(x86)%\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars32.bat"`

    * **Visual Studio Preview:**

        Note 1: Change `Community` to either `Professional` or `Enterprise` in the command below depending on the version installed.

        Note 2: Change `vcvars32.bat` to `vcvars64.bat` in the command below for the 64-bit version.

        `"%ProgramFiles(x86)%\Microsoft Visual Studio\Preview\Community\VC\Auxiliary\Build\vcvars32.bat"`


4. Generate the build system with Meson:

  Note 1: Change `debug` to `release` in the command below depending on whether the latest version or release version is desired.

  Note 2: If you are using visual studio 2017, you can change swap `vs2015` for `vs2017`.

  `meson build --buildtype debug --backend vs2015 --prefix %cd%\dest`

  Meson currently requires `--prefix` to point to an absolute path. We use the %CD% pseudo-variable to get the absolute path to the current working directory.

5. Start a build:

    Note: Change `Debug` to `Release` in the command below depending on the version desired.

    `msbuild build\radare2.sln /p:Configuration=Debug /m`

    The `/m[axcpucount]` switch creates one MSBuild worker process per logical processor on your machine. You can specify a numeric value (e.g. `/m:2`) to limit the number of worker processes if needed. (This should not be confused with the Visual C++ Compiler switch `/MP`.)

    If you get an error with the 32-bit install that says something along the lines of `error MSB4126: The specified solution configuration "Debug|x86" is invalid.` Get around this by adding the following argument to the command: `/p:Platform=Win32`

6. Install into your destination folder: `meson install -C build --no-rebuild`
7. Check your Radare2 version: `dest\bin\radare2.exe -v`

#### Check That Radare2 Runs From All Locations
1. In the file explorer go to the folder Radare2 was just installed in.
2. From this folder go to `dest` > `bin` and keep this window open.
3. Go to System Properties: In the Windows search bar enter `sysdm.cpl`.
4. Go to `Advanced > Environment Variables`.
5. Click on the PATH variable and then click edit (if it exists within both the user and system variables, look at the user version).
6. Ensure the file path displayed in the window left open is listed within the PATH variable. If it is not add it and click `ok`.
7. Log out of your Windows session.
8. Open up a new Windows Command Prompt: type `cmd` in the search bar. Ensure that the current path is not in the Radare2 folder.
9. Check Radare2 version from Command Prompt Window: `radare2 -v`
