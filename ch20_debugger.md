#Chapter 20: Debugger

The debugger in radare is implemented as an IO plugin. It handles two different URIs for creating or attaching to a process: dbg:// and pid://.

There are different backends for multiple architectures and operating systems like GNU/Linux, Windows, MacOSX, (Net,Free,Open)BSD and Solaris.

The process memory is interpreted by radare as a plain file. So all the mapped pages like the program and the libraries can be readed and interpreted as code, structures, etc..

The rest of the communication between radare and the debugger layer is the wrapped system() call that receives a string as argument and executes the given command. The result of the operation is buffered in the output console and this contents can be handled by a scripting language.

This is the reason why radare can handle single and double exclamation marks for calling system().

    [0x00000000]> ds
    [0x00000000]> !!ls
The double exclamation mark tells radare to skip the plugin list to find an IO plugin handling this command to launch it directly to the shell. A single one will walk through the io plugin list.

The debugger commands are mostly portable between architectures and operating systems. But radare tries to implement them on all the artchitectures and OSs injecting shellcodes, or handling exceptions in a special way. For example in mips there's no stepping feature by hardware, so radare has an own implementation using a mix of code analysis and software breakpoints to bypass this limitation.

To get the basic help of the debugger you can just type 'd?':

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


