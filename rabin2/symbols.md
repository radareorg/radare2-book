## Symbols (Exports)

With rabin2, the generated symbols list format is similar to the imports list. Use the `-s` option to get it:

    $ rabin2 -s /bin/ls | head
    [Symbols]
    addr=0x0021a610 off=0x0021a610 ord=114 fwd=NONE sz=8 bind=GLOBAL type=OBJECT name=stdout
    addr=0x0021a600 off=0x0021a600 ord=115 fwd=NONE sz=0 bind=GLOBAL type=NOTYPE name=_edata
    addr=0x0021b388 off=0x0021b388 ord=116 fwd=NONE sz=0 bind=GLOBAL type=NOTYPE name=_end
    addr=0x0021a600 off=0x0021a600 ord=117 fwd=NONE sz=8 bind=GLOBAL type=OBJECT name=__progname
    addr=0x0021a630 off=0x0021a630 ord=119 fwd=NONE sz=8 bind=UNKNOWN type=OBJECT name=program_invocation_name
    addr=0x0021a600 off=0x0021a600 ord=121 fwd=NONE sz=0 bind=GLOBAL type=NOTYPE name=__bss_start
    addr=0x0021a630 off=0x0021a630 ord=122 fwd=NONE sz=8 bind=GLOBAL type=OBJECT name=__progname_full
    addr=0x0021a600 off=0x0021a600 ord=123 fwd=NONE sz=8 bind=UNKNOWN type=OBJECT name=program_invocation_short_name
    addr=0x00002178 off=0x00002178 ord=124 fwd=NONE sz=0 bind=GLOBAL type=FUNC name=_init

With the `-sr` option rabin2 produces a radare2 script instead. It can later be passed to the core to automatically flag all symbols and to define corresponding byte ranges as functions and data blocks.

    $ rabin2 -sr /bin/ls
    
    fs symbols
    Cd 8 @ 0x0021a610
    f sym.stdout 8 0x0021a610
    f sym._edata 0 0x0021a600
    f sym._end 0 0x0021b388
    Cd 8 @ 0x0021a600
    f sym.__progname 8 0x0021a600
    Cd 8 @ 0x0021a630
    f sym.program_invocation_name 8 0x0021a630
    f sym.__bss_start 0 0x0021a600
