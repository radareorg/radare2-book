# Debugger

Debuggers are implemented as IO plugins. Therefore, radare can handle different URI types for spawning, attaching and controlling processes. The complete list of IO plugins can be viewed with `r2 -L`. Those that have "d" in the first column ("rwd") support debugging. For example:

    r_d  debug       Debug a program or pid. dbg:///bin/ls, dbg://1388 (LGPL3)
    rwd  gdb         Attach to gdbserver, 'qemu -s', gdb://localhost:1234 (LGPL3)

There are different backends for many target architectures and operating systems, e.g., GNU/Linux, Windows, MacOS X, (Net,Free,Open)BSD and Solaris.

A process memory is treated as a plain file. All mapped memory pages of a debugged program and its libraries can be readed and interpreted as code, data structures etc.

Communication between radare and debugger IO layer is wrapped into `system()` calls, which accepts a string as an argument, and executes it as a command. An answer is then buffered in output console, its contents can be additionally processed by a script. This is how radare handles single `!` and double `!!` exclamation mark commands for calling `system()`:

    [0x00000000]> ds
    [0x00000000]> !!ls
The double exclamation mark `!!` tells radare to skip the IO plugin list, and to pass the rest of the command directly to shell. Using the single `!` to prepend a command will cause a walk through the IO plugin list to find one that handles it.

In general, debugger commands are portable between architectures and operating systems. Still, as radare tries to support the same functionality for all target architectures and operating systems, certain things have to be handled separately. They include injecting shellcodes and handling exceptions. For example, in MIPS targets there is no hardware-supported single-stepping feature. In this case, radare2 provides its own implementation for single-step by using a mix of code analysis and software breakpoints.

To get the basic help for debugger, type 'd?':

    Usage: d[sbhcrbo] [arg]
    dh [handler]   list or set debugger handler
    dH [handler]   transplant process to a new handler
    dd             file descriptors (!fd in r1)
    ds[ol] N       step, over, source line
    do             open process (reload, alias for 'oo')
    dk [sig][=act] list, send, get, set, signal handlers of child
    di[s] [arg..]  inject code on running process and execute it (See gs)
    dp[=*?t][pid]  list, attach to process or thread id
    dc[?]          continue execution. dc? for more
    dr[?]          cpu registers, dr? for extended help
    db[?]          breakpoints
    dbt            display backtrace
    dt[?r] [tag]   display instruction traces (dtr=reset)
    dm[?*]         show memory maps
    dw [pid]       block prompt until pid dies

To restart your debugging session, you can type `oo` or `oo+`, depending on desired behavior.

    oo                 reopen current file (kill+fork in debugger)
    oo+                reopen current file in read-write
    
