## Rabin2

Under this bunny-arabic-like name, radare hides a powerful tool to handle binary files, to get information on imports, sections, headers and other data. Rabin2 can present it in several formats accepted by other tools, including radare2 itself.
Rabin2 understands many file formats: Java CLASS, ELF, PE, Mach-O or any format supported by plugins, and it is able to obtain symbol import/exports, library dependencies, strings of data sections, xrefs,  entrypoint address, sections, architecture type.

```
Usage: rabin2 [-AcdeEghHiIjlLMqrRsSUvVxzZ] [-@ at] [-a arch] [-b bits] [-B addr]
              [-C F:C:D] [-f str] [-m addr] [-n str] [-N m:M] [-P[-P] pdb]
              [-o str] [-O help] [-k query] [-D lang mangledsymbol] file
 -@ [addr]       show section, symbol or import at addr
 -A              list sub-binaries and their arch-bits pairs
 -a [arch]       set arch (x86, arm, .. or <arch>_<bits>)
 -b [bits]       set bits (32, 64 ...)
 -B [addr]       override base address (pie bins)
 -c              list classes
 -cc             list classes in header format
 -C [fmt:C:D]    create [elf,mach0,pe] with Code and Data hexpairs (see -a)
 -d              show debug/dwarf information
 -D lang name    demangle symbol name (-D all for bin.demangle=true)
 -e              program entrypoint
 -ee             constructor/destructor entrypoints
 -E              globally exportable symbols
 -f [str]        select sub-bin named str
 -F [binfmt]     force to use that bin plugin (ignore header check)
 -g              same as -SMZIHVResizcld -SS -SSS -ee (show all info)
 -G [addr]       load address . offset to header
 -h              this help message
 -H              header fields
 -i              imports (symbols imported from libraries)
 -I              binary info
 -j              output in json
 -k [sdb-query]  run sdb query. for example: '*'
 -K [algo]       calculate checksums (md5, sha1, ..)
 -l              linked libraries
 -L [plugin]     list supported bin plugins or plugin details
 -m [addr]       show source line at addr
 -M              main (show address of main symbol)
 -n [str]        show section, symbol or import named str
 -N [min:max]    force min:max number of chars per string (see -z and -zz)
 -o [str]        output file/folder for write operations (out by default)
 -O [str]        write/extract operations (-O help)
 -p              show always physical addresses
 -P              show debug/pdb information
 -PP             download pdb file for binary
 -q              be quiet, just show fewer data
 -qq             show less info (no offset/size for -z for ex.)
 -Q              show load address used by dlopen (non-aslr libs)
 -r              radare output
 -R              relocations
 -s              symbols
 -S              sections
 -SS             segments
 -SSS            sections mapping to segments
 -t              display file hashes
 -T              display file signature
 -u              unfiltered (no rename duplicated symbols/sections)
 -U              resoUrces
 -v              display version and quit
 -V              show binary version information
 -w              display try/catch blocks
 -x              extract bins contained in file
 -X [fmt] [f] .. package in fat or zip the given files and bins contained in file
 -z              strings (from data section)
 -zz             strings (from raw bins [e bin.str.raw=1])
 -zzz            dump raw strings to stdout (for huge files)
 -Z              guess size of binary program
Environment:
 R2_NOPLUGINS:     1|0|               # do not load shared plugins (speedup loading)
 RABIN2_ARGS:                         # ignore cli and use these program arguments
 RABIN2_CHARSET:   e cfg.charset      # set default value charset for -z strings
 RABIN2_DEBASE64:  e bin.str.debase64 # try to debase64 all strings
 RABIN2_DEMANGLE=0:e bin.demangle     # do not demangle symbols
 RABIN2_DMNGLRCMD: e bin.demanglercmd # try to purge false positives
 RABIN2_LANG:      e bin.lang         # assume lang for demangling
 RABIN2_MAXSTRBUF: e bin.str.maxbuf   # specify maximum buffer size
 RABIN2_PDBSERVER: e pdb.server       # use alternative PDB server
 RABIN2_PREFIX:    e bin.prefix       # prefix symbols/sections/relocs with a specific string
 RABIN2_STRFILTER: e bin.str.filter   # r2 -qc 'e bin.str.filter=??' -
 RABIN2_MACHO_NOFUNCSTARTS=0|1        # if set it will ignore the FUNCSTART information
 RABIN2_MACHO_NOSWIFT=0|1
 RABIN2_MACHO_SKIPFIXUPS=0|1
 RABIN2_CODESIGN_VERBOSE=0|1
 RABIN2_STRPURGE:  e bin.str.purge    # try to purge false positives
 RABIN2_SYMSTORE:  e pdb.symstore     # path to downstream symbol store
 RABIN2_SWIFTLIB:  1|0|               # load Swift libs to demangle (default: true)
 RABIN2_VERBOSE:   e bin.verbose      # show debugging messages from the parser
```
