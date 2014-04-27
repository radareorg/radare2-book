## Basic debugger session

To start debugging a program use the `-d` flag and append the PID or the program path with arguments.

    $ r2 -d /bin/ls

The debugger will fork and load the `ls` program in memory stopping the execution in the `ld.so`, so don't expect to see the entrypoint or the mapped libraries at this point. To change this you can define a new 'break entry point' adding `e dbg.bep=entry` or `dbg.bep=main` to your .radarerc.

But take care on this, because some malware or programs can execute code before the main.

Now the debugger prompt should appear and if you press `enter` ( null command ) the basic view of the process will be displayed with the stack dump, general purpose registers and disassembly from current program counter (eip on intel).


Here's a list of the most common commands for the debugger:

    > d?          ; get help on debugger commands
    > ds 3        ; step 3 times
    > db 0x8048920  ; setup a breakpoint
    > db -0x8048920 ; remove a breakpoint
    > dc          ; continue process execution
    > dcs        ; continue until syscall
    > dd            ; manipulate file descriptors
    > dm          ; show process maps
    > dmp A S rwx  ; change page at A with size S protection permissions
    > dr eax=33 ; set register value. eax = 33

The easiest way to use the debugger is from the Visual mode. That way you will not need to remember many commands nor keep states in your mind.

    [0xB7F0C8C0]> V

After entering this command an hexdump of the current eip will be showed. Now press `p` one time to get into the debugger view. You can press `p` and `P` to rotate thru the most commonly used print modes.

Use F7 or `s` to step into and F8 or `S` to step over.

With the `c` key you will toggle the cursor mode and being able to select range of bytes to nop them or set breakpoints using the `F2` key.

In the visual mode you can enter commands with `:` to dump buffer contents like

    x @ esi

To get the help in the visual mode press `?`.

At this point the most common commands are !reg that can be used to get or set values for the general purpose registers. You can also manipulate the hardware and extended/floating registers. 
