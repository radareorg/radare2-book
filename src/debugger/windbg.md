# WinDBG Kernel-mode Debugging (KD)

The WinDBG KD interface support for r2 allows you to attach to VM running
Windows and debug its kernel over a serial port or network.

It is also possible to use the remote GDB interface to connect and
debug Windows kernels without depending on Windows capabilities.

Bear in mind that WinDBG KD support is still work-in-progress, and this is
just an initial implementation which will get better in time.

## Setting Up KD on Windows

> For a complete walkthrough, refer to Microsoft's [documentation](https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/setting-up-kernel-mode-debugging-in-windbg--cdb--or-ntsd).

### Serial Port
Enable KD over a serial port on Windows Vista and higher like this:

```
bcdedit /debug on
bcdedit /dbgsettings serial debugport:1 baudrate:115200
```

Or like this for Windows XP:
	Open boot.ini and add /debug /debugport=COM1 /baudrate=115200:
```
[boot loader]
timeout=30
default=multi(0)disk(0)rdisk(0)partition(1)\WINDOWS
[operating systems]
multi(0)disk(0)rdisk(0)partition(1)\WINDOWS="Debugging with Cable" /fastdetect /debug /debugport=COM1 /baudrate=57600
```
In case of VMWare
```
	Virtual Machine Settings -> Add -> Serial Port
	Device Status:
	[v] Connect at power on
	Connection:
	[v] Use socket (named pipe)
	[_/tmp/winkd.pipe________]
	From: Server To: Virtual Machine
```
Configure the VirtualBox Machine like this:
```
    Preferences -> Serial Ports -> Port 1

    [v] Enable Serial Port
    Port Number: [_COM1_______[v]]
    Port Mode:   [_Host_Pipe__[v]]
                 [v] Create Pipe
    Port/File Path: [_/tmp/winkd.pipe____]
```
Or just spawn the VM with qemu like this:
```
$ qemu-system-x86_64 -chardev socket,id=serial0,\
     path=/tmp/winkd.pipe,nowait,server \
     -serial chardev:serial0 -hda Windows7-VM.vdi
```

### Network
Enable KD over network (KDNet) on Windows 7 or later likes this:
```
bcdedit /debug on
bcdedit /dbgsettings net hostip:w.x.y.z port:n
```
Starting from Windows 8 there is no way to enforce debugging
for every boot, but it is possible to always show the advanced boot options,
which allows to enable kernel debugging:
```
bcedit /set {globalsettings} advancedoptions true
```

## Connecting to KD interface on r2

### Serial Port
Radare2 will use the `winkd` io plugin to connect to a socket file
created by virtualbox or qemu. Also, the `winkd` debugger plugin and
we should specify the x86-32 too. (32 and 64 bit debugging is supported)
```
$ r2 -a x86 -b 32 -D winkd winkd:///tmp/winkd.pipe
```

On Windows you should run the following line:
```
$ radare2 -D winkd winkd://\\.\pipe\com_1
```

### Network
```
$ r2 -a x86 -b 32 -d winkd://<hostip>:<port>:w.x.y.z
```

## Using KD
When connecting to a KD interface, r2 will send a breakin packet to interrupt
the target and we will get stuck here:
```
[0x828997b8]> pd 20
	;-- eip:
	0x828997b8    cc           int3
	0x828997b9    c20400       ret 4
	0x828997bc    cc           int3
	0x828997bd    90           nop
	0x828997be    c3           ret
    0x828997bf    90           nop
```

In order to skip that trap we will need to change eip and run 'dc' twice:
```
dr eip=eip+1
dc
dr eip=eip+1
dc
```
Now the Windows VM will be interactive again. We will need to kill r2 and
attach again to get back to control the kernel.

In addition, the `dp` command can be used to list all processes, and
`dpa` or `dp=` to attach to the process. This will display the base
address of the process in the physical memory layout.

# WinDBG Backend for Windows (DbgEng)

On Windows, radare2 can use `DbgEng.dll` as a debugging backend,
allowing it to make use of WinDBG's capabilities, supporting dump files,
local and remote user and kernel mode debugging.

You can use the debugging DLLs included on Windows or get the latest version from Microsoft's [download page](https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/debugger-download-tools) (recommended).

> You cannot use DLLs from the Microsoft Store's `WinDbg Preview` app folder directly as they are not marked as executable for normal users.

> radare2 will try to load `dbgeng.dll` from the `_NT_DEBUGGER_EXTENSION_PATH` environment variable before using Windows' default library search path.

## Using the plugin

To use the `windbg` plugin, pass the same command-line options as you would for `WinDBG` or `kd` (see Microsoft's [documentation](https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/windbg-command-line-options)), quoting/escaping when necessary:

```
> r2 -d "windbg://-remote tcp:server=Server,port=Socket"
```
```
> r2 -d "windbg://MyProgram.exe \"my arg\""
```
```
> r2 -d "windbg://-k net:port=<n>,key=<MyKey>"
```
```
> r2 -d "windbg://-z MyDumpFile.dmp"
```

You can then debug normally (see `d?` command) or interact with the backend shell directly with the `=!` command:

```
[0x7ffcac9fcea0]> dcu 0x0007ffc98f42190
Continue until 0x7ffc98f42190 using 1 bpsize
ModLoad: 00007ffc`ab6b0000 00007ffc`ab6e0000   C:\WINDOWS\System32\IMM32.DLL
Breakpoint 1 hit
hit breakpoint at: 0x7ffc98f42190

[0x7fffcf232190]> =!k4
Child-SP          RetAddr           Call Site
00000033`73b1f618 00007ff6`c67a861d r_main!r_main_radare2
00000033`73b1f620 00007ff6`c67d0019 radare2!main+0x8d
00000033`73b1f720 00007ff6`c67cfebe radare2!invoke_main+0x39
00000033`73b1f770 00007ff6`c67cfd7e radare2!__scrt_common_main_seh+0x12e
```
