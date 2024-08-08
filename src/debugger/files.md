## Files

The radare2 debugger allows the user to list and manipulate the file descriptors from the target process.

This is a useful feature, which is not found in other debuggers, the functionality is similar to the lsof command line tool, but have extra subcommands to change the seek, close or duplicate them.

So, at any time in the debugging session you can replace the stdio file descriptors to use network sockets created by r2, or replace a network socket connection to hijack it.

This functionality is also available in r2frida by using the dd command prefixed with a backslash. In r2 you may want to see the output of dd? for proper details.

```console
[0x00000000]> dd?
Usage: dd  Manage file descriptors for child process (* to show r2 commands)
| dd[*]                      list file descriptors
| dd[*] <file|addr>          open file as read-only (r--); addr = use as char* for path
| dd+[*] <file|addr>         open/create file as read-write (rw-); addr = use as char* for path
| dd-[*] <fd>                close fd
| ddt[*]                     close terminal fd (alias for `dd- 0`)
| dds[*] <fd> [offset]       seek fd to offset (no offset = seek to beginning)
| ddd[*] <oldfd> <newfd>     copy oldfd to newfd with dup2
| ddf[*] <addr>              create pipe and write fds to (int[2])addr
| ddr[*] <fd> <addr> <size>  read bytes from fd into (char*)addr
| ddw[*] <fd> <addr> <size>  write bytes from (const char*)addr to fd
[0x00000000]>
```

### Tweaking descriptors

The dd command will use ragg2 internally to compile a shellcode that is then injected into the target process to manipulate the file descriptors.

For example if we want to open a file we can use this:

`dd /bin/ls`

We can also close that file with: `dd-4`
