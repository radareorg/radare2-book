# r2renef

r2renef is an IO plugin that merges the capabilities of radare2 and Renef, allowing you to perform powerful static analysis on live Android processes. It bridges radare2's comprehensive reverse engineering toolkit with Renef's dynamic instrumentation engine for ARM64 Android applications.

With r2renef you can use standard radare2 commands like `pd`, `px`, `pdf`,`w` and `s` directly on running Android processes. The plugin provides seamless integration between radare2's analysis capabilities and Renef's runtime manipulation features, enabling real-time memory inspection, function hooking, and Lua scripting.

Some of the most relevant features include:

* Live memory analysis: Read/write process memory through radare2
* Full r2 integration: Use `px`, `pd`, `pf`,`w` commands on live processes
* Renef commands: Access Renef API with `:` prefix commands
* Memory operations: Dump and scan memory with `:md` and `:ms`
* Hook support: Load Lua scripts (`l`), manage hooks with `:hooks` and `:unhook`
* Real-time monitoring: Watch hook callbacks with `:watch`

## Installation

### Requirements

* Radare2 (version 5.8.0 or newer)
* Renef running on the Android device
* ARM64 Android device with root access
* ADB (Android Debug Bridge)

### Building from Source

Clone the repository and build:

```console
$ git clone https://github.com/ahmeth4n/r2renef
$ cd r2renef
$ make
$ make install
```

Alternatively, using Meson build system:

```console
$ meson setup build
$ ninja -C build
$ ninja -C build install
```

### Verify Installation

Check that the plugin is loaded:

```console
$ r2 -L | grep renef
rw_ renef Renef IO plugin for radare2
```

## Connecting to a Process

Before using r2renef, ensure Renef is running on your Android device and configure ADB port forwarding:

```console
$ adb forward tcp:1907 localabstract:com.android.internal.os.RuntimeInit;
```

The URI handler supports two modes:

**Spawn a new process:**

```console
$ r2 renef://spawn/com.example.app
```

**Attach to running process by PID:**

```console
$ r2 renef://attach/12345
```
