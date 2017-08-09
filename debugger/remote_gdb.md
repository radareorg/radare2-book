# Debugging with gdbserver

radare2 allows remote debugging over the gdb remote protocol. So you can run a
gdbserver and connect to it with radare2 for remote debugging. The syntax for
connecting is:

    $ r2 -D gdb gdb://<host>:<port>

Or if the gdbserver is running in extended mode, you can attach to a process on
the host with:

    $ r2 -D gdb gdb://<host>:<port>/<pid>

After connecting, debugging works as normal.

radare2 does not yet load symbols from gdbserver, so it needs the binary to
be locally present to load symbols from it. In case symbols are not loaded even
if the binary is present, you can try specifying the path with `e dbg.exe.path`:

    $ r2 -e dbg.exe.path=<path> -D gdb gdb://<host>:<port>

If symbols are loaded at an incorrect base address, you can try specifying
the base address too with `e bin.baddr`:

    $ r2 -e bin.baddr=<baddr> -e dbg.exe.path=<path> -D gdb gdb://<host>:<port>

Usually the gdbserver reports the maximum packet size it supports. Otherwise,
radare2 resorts to sensible defaults. But you can specify the maximum packet
size with the environment variable `R2_GDB_PKTSZ`. You can also check and set
the max packet size during a session with the IO system, `=!`.

    $ export R2_GDB_PKTSZ=512
    $ r2 -D gdb gdb://<host>:<port>
    = attach <pid> <tid>
    Assuming filepath <path/to/exe>
    [0x7ff659d9fcc0]> =!pktsz
    packet size: 512 bytes
    [0x7ff659d9fcc0]> =!pktsz 64
    [0x7ff659d9fcc0]> =!pktsz
    packet size: 64 bytes

The gdb IO system provides useful commands which might not fit into any
standard radare2 commands. You can get a list of these commands with
`=!?`. (Remember, `=!` accesses the underlying IO plugin's `system()`).

    [0x7ff659d9fcc0]> =!?
    Usage: =!cmd args
     =!pid             - show targeted pid
     =!pkt s           - send packet 's'
     =!monitor cmd     - hex-encode monitor command and pass to target interpreter
     =!detach [pid]    - detach from remote/detach specific pid
     =!inv.reg         - invalidate reg cache
     =!pktsz           - get max packet size used
     =!pktsz bytes     - set max. packet size as 'bytes' bytes
     =!exec_file [pid] - get file which was executed for current/specified pid

radare2 also provides its own gdbserver implementation:

    $ r2 -
    [0x00000000]> =?
    ...
    gdbserver:
    | =g port file [args]  listen on 'port' debugging 'file' using gdbserver
    | =g! port file [args] same as above, but debug protocol messages (like gdbserver --remote-debug)

So you can start it as:

    $ r2 -
    [0x00000000]> =g 8000 /bin/radare2 -

And then connect to it like you would to any gdbserver. For example, with radare2:

    $ r2 -D gdb gdb://localhost:8000