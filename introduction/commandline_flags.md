## Command-line Options

The radare core accepts many flags from command line.

An excerpt from usage help message:

    $ radare2 -h
    Usage: r2 [-dDwntLqv] [-P patch] [-p prj] [-a arch] [-b bits] [-i file] [-s addr] [-B blocksize] [-c cmd] [-e k=v] file|-

       -a [arch]    set asm.arch
       -A           run 'aa' command to analyze all referenced code
       -b [bits]    set asm.bits
       -B [baddr]   set base address for PIE binaries
       -c 'cmd..'   execute radare command
       -C           file is host:port (alias for -c+=http://%s/cmd/)
       -d           use 'file' as a program for debug
       -D [backend] enable debug mode (e cfg.debug=true)
       -e k=v       evaluate config var
       -f           block size = file size
       -h, -hh      show help message, -hh for long
       -i [file]    run script file
       -k [kernel]  set asm.os variable for asm and anal
       -l [lib]     load plugin file
       -L           list supported IO plugins
       -m [addr]    map file at given address
       -n           disable analysis
       -N           disable user settings
       -q           quiet mode (no promt) and quit after -i
       -p [prj]     set project file
       -P [file]    apply rapatch file and quit
       -s [addr]    initial seek
       -S           start r2 in sandbox mode
       -t           load rabin2 info in thread
       -v, -V       show radare2 version (-V show lib versions)
       -w           open file in write mode

Common usage patterns of  command-line options.

* Open a file in write mode without parsing the file format headers.

    $ r2 -nw file

* Quickly get into an r2 shell without opening any file.

    $ r2 -

Specify which sub-binary you want to select when opening a fatbin file:

    $ r2 -a ppc -b 32 ls.fat

Run a script before showing interactive command-line prompt:

    $ r2 -i patch.r2 target.bin

Execute a command and quit without entering the interactive mode:

    $ r2 -qc ij hi.bin > imports.json

Configure an eval variable:

    $ r2 -e scr.color=false blah.bin

Debug a program:

    $ r2 -d ls

Use an existing project file:

    $ r2 -p test

