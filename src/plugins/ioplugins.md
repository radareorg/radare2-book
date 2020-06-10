# IO plugins

All access to files, network, debugger and all input/output in general is wrapped by an IO abstraction layer that allows radare to treat all data as if it were just a file.

IO plugins are the ones used to wrap the open, read, write and 'system' on virtual file systems. You can make radare understand anything as a plain file. E.g. a socket connection, a remote radare session, a file, a process, a device, a gdb session.

So, when radare reads a block of bytes, it is the task of an IO plugin to get these bytes from any place and put them into internal buffer. An IO plugin is chosen by a file's URI to be opened. Some examples:

* Debugging URIs
```
$ r2 dbg:///bin/ls<br />
$ r2 pid://1927
```
* Remote sessions
```
$ r2 rap://:1234<br />
$ r2 rap://<host>:1234//bin/ls
```
* Virtual buffers
```
$ r2 malloc://512<br />
shortcut for
$ r2 -
```
You can get a list of the radare IO plugins by typing `radare2 -L`:
```
$ r2 -L
rw_  ar       Open ar/lib files [ar|lib]://[file//path] (LGPL3)
rw_  bfdbg    BrainFuck Debugger (bfdbg://path/to/file) (LGPL3)
rwd  bochs    Attach to a BOCHS debugger (LGPL3)
r_d  debug    Native debugger (dbg:///bin/ls dbg://1388 pidof:// waitfor://) (LGPL3) v0.2.0 pancake
rw_  default  open local files using def_mmap:// (LGPL3)
rwd  gdb      Attach to gdbserver, 'qemu -s', gdb://localhost:1234 (LGPL3)
rw_  gprobe   open gprobe connection using gprobe:// (LGPL3)
rw_  gzip     read/write gzipped files (LGPL3)
rw_  http     http get (http://rada.re/) (LGPL3)
rw_  ihex     Intel HEX file (ihex://eeproms.hex) (LGPL)
r__  mach     mach debug io (unsupported in this platform) (LGPL)
rw_  malloc   memory allocation (malloc://1024 hex://cd8090) (LGPL3)
rw_  mmap     open file using mmap:// (LGPL3)
rw_  null     null-plugin (null://23) (LGPL3)
rw_  procpid  /proc/pid/mem io (LGPL3)
rwd  ptrace   ptrace and /proc/pid/mem (if available) io (LGPL3)
rwd  qnx      Attach to QNX pdebug instance, qnx://host:1234 (LGPL3)
rw_  r2k      kernel access API io (r2k://) (LGPL3)
rw_  r2pipe   r2pipe io plugin (MIT)
rw_  r2web    r2web io client (r2web://cloud.rada.re/cmd/) (LGPL3)
rw_  rap      radare network protocol (rap://:port rap://host:port/file) (LGPL3)
rw_  rbuf     RBuffer IO plugin: rbuf:// (LGPL)
rw_  self     read memory from myself using 'self://' (LGPL3)
rw_  shm      shared memory resources (shm://key) (LGPL3)
rw_  sparse   sparse buffer allocation (sparse://1024 sparse://) (LGPL3)
rw_  tcp      load files via TCP (listen or connect) (LGPL3)
rwd  windbg   Attach to a KD debugger (windbg://socket) (LGPL3)
rwd  winedbg  Wine-dbg io and debug.io plugin for r2 (MIT)
rw_  zip      Open zip files [apk|ipa|zip|zipall]://[file//path] (BSD)
```
