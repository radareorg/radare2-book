# Searching for Bytes

The radare2 search engine is based on work done by esteve, plus multiple features implemented on top of it. It supports multiple keyword searches, binary masks, hexadecimal. It automatically flags search hit entries to ease future referencing.
Search is initiated by `/` command.

    [0x00000000]> /?
    Usage: /[amx/] [arg]
    / foo\x00       search for string `foo\0`
    /w foo          search for wide string `f\0o\0o\0`
    /wi foo         search for wide string ignoring case `f\0o\0o\0`
    /! ff           search for first occurrence not matching
    /i foo          search for string `foo` ignoring case
    /e /E.F/i       match regular expression
    /x ff0033       search for hex string
    /x ff..33       search for hex string ignoring some nibbles
    /x ff43 ffd0    search for hexpair with mask
    /d 101112       search for a deltified sequence of bytes
    /!x 00          inverse hexa search (find first byte != 0x00)
    /c jmp [esp]    search for asm code (see search.asmstr)
    /a jmp eax      assemble opcode and search its bytes
    /A              search for AES expanded keys
    /r sym.printf   analyze opcode reference an offset
    /R              search for ROP gadgets
    /P              show offset of previous instruction
    /m magicfile    search for matching magic file (use blocksize)
    /p patternsize  search for pattern of given size
    /z min max      search for strings of given size
    /v[?248] num    look for a asm.bigendian 32bit value
    //              repeat last search
    /b              search backwards

Because everything is treated as a file in radare2, it does not matter whether you search in a socket, a remote device, in  process memory, or a file.
