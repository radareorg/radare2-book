# Files

The radare2 debugger allows the user to list and manipulate the filedescriptors from the target process.

This is a useful feature, which is not found in other debuggers, the functionality is similar to the lsof command line tool.

But have extra subcommands to change the seek, close them, duplicate, etc,

So, at any time in the debugging session you can replace the stdio filedescriptors to use network sockets created by r2, or replace a network socket connection to hijack it.

This functionality is also available in r2frida thru by using the dd command prefixed with a backslash. In r2 you may want to see the output of dd? for proper details.