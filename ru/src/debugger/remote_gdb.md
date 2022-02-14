# Debugging with gdbserver

radare2 allows remote debugging over the gdb remote protocol. So you can run a
gdbserver and connect to it with radare2 for remote debugging. The syntax for
connecting is:

```
$ r2 -d gdb://<host>:<port>
```

Note that the following command does the same, r2 will use the debug plugin specified by the uri if found.

```
$ r2 -D gdb gdb://<host>:<port>
```

The debug plugin can be changed at runtime using the dL or Ld commands.

Or if the gdbserver is running in extended mode, you can attach to a process on
the host with:

```
$ r2 -d gdb://<host>:<port>/<pid>
```

It is also possible to start debugging after analyzing a file using the `doof` command
which rebases the current session's data after opening gdb

```
[0x00404870]> doof gdb://<host>:<port>/<pid>
```

After connecting, you can use the standard r2 debug commands as normal.

radare2 does not yet load symbols from gdbserver, so it needs the binary to
be locally present to load symbols from it. In case symbols are not loaded even
if the binary is present, you can try specifying the path with `e dbg.exe.path`:

```
$ r2 -e dbg.exe.path=<path> -d gdb://<host>:<port>
```

If symbols are loaded at an incorrect base address, you can try specifying
the base address too with `e bin.baddr`:

```
$ r2 -e bin.baddr=<baddr> -e dbg.exe.path=<path> -d gdb://<host>:<port>
```

Usually the gdbserver reports the maximum packet size it supports. Otherwise,
radare2 resorts to sensible defaults. But you can specify the maximum packet
size with the environment variable `R2_GDB_PKTSZ`. You can also check and set
the max packet size during a session with the IO system, `=!`.

```
$ export R2_GDB_PKTSZ=512
$ r2 -d gdb://<host>:<port>
= attach <pid> <tid>
Assuming filepath <path/to/exe>
[0x7ff659d9fcc0]> =!pktsz
packet size: 512 bytes
[0x7ff659d9fcc0]> =!pktsz 64
[0x7ff659d9fcc0]> =!pktsz
packet size: 64 bytes
```

The gdb IO system provides useful commands which might not fit into any
standard radare2 commands. You can get a list of these commands with
`=!?`. (Remember, `=!` accesses the underlying IO plugin's `system()`).

```
[0x7ff659d9fcc0]> =!?
Usage: =!cmd args
 =!pid             - show targeted pid
 =!pkt s           - send packet 's'
 =!monitor cmd     - hex-encode monitor command and pass to target interpreter
 =!rd              - show reverse debugging availability
 =!dsb             - step backwards
 =!dcb             - continue backwards
 =!detach [pid]    - detach from remote/detach specific pid
 =!inv.reg         - invalidate reg cache
 =!pktsz           - get max packet size used
 =!pktsz bytes     - set max. packet size as 'bytes' bytes
 =!exec_file [pid] - get file which was executed for current/specified pid
```

Note that `=!dsb` and `=!dcb` are only available in special gdbserver implementations such
as [Mozilla's rr](https://github.com/mozilla/rr), the default gdbserver doesn't include
remote reverse debugging support.
Use `=!rd` to print the currently available reverse debugging capabilities.

If you are interested in debugging radare2's interaction with gdbserver you can use
`=!monitor set remote-debug 1` to turn on logging of gdb's remote protocol packets in
gdbserver's console and `=!monitor set debug 1` to show general debug messages from
gdbserver in it's console.

radare2 also provides its own gdbserver implementation:

```
$ r2 -
[0x00000000]> =g?
|Usage:  =[g] [...] # gdb server
| gdbserver:
| =g port file [args]   listen on 'port' debugging 'file' using gdbserver
| =g! port file [args]  same as above, but debug protocol messages (like gdbserver --remote-debug)
```

So you can start it as:

```
$ r2 -
[0x00000000]> =g 8000 /bin/radare2 -
```

And then connect to it like you would to any gdbserver. For example, with radare2:

```
$ r2 -d gdb://localhost:8000
```

