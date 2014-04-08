# Chapter 2: Configuration

The core reads ~/.radare2rc while starting, so you can setup there some `e` commands to set it up in your favorite way.

To avoid parsing this file, use `-n` and to get a cleaner output for using radare in batch mode maybe is better to just drop the verbosity with `-v`.

All the configuration of radare is done with the `eval` command which allows the user to change some variables from an internal hashtable containing string pairs.

The most common configuration looks like this:

    $ cat ~/.radarerc
    e scr.color = true
    e dbg.bep   = loader
    
    
These configurations can be also defined using the `-e` flag of radare while loading it, so you can setup different initial configurations from the commandline without having to change to rc file.

    $ radare -n -e scr.color=true -e asm.syntax=intel -d /bin/ls
    
All the configuration is stored in a hash table grouped by different root names (cfg., file., dbg., ..)

To get a list of the configuration variables just type `e` in the prompt. All the basic commands can be reduced to a single char. You can also list the configuration variables of a single eval configruation group ending the command argument with a dot `.`.

There are two enhanced interfaces to help users to interactively configure this hashtable. One is called `emenu` and provides a shell for walking through the tree and change variables.

To get a help about this command you can type `e?`:

    Usage: e[?] [var[=value]]
    e?              show this help
    e?asm.bytes     show description
    e??             list config vars with description
    e               list config vars
    e-              reset config vars
    e*              dump config vars in r commands
    e!a             invert the boolean value of 'a' var
    er [key]        set config key as readonly. no way back
    ec [k] [color]  set color for given key (prompt, offset, ...)
    e a             get value of var 'a'
    e a=b           set var 'a' the 'b' value
    env [k[=v]]     get/set environment variable

       
There is a easier `e` interface accessible from the Visual mode, just typing `Ve` after entering this mode.

    Eval spaces:                                                                   
    
    >  anal                                                                        
       asm                                                                         
       scr                                                                         
       asm                                                                         
       bin                                                                         
       cfg                                                                         
       diff                                                                        
       dir                                                                         
       dbg                                                                         
       cmd                                                                         
       fs                                                                          
       hex                                                                         
       http                                                                        
       graph                                                                       
       hud                                                                         
       scr                                                                         
       search                                                                      
       io                                                                          
   


Most of the eval tree is quite stable, so don't expect hard changes on this area.

I encourage you to experiment a bit on this to fit the interface to your needs.


## 2.1 Colors

The console access is wrapped by an API that permits to show the output of any command as ANSI, w32 console or HTML (more to come ncurses, pango, ...) this allows the core to be flexible enought to run on limited environments like kernels or embedded devices allowing us to get the feedback from the application in our favourite format.

To start, we'll enable the colors by default in our rc file:

    $ echo 'e scr.color=true' >> ~/.radare2rc
You can configure the colors to be used in almost every element in your disassembly.
r2 supports rgb colors in unix terminals and allows to change the console color paletes using the `ec` command.

Type `ec` to get a list of all the palete elements.
Type `ecs` to show a color palete to pick colors from:

![img](http://lolcathost.org/b/r2pal.png)

#### xvilka theme


    ec fname rgb:0cf
    ec label rgb:0f3
    ec math rgb:660
    ec bin rgb:f90
    ec call rgb:f00
    ec jmp rgb:03f
    ec cjmp rgb:33c
    ec offset rgb:366
    ec comment rgb:0cf
    ec push rgb:0c0
    ec pop rgb:0c0
    ec cmp rgb:060
    ec nop rgb:000
    ec b0x00 rgb:444
    ec b0x7f rgb:555
    ec b0xff rgb:666
    ec btext rgb:777
    ec other rgb:bbb
    ec num rgb:f03
    ec reg rgb:6f0
    ec fline rgb:fc0
    ec flow rgb:0f0

![img](http://xvilka.me/r2-rainbow.png)

## 2.2 Common configuration variables

Here's a list of the most common eval configuration variables, you can get the complete list using the `e` command without arguments or just use `e cfg.` (ending with dot, to list all the configuration variables of the cfg. space). You can get help on any eval configuration variable using : `??e cfg.` for example

    asm.arch
Defines the architecture to be used while disassembling (pd, pD commands) and analyzing code (`a` command). Currently it handles `intel32`, `intel64`, `mips`, `arm16`, `arm` `java`, `csr`, `sparc`, `ppc`, `msil` and `m68k`.

It is quite simple to add new architectures for disassembling and analyzing code, so there is an interface adapted for the GNU disassembler and others for udis86 or handmade ones.

    asm.bits
This variable will change the `asm.arch` one (in radare1) and viceversa (is determined by asm.arch). It determines the size in bits of the registers for the selected architecture. This is 8, 16, 32, 64.

    asm.syntax
Defines the syntax flavour to be used while disassembling. This is currently only targeting the udis86 disassembler for the x86 (32/64 bits). The supported values are `intel` or `att`.

    asm.pseudo
Boolean value that determines which string disassembly engine to use (the native one defined by the architecture) or the one filtered to show pseudocode strings. This is `eax=ebx` instead of a `mov eax, ebx` for example.

    asm.os
Defines the target operating system of the binary to analyze. This is automatically defined by `rabin -rI` and it's useful for switching between the different syscall tables and perform different depending on the OS.

    asm.flags
If defined to `true` shows the flags column inside the disassembly.

    asm.linescall
Draw lines at the left of the offset in the dissassemble print format (pd, pD) to graphically represent jumps and calls inside the current block.

    asm.linesout
When defined as `true`, also draws the jump lines in the current block that goes ouside of this block.

    asm.linestyle
Can get `true` or `false` values and makes the line analysis be performed from top to bottom if false or bottom to top if true. `false` is the optimal and default value for readability.

    asm.offset
Boolean value that shows or hides the offset address of the disassembled opcode.

    asm.profile
Set how much information is showed to the user on disassembly. Can get the values `default`, `simple`, `gas`, `smart`, `debug`, `full`.

This eval will modify other asm. variables to change the visualization properties for the disassembler engine. `simple` asm.profile will show only offset+opcode, and `debug` will show information about traced opcodes, stack pointer delta, etc..

    asm.trace
Show tracing information at the left of each opcode (sequence number and counter). This is useful to read execution traces of programs.

    asm.bytes
Boolean value that shows or hides the bytes of the disassembled opcode.

    cfg.bigendian
Choose the endian flavour `true` for big, `false` for little.

    file.analyze
Runs `.af* @@ sym.` and `.af* @ entrypoint`, after resolving the symbols while loading the binary, to determine the maximum information about the code analysis of the program. This will not be used while opening a project file, so it is preloaded. This option requires file.id and file.flag to be true.

    scr.color
This boolean variable allows to enable or disable the colorized output

    scr.seek
This variable accepts an expression, a pointer (eg. eip), etc. radare will automatically seek to make sure its value is always within the limits of the screen.

    cfg.fortunes
Enables or disables the 'fortune' message at the begining of the program
