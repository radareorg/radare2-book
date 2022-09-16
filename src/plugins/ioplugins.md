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

An example of how to write a plugin is available in this commit: [here](https://github.com/radareorg/radare2/pull/20574/commits/f9abd3026c37961a485656447eff7bff616322bf).


##1)Write r2 IO plugin with Makefile (for Linux only)

To write an IO plugin in radare2, in `radare2/libr/io/p/`, create a file `dap.mk`. Put there:

```Makefile
OBJ_DAP=io_dap.o

STATIC_OBJ+=${OBJ_DAP}
TARGET_DAP=io_dap.${EXT_SO}
ALL_TARGETS+=${TARGET_DAP}

ifeq (${WITHPIC},0)
LINKFLAGS+=../../util/libr_util.a
LINKFLAGS+=../../io/libr_io.a
else
LINKFLAGS+=-L../../util -lr_util
LINKFLAGS+=-L.. -lr_io
endif

${TARGET_DAP}: ${OBJ_DAP}
	${CC} $(call libname,io_dap) ${OBJ_DAP} ${CFLAGS} \
		${LINKFLAGS} ${LDFLAGS_LIB} $(LDFLAGS)
```

Edit the file `radare2/libr/io/p/dap.c`:

```c
#include <r_userconf.h>
#include <r_io.h>
#include <r_lib.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "io_dap.h"

#define URI_PREFIX "foo://"

extern RIOPlugin r_io_plugin_dap; // forward declaration

static bool __plugin_open(RIO *io, const char *pathname, bool many) {
    return (strncmp(pathname, URI_PREFIX, strlen(URI_PREFIX)) == 0);
}

static RIODesc *__open(RIO *io, const char *pathname, int flags, int mode) {
    RIODesc *ret = NULL;
    RIOFoo *rio_foo = NULL;

    printf("%s\n", __func__);

    if (!__plugin_open(io, pathname, 0))
        return ret;

    return r_io_desc_new (io, &r_io_plugin_dap, pathname, flags, mode, rio_foo);
}

static int __close(RIODesc *fd) {
    RIOFoo *rio_foo = NULL;

    printf("%s\n", __func__);
    if (!fd || !fd->data)
        return -1;

    rio_foo = fd->data;
    // destroy
    return true;
}

static ut64 __lseek(RIO *io, RIODesc *fd, ut64 offset, int whence) {
    printf("%s, offset: %lx, io->off: %lx\n", __func__, offset, io->off);

    if (!fd || !fd->data)
        return -1;

    switch (whence) {
    case SEEK_SET:
        io->off = offset;
        break;
    case SEEK_CUR:
        io->off += (int)offset;
        break;
    case SEEK_END:
        io->off = UT64_MAX;
        break;
    }
    return io->off;
}

static int __read(RIO *io, RIODesc *fd, ut8 *buf, int len) {
    RIOFoo *rio_foo = NULL;

    printf("%s, offset: %lx\n", __func__, io->off);

    if (!fd || !fd->data)
        return -1;

    rio_foo = fd->data;

    return 0;
}

static int __write(RIO *io, RIODesc *fd, const ut8 *buf, int len) {
    printf("%s\n", __func__);

    return 0;
}

static int __getpid(RIODesc *fd) {
    RIOFoo *rio_foo = NULL;

    printf("%s\n", __func__);
    if (!fd || !fd->data)
        return -1;

    rio_foo = fd->data;
    return 0;
}

static int __gettid(RIODesc *fd) {
    printf("%s\n", __func__);
    return 0;
}

static char *__system(RIO *io, RIODesc *fd, const char *command) {
    printf("%s command: %s\n", __func__, command);
    // io->cb_printf()
    return NULL;
}

RIOPlugin r_io_plugin_dap = {
    .name = "dap",
    .desc = "IO Foo plugin",
    .license = "LGPL",
    .check = __plugin_open,
    .open = __open,
    .close = __close,
    .seek = __lseek,
    .read = __read,
    .write = __write,
    .getpid = __getpid,
    .system = __system,
    .isdbg = true,  // # --d flag
};

#ifndef R2_PLUGIN_INCORE
R_API RLibStruct radare_plugin = {
    .type = R_LIB_TYPE_IO,
    .data = &r_io_plugin_dap,
    .version = R2_VERSION
};
#endif
```

Do not forget to include your definition of `r_io_plugin_dap` in `libr/include/r_io.h`:
At this line: [here](https://github.com/radareorg/radare2/blob/f9abd3026c37961a485656447eff7bff616322bf/libr/include/r_io.h#L590), add:
```c
extern RIOPlugin r_io_plugin_dap;
```

At last but not least, for the Makefile (not for meson), in `dist/plugins-cfg/plugins.def.cfg`, add `io.dap` after `io.debug`.

##2)Write r2 plugin for radare2:

Edit `radare2/libr/io/meson.build` and in `r_io_sources = [...` add `'p/io_dap.c',`. And in `radare2/libr/meson.build`, add `'dap'` in `io_plugins += [`.

##3)Insert the plugin in radare2

Edit `radare2/dist/plugins-cfg/plugins.def.cfg` and add `io.dap` in the list.

##4)Add dap.h

Now, if you need to use some data and structures, edit the file `radare2/libr/io/p/dap.h` for:

```c
#ifndef LIBR_IO_P_IO_DAP_H_
#define LIBR_IO_P_IO_DAP_H_

typedef struct {
	int x;
} RIOFoo;

#endif /* LIBR_IO_P_IO_DAP_H_ */
```

This tutorial is based on [this one by Wensel](https://wenzel.github.io/2018/04/15/radare2-io-plugin-tutorial.html).
