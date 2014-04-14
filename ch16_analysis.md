# Chapter 16: Analysis

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

#16.1 Code analysis

The code analysis is a common technique used to extract information from the assembly code. Radare stores multiple internal data structures to identify basic blocks, function trees, extract opcode-level information and such.

One common radare2 analysis command usage is the following:

    [0x08048440]> aa
    [0x08048440]> pdf @ main 
    
               ; DATA XREF from 0x08048457 (entry0)
    / (fcn) fcn.08048648 141
    |          ;-- main:
    |          0x08048648    8d4c2404     lea ecx, [esp+0x4]
    |          0x0804864c    83e4f0       and esp, 0xfffffff0
    |          0x0804864f    ff71fc       push dword [ecx-0x4]
    |          0x08048652    55           push ebp
    |          ; CODE (CALL) XREF from 0x08048734 (fcn.080486e5)
    |          0x08048653    89e5         mov ebp, esp
    |          0x08048655    83ec28       sub esp, 0x28
    |          0x08048658    894df4       mov [ebp-0xc], ecx
    |          0x0804865b    895df8       mov [ebp-0x8], ebx
    |          0x0804865e    8975fc       mov [ebp-0x4], esi
    |          0x08048661    8b19         mov ebx, [ecx]
    |          0x08048663    8b7104       mov esi, [ecx+0x4]
    |          0x08048666    c744240c000. mov dword [esp+0xc], 0x0
    |          0x0804866e    c7442408010. mov dword [esp+0x8], 0x1 ;  0x00000001 
    |          0x08048676    c7442404000. mov dword [esp+0x4], 0x0
    |          0x0804867e    c7042400000. mov dword [esp], 0x0
    |          0x08048685    e852fdffff   call sym..imp.ptrace
    |             sym..imp.ptrace(unk, unk)
    |          0x0804868a    85c0         test eax, eax
    |      ,=< 0x0804868c    7911         jns 0x804869f
    |      |   0x0804868e    c70424cf870. mov dword [esp], str.Don_tuseadebuguer_ ;  0x080487cf 
    |      |   0x08048695    e882fdffff   call sym..imp.puts
    |      |      sym..imp.puts()
    |      |   0x0804869a    e80dfdffff   call sym..imp.abort
    |      |      sym..imp.abort()
    |      `-> 0x0804869f    83fb02       cmp ebx, 0x2
    |     ,==< 0x080486a2    7411         je 0x80486b5
    |     |    0x080486a4    c704240c880. mov dword [esp], str.Youmustgiveapasswordforusethisprogram_ ;  0x0804880c 
    |     |    0x080486ab    e86cfdffff   call sym..imp.puts
    |     |       sym..imp.puts()
    |     |    0x080486b0    e8f7fcffff   call sym..imp.abort
    |     |       sym..imp.abort()
    |     `--> 0x080486b5    8b4604       mov eax, [esi+0x4]
    |          0x080486b8    890424       mov [esp], eax
    |          0x080486bb    e8e5feffff   call fcn.080485a5
    |             fcn.080485a5() ; fcn.080484c6+223
    |          0x080486c0    b800000000   mov eax, 0x0
    |          0x080486c5    8b4df4       mov ecx, [ebp-0xc]
    |          0x080486c8    8b5df8       mov ebx, [ebp-0x8]
    |          0x080486cb    8b75fc       mov esi, [ebp-0x4]
    |          0x080486ce    89ec         mov esp, ebp
    |          0x080486d0    5d           pop ebp
    |          0x080486d1    8d61fc       lea esp, [ecx-0x4]
    \          0x080486d4    c3           ret

