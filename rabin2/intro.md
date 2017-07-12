# Rabin2 — Show Properties of a Binary

Under this bunny-arabic-like name, radare hides a powerful tool to handle binary files, to get information on imports, sections, headers etc. Rabin2 can present it in several formats accepted by other tools, including radare2 itself.
Rabin2 understands many file formats: Java CLASS, ELF, PE, Mach-O, etc., and it is able to obtain symbol import/exports, library dependencies, strings of data sections, xrefs,  entrypoint address, sections, architecture type.

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
     -E              globally exportable symbols
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
     -s              symbols
     -S              sections
     -v              use vaddr in radare output (or show version if no file)
     -x              extract bins contained in file
     -z              strings (from data section)
     -zz             strings (from raw bins [e bin.rawstr=1])
     -Z              guess size of binary program
