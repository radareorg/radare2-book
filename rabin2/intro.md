# Rabin2

Under this bunny-arabic-like name, radare hides the power of a wonderful tool to handle binary files and get information to show it in the command line or import it into the core.

Rabin2 is able to handle multiple file formats like Java CLASS, ELF, PE, MACH-O, etc.. and it is able to get symbol import/exports, library dependencies, strings of data sections, xrefs, address of entrypoint, sections, architecture type, etc.

    $ rabin2 -h

    Usage: rabin2 [-ACdehHiIjlLMqrRsSvVxzZ] [-@ addr] [-a arch] [-b bits]
                  [-B addr] [-c F:C:D] [-f str] [-m addr] [-n str] [-N len]
                  [-o str] [-O str] file
     -@ [addr]       show section, symbol or import at addr
     -A              list archs
     -a [arch]       set arch (x86, arm, .. or <arch>_<bits>)
     -b [bits]       set bits (32, 64 ...)
     -B [addr]       override base address (pie bins)
     -c [fmt:C:D]    create [elf,mach0,pe] with Code and Data hexpairs (see -a)
     -C              list classes
     -d              show debug/dwarf information
     -e              entrypoint
     -f [str]        select sub-bin named str
     -g              same as -SMRevsiz (show all info)
     -h              this help
     -H              header fields
     -i              imports (symbols imported from libraries)
     -I              binary info
     -j              output in json
     -l              linked libraries
     -L              list supported bin plugins
     -m [addr]       show source line at addr
     -M              main (show address of main symbol)
     -n [str]        show section, symbol or import named str
     -N [minlen]     force minimum number of chars per string (see -z)
     -o [str]        output file/folder for write operations (out by default)
     -O [str]        write/extract operations (-O help)
     -q              be quiet, just show fewer data
     -r              radare output
     -R              relocations
     -s              symbols (exports)
     -S              sections
     -v              use vaddr in radare output (or show version if no file)
     -x              extract bins contained in file
     -z              strings (from data section)
     -zz             strings (from raw bins [e bin.rawstr=1])
     -Z              guess size of binary program
