## Command-line Options

The radare core accepts many flags from the command line.

This is an excerpt from the usage help message:
```
$ radare2 -h
Usage: r2 [-ACdfjLMnNqStuvwzX] [-P patch] [-p prj] [-a arch] [-b bits] [-c cmd]
          [-s addr] [-B baddr] [-m maddr] [-i script] [-e k=v] file|pid|-|--|=
 --           run radare2 without opening any file
 -            same as 'r2 malloc://512'
 =            read file from stdin (use -i and -c to run cmds)
 -=           perform !=! command to run all commands remotely
 -0           print \x00 after init and every command
 -2           close stderr file descriptor (silent warning messages)
 -a [arch]    set asm.arch
 -A           run 'aaa' command to analyze all referenced code
 -b [bits]    set asm.bits
 -B [baddr]   set base address for PIE binaries
 -c 'cmd..'   execute radare command
 -C           file is host:port (alias for -c+=http://%s/cmd/)
 -d           debug the executable 'file' or running process 'pid'
 -D [backend] enable debug mode (e cfg.debug=true)
 -e k=v       evaluate config var
 -f           block size = file size
 -F [binplug] force to use that rbin plugin
 -h, -hh      show help message, -hh for long
 -H ([var])   display variable
 -i [file]    run script file
 -I [file]    run script file before the file is opened
 -j           use json for -v, -L and maybe others
 -k [OS/kern] set asm.os (linux, macos, w32, netbsd, ...)
 -l [lib]     load plugin file
 -L, -LL      list supported IO plugins (-LL list core plugins)
 -m [addr]    map file at given address (loadaddr)
 -M           do not demangle symbol names
 -n, -nn      do not load RBin info (-nn only load bin structures)
 -N           do not load user settings and scripts
 -NN          do not load any script or plugin
 -q           quiet mode (no prompt) and quit after -i
 -qq          quit after running all -c and -i
 -Q           quiet mode (no prompt) and quit faster (quickLeak=true)
 -p [prj]     use project, list if no arg, load if no file
 -P [file]    apply rapatch file and quit
 -r [rarun2]  specify rarun2 profile to load (same as -e dbg.profile=X)
 -R [rr2rule] specify custom rarun2 directive
 -s [addr]    initial seek
 -S           start r2 in sandbox mode
 -t           load rabin2 info in thread
 -u           set bin.filter=false to get raw sym/sec/cls names
 -v, -V       show radare2 version (-V show lib versions)
 -w           open file in write mode
 -x           open without exec-flag (asm.emu will not work), See io.exec
 -X           same as -e bin.usextr=false (useful for dyldcache)
 -z, -zz      do not load strings or load them even in raw
```

### Common usages

At first sight it may seem like there are so many options and without some practical use cases it may feel a bit overwhelming, this sections aims to address that by sharing some of the most common ways to get started.

Open a file in write mode and do not parse the headers (raw mode).

```bash
$ r2 -nw file
```

Quickly get into the r2 shell opening a 1KB malloc virtual file, handy for testing things. note that a single dash is an alias for malloc://1024

```
$ r2 -
```

Specify which sub-binary you want to select when opening a fatbin file:

```
$ r2 -a ppc -b 32 ls.fat
```

Run a script before entering the prompt:

```
$ r2 -i patch.r2 target.bin
```

Execute a command and quit without entering the interactive mode:

```
$ r2 -qc ij hi.bin > imports.json
```

Set the configuration variable:

```
$ r2 -e scr.color=0 blah.bin
```

Spawn and start debugging a program:

```
$ r2 -d ls
```

Attach to an already running process by its process id (PID):

```
$ r2 -d 1234
```

Load an existing project file:

```
$ r2 -p test
```
