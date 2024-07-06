## First Steps

If there's nothing after the peekaboo (`://`) you will get introduced into the visual uri maker which let's you select the target device, communication channel, and application/process to attach or spawn to start tracing from it.

```bash
$ r2 frida://
```

You can invoke the help menu via the following command:

```
$ r2 'frida://?'
r2 frida://[action]/[link]/[device]/[target]
* action = list | apps | attach | spawn | launch
* link   = local | usb | remote host:port
* device = '' | host:port | device-id
* target = pid | appname | process-name | program-in-path | abspath
Local:
* frida://                         # visual mode to select action+device+program
* frida://?                        # show this help
* frida://0                        # attach to frida-helper (no spawn needed)
* frida:///usr/local/bin/rax2      # abspath to spawn
* frida://rax2                     # same as above, considering local/bin is in PATH
* frida://spawn/$(program)         # spawn a new process in the current system
* frida://attach/(target)          # attach to target PID in current host
USB:
* frida://list/usb//               # list processes in the first usb device
* frida://apps/usb//               # list apps in the first usb device
* frida://attach/usb//12345        # attach to given pid in the first usb device
* frida://spawn/usb//appname       # spawn an app in the first resolved usb device
* frida://launch/usb//appname      # spawn+resume an app in the first usb device
Remote:
* frida://attach/remote/10.0.0.3:9999/558 # attach to pid 558 on tcp remote frida-server
Environment: (Use the `%` command to change the environment at runtime)
  R2FRIDA_SCRIPTS_DIR=/usr/local/share/r2frida/scripts
  R2FRIDA_SCRIPTS_DIR=~/.local/share/radare2/r2frida/scripts
  R2FRIDA_SAFE_IO=0|1              # Workaround a Frida bug on Android/thumb
  R2FRIDA_DEBUG=0|1                # Used to trace internal r2frida C and JS calls
  R2FRIDA_RUNTIME=qjs|v8           # Select the javascript engine to use in the agent side (v8 is default)
  R2FRIDA_DEBUG_URI=0|1            # Trace uri parsing code and exit before doing any action
  R2FRIDA_COMPILER_DISABLE=0|1     # Disable the new frida typescript compiler (`:. foo.ts`)
  R2FRIDA_AGENT_SCRIPT=[file]      # path to file of the r2frida agent

ERROR: Cannot open 'frida://?'
```

## Process Info

### Basic information about the app and environemnt
The `:i` commands are useful to check some basic information about the runtime:
```
[0x100610000]> :i
arch                arm
bits                64
os                  darwin
pid                 19347
uid                 0
objc                false
runtime             QJS
swift               false
java                false
mainLoop            false
pageSize            16384
pointerSize         8
modulename          arm_hello_ios
modulebase          0x10060c000
codeSigningPolicy   optional
isDebuggerAttached  false
cwd                 /private/var/root
[0x100610000]>
```

### Enumerating symbols
Here we can use `:is` to enumerate the symbols present in the process.
```
[0x100610000]> :is
0x10060c000 s _mh_execute_header
0x100610000 s main
0x0 u printf
0x0 u sleep
```

We can also enumerate imports using `:ii`:
```
[0x55d13c11061c]> :ii
0x7fa7d2170a4b f r_sys_getenv /home/hex/Tools/radare2/libr/util/libr_util.so
0x7fa7d1fd61e0 f read /usr/lib/x86_64-linux-gnu/libc-2.31.so
0x7fa7d204c230 f strncmp /usr/lib/x86_64-linux-gnu/libc-2.31.so
```

The same goes for exports using `:ie`:
```
[0x100610000]> :iE
0x10060c000 v _mh_execute_header
0x100610000 f main
```

### Enumerating loaded libraries
To view the libraries in memory, we can use `:il`, and we'll see some basic information such as their base address:
```
[0x100610000]> :il
0x000000010060c000 0x0000000100614000 arm_hello_ios
0x0000000101154000 0x00000001013fc000 substitute-loader.dylib
0x00000001be229000 0x00000001be22b000 libSystem.B.dylib
0x00000001db8c3000 0x00000001db8c9000 libcache.dylib
```

### Enumerating memory ranges

We can get virtual memory maps using `:dm`:
```
[0x1021d8058]> :dm
0x00000001021d4000 - 0x00000001021d8000 r-x /private/var/root/arm_hello_ios
0x00000001021d8000 - 0x00000001021dc000 r-x /private/var/root/arm_hello_ios
0x00000001021dc000 - 0x00000001021e0000 r-- /private/var/root/arm_hello_ios
0x00000001021e0000 - 0x00000001021e4000 r-- /private/var/root/arm_hello_ios
0x00000001021e4000 - 0x0000000102208000 r-- /usr/lib/libsubstrate.dylib
0x00000001022e4000 - 0x00000001022ec000 rw- /usr/lib/libsubstrate.dylib
0x00000001022ec000 - 0x00000001022f0000 r-- /usr/lib/libsubstrate.dylib
```

And we can get the full ranges using `:dmm`:
```
[0x1021d8058]> :dmm
0x00000001021d4000 - 0x00000001021e4000 r-x /private/var/root/arm_hello_ios
0x00000001021e4000 - 0x0000000102328000 rwx /usr/lib/libsubstrate.dylib
0x0000000102328000 - 0x0000000102360000 rwx /usr/lib/libsubstitute.dylib
0x0000000102360000 - 0x00000001024c0000 rwx /usr/lib/dyld
0x0000000102520000 - 0x0000000102ed0000 rwx /usr/lib/substitute-loader.dylib
0x0000000107d00000 - 0x000000018ae40000 rwx /System/Library/Caches/com.apple.dyld/dyld_shared_cache_arm64
```

## Enumerating Objective-C
### Classes
We can list the ObjC classes in memory using the `:icl` command:
```
[0x00000000]> :icl
Obfuscator
Challenge3
Challenge2
Challenge1
AppDelegate
```

