# Plugins

## IO plugins

IO plugins are the ones used to wrap the open, read, write and 'system' on virtual file systems.

The cool thing of IO plugins is that you can make radare understand that any thing can be handled as a plain file. A socket connection, a remote radare session, a file, a process, a device, a gdb session, etc..

So, when radare reads a block of bytes, is the task of the IO plugin to get these bytes from any place and put them in the internal buffer.

IO plugins are selected while opening a file by its URI. Here'r some examples:

Debugging URIs

    $ r2 dbg:///bin/ls
    $ r2 pid://1927

Remote sessions

    $ r2 rap://:1234
    $ r2 rap://<host>:1234//bin/ls

# Virtual buffers

    $ r2 malloc://512
    shortcut for
    $ r2 -

You can get a list of the radare IO plugins by typing 'radare -L':

    $ r2 -L
    rw_  zip         Open zip files apk://foo.apk//MANIFEST or zip://foo.apk//theclass/fun.class, show files with: zip://foo.apk/, open all files with zipall:// (BSD)
    rwd  windbg      Attach to a KD debugger (LGPL3)
    rw_  sparse      sparse buffer allocation (sparse://1024 sparse://) (LGPL3)
    rw_  shm         shared memory resources (shm://key) (LGPL3)
    rw_  self        read memory from myself using 'self://' (LGPL3)
    rw_  rap         radare network protocol (rap://:port rap://host:port/file) (LGPL3)
    rwd  ptrace      ptrace and /proc/pid/mem (if available) io (LGPL3)
    rw_  procpid     /proc/pid/mem io (LGPL3)
    rw_  mmap        open file using mmap:// (LGPL3)
    rw_  malloc      memory allocation (malloc://1024 hex://cd8090) (LGPL3)
    r__  mach        mach debug io (unsupported in this platform) (LGPL)
    rw_  ihex        Intel HEX file (ihex://eeproms.hex) (LGPL)
    rw_  http        http get (http://radare.org/) (LGPL3)
    rw_  gzip        read/write gzipped files (LGPL3)
    rwd  gdb         Attach to gdbserver, 'qemu -s', gdb://localhost:1234 (LGPL3)
    r_d  debug       Debug a program or pid. dbg:///bin/ls, dbg://1388 (LGPL3)
    rw_  bfdbg       BrainFuck Debugger (bfdbg://path/to/file) (LGPL3)
    