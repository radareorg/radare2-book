# Reverse Debugging

Radare2 has reverse debugger, that can seek program counter backward.
(e.g. reverse-next, reverse-continue in gdb)
Firstly you need to save program state at the point that you want to start recording.
The syntax for recording is:

    [0x004028a0]> dts+

You can use `dts` commands for recording and managing program states.
After recording the states, you can seek pc back and forth to any points after saved address.
So after recording, you can try single step back:

    [0x004028a0]> 2dso
    [0x004028a0]> dr rip
    0x004028ae
    [0x004028a0]> dsb
    continue until 0x004028a2
    hit breakpoint at: 4028a2
    [0x004028a0]> dr rip
    0x004028a2

When you run `dsb`, reverse debugger restore previous recorded state and execute program from it
until desired point.

Or you can also try continue back:

    [0x004028a0]> db 0x004028a2
    [0x004028a0]> 10dso
    [0x004028a0]> dr rip
    0x004028b9
    [0x004028a0]> dcb
    [0x004028a0]> dr rip
    0x004028a2

`dcb` seek program counter until hit the latest breakpoint.

You can see current recorded program states by `dts`:

    [0x004028a0]> dts
    session: 0   at:0x004028a0   ""
    session: 1   at:0x004028c2   ""

And also can add comment:

    [0x004028c2]> dtsC 0 program start
    [0x004028c2]> dtsC 1 decryption start
    [0x004028c2]> dts
    session: 0   at:0x004028a0   "program start"
    session: 1   at:0x004028c2   "decryption start"

Export/Import records to/from file:

    [0x004028c2]> dtst records_for_test
    Session saved in records_for_test.session and dump in records_for_test.dump
    [0x004028c2]> dtsf records_for_test
    session: 0, 0x4028a0 diffs: 0
    session: 1, 0x4028c2 diffs: 0
