## Android

Radare2 can be cross-compiled for other architectures/systems as well, like Android.

### Prerequisites

* Python 3
* Meson
* Ninja
* Git
* Android NDK

### Step-by-step

#### Download and extract the Android NDK

Download the Android NDK from the [official site](https://developer.android.com/ndk) and extract it somewhere on your system (e.g. `/tmp/android-ndk`)

#### Make

##### Specify NDK base path

```
$ echo NDK=/tmp/android-ndk  > ~/.r2androidrc
```

##### Compile + create tar.gz + push it to connected android device

```
./sys/android-build.sh arm64-static
```

You can build for different architectures by changing the argument to
`./sys/android-build.sh`. Run the script without any argument to see the
accepted values.

#### Meson

##### Create a cross-file for meson

Meson needs a configuration file that describes the cross compilation environment (e.g. `meson-android.ini`).
You can adjust it as necessary, but something like the following should be a good starting point:
```
[binaries]
c       = '/tmp/android-ndk/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android28-clang'
cpp     = '/tmp/android-ndk/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android28-clang++'
ar      = '/tmp/android-ndk/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android-ar'
as      = '/tmp/android-ndk/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android-as'
ranlib  = '/tmp/android-ndk/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android-ranlib'
ld      = '/tmp/android-ndk/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android-ld'
strip   = '/tmp/android-ndk/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android-strip'
pkgconfig = 'false'

[properties]
sys_root = '/tmp/android-ndk/sysroot'

[host_machine]
system = 'android'
cpu_family = 'arm'
cpu = 'aarch64'
endian = 'little'
```

##### Compile with meson + ninja

Now setup the build directory with meson as usual:
```
$ CFLAGS="-static" LDFLAGS="-static" meson --default-library static --prefix=/tmp/android-dir -Dblob=true build --cross-file ./meson-android.ini
```

A bit of explanation about all the options:
* `CFLAGS="-static"`, `LDFLAGS="-static"`, `--default-library static`: this
  ensure that libraries and binaries are statically compiled, so you do not need
  to properly set LD_* environment variables in your Android environment to make
  it find the right libraries. Binaries have everything they need inside.
* `-Dblob=true`: it tells meson to compile just one binary with all the needed
  code for running `radare2`, `rabin2`, `rasm2`, etc. and creates symbolic links to
  those names. This avoids creating many statically compiled large binaries and
  just create one that provides all features. You will still have `rabin2`,
  `rasm2`, `rax2`, etc. but they are just symlinks to `radare2`.
* `--cross-file ./meson-android.ini`: it describes how to compile radare2 for Android

Then compile and install the project:
```
$ ninja -C build
$ ninja -C build install
```

##### Move files to your android device and enjoy

At this point you can copy the generated files in /tmp/android-dir to your Android device and running radare2 from it.
For example:
```
$ cd /tmp && tar -cvf radare2-android.tar.gz android-dir
$ adb push radare2-android.tar.gz /data/local/tmp
$ adb shell
DEVICE:/ $ cd /data/local/tmp
DEVICE:/data/local/tmp $ tar xvf radare2-android.tar.gz
DEVICE:/data/local/tmp $ ./android-dir/bin/radare2
Usage: r2 [-ACdfLMnNqStuvwzX] [-P patch] [-p prj] [-a arch] [-b bits] [-i file]
          [-s addr] [-B baddr] [-m maddr] [-c cmd] [-e k=v] file|pid|-|--|=
```
