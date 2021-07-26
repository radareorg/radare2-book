## Windows

To build `r2` on Windows you have to use the Meson build system. Despite being able to build r2 on Windows using cygwin, mingw or wsl using the acr/make build system it is not the recommended/official/supported method and may result on unexpected results.

Binary builds can be downloaded from the release page or the github CI artifacts from every single commit for 32bit and 64bit Windows.

* https://github.com/radareorg/radare2/releases

### Prerequisites

* 3 GB of free disk space
* Visual Studio 2019 (or higher)
* Python 3
* Meson
* Ninja
* Git

### Step-by-Step

#### Install Visual Studio 2015 (or higher)

Visual Studio must be installed with a Visual C++ compiler, supporting C++ libraries, and the appropriate Windows SDK for the target platform version.

* Ensure `Programming Languages > Visual C++` is selected

If you need a copy of Visual Studio, the Community versions are free and work great.

* [Download Visual Studio 2019](https://visualstudio.microsoft.com/downloads/)

#### Install Python 3 and Meson via Conda

Conda is our probably the best Python distribution for Windows. But you can skip the next steps if you have Python installed already

* https://docs.conda.io/en/latest/miniconda.html
* https://repo.anaconda.com/archive/

##### Create a Python Environment for Radare2

Follow these steps to create and activate a Conda environment named *r2*. All instructions from this point on will assume this name matches your environment, but you may change this if desired.

1. Start > Anaconda Prompt
2. `conda create -n r2 python=3`
3. `activate r2`

Any time you wish to enter this environment, open the Anaconda Prompt and re-issue `activate r2`. Conversely, `deactivate` will leave the environment.

##### Install Meson

Ensure Meson is version 0.48 or higher (`meson -v`)

```
pip install meson
```

#### Install Git for Windows

All Radare2 code is managed via the Git version control system and [hosted on GitHub](https://github.com/radareorg).

Follow these steps to install Git for Windows.

Download Git for Windows 

* https://git-scm.com/download/win

Check the following options during the Wizard steps.

* Use a TrueType font in all console windows
* Use Git from the Windows Command Prompt
* Use the native Windows Secure Channel library (instead of OpenSSL)
* Checkout Windows-style, commit Unix-style line endings (core.autocrlf=true)
* Use Windows' default console window (instead of Mintty)
* Ensure `git --version` works after install


#### Get Radare2 Code

Follow these steps to clone the Radare2 git repository.

```
git clone https://github.com/radareorg/radare2
```

#### Compile Radare2 Code

Follow these steps to compile the Radare2 Code.

    * **Visual Studio 2017:**

        Note 1: Change `Community` to either `Professional` or `Enterprise` in the command below depending on the version installed.

        Note 2: Change `vcvars32.bat` to `vcvars64.bat` in the command below for the 64-bit version.

         `"%ProgramFiles(x86)%\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars32.bat"`

4. Generate the build system with Meson:

Meson takes some arguments to configure the build type, but in short you should be able to build r2 without any meson flag. For Visual Studio. Note: Change `Debug` to `Release` in the command below depending on the version desired.

```
meson b --buildtype debug --backend vs2019 --prefix %cd%\dest
msbuild build\radare2.sln /p:Configuration=Debug /m
```

For Ninja (no visual studio interface required, just msvc compiler toolchain installed):

```
meson b
ninja -C b
```

Finally run this line to install r2 into the given absolute prefix directory:

```
meson install -C build --no-rebuild
```

#### Build options notes

The `/m[axcpucount]` switch creates one MSBuild worker process per logical processor on your machine. You can specify a numeric value (e.g. `/m:2`) to limit the number of worker processes if needed. (This should not be confused with the Visual C++ Compiler switch `/MP`.)

If you get an error with the 32-bit install that says something along the lines of `error MSB4126: The specified solution configuration "Debug|x86" is invalid.` Get around this by adding the following argument to the command: `/p:Platform=Win32`

Check your Radare2 version: `dest\bin\radare2.exe -v`

#### Check That Radare2 Runs From All Locations

Note that `r2` in UNIX systems is just a symlink to the `radare2` executable. So, in case you want to have it in Windows you can just `copy radare2.exe r2.exe` and add the directory into the system-wide PATH env var in the **File Explorer** settings.

Open the `cmd.exe` console and type `r2 -v` to confirm the whole process was successful.

#### Notes about setting up the system-wide env var

1. In the file explorer go to the folder Radare2 was just installed in.
2. From this folder go to `dest` > `bin` and keep this window open.
3. Go to System Properties: In the Windows search bar enter `sysdm.cpl`.
4. Go to `Advanced > Environment Variables`.
5. Click on the PATH variable and then click edit (if it exists within both the user and system variables, look at the user version).
6. Ensure the file path displayed in the window left open is listed within the PATH variable. If it is not add it and click `ok`.
7. Log out of your Windows session.
8. Open up a new Windows Command Prompt: type `cmd` in the search bar. Ensure that the current path is not in the Radare2 folder.
9. Check Radare2 version from Command Prompt Window: `radare2 -v`
