# Analysis

There are different commands to perform data and code analysis and extract information like pointers, string references, basic blocks, extract opcode information, jump information, xrefs, etc..

Those operations are handled by the root 'a'nalyze command:

    |Usage: a[?adfFghoprsx]
    | a8 [hexpairs]    analyze bytes
    | aa               analyze all (fcns + bbs)
    | ad               analyze data trampoline (wip)
    | ad [from] [to]   analyze data pointers to (from-to)
    | ae [expr]        analyze opcode eval expression (see ao)
    | af[rnbcsl?+-*]   analyze Functions
    | aF               same as above, but using graph.depth=1
    | ag[?acgdlf]      output Graphviz code
    | ah[?lba-]        analysis hints (force opcode size, ...)
    | ao[e?] [len]     analyze Opcodes (or emulate it)
    | ap               find and analyze function preludes
    | ar[?ld-*]        manage refs/xrefs (see also afr?)
    | as [num]         analyze syscall using dbg.reg
    | at[trd+-*?] [.]  analyze execution Traces
    |Examples:
    | f ts @ `S*~text:0[3]`; f t @ section..text
    | f ds @ `S*~data:0[3]`; f d @ section..data
    | .ad t t+ts @ d:ds
