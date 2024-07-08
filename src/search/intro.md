# Searching

The radare2 search engine is based on work done by esteve, plus multiple features implemented on top of it. It supports multiple keyword searches, binary masks, and hexadecimal values. It automatically creates flags for search hit locations to ease future referencing.

Searching is accessed with `/` command.

```
[0x00000000]> /?
|Usage: /[!bf] [arg]Search stuff (see 'e??search' for options)
|Use io.va for searching in non virtual addressing spaces
| / foo\x00               search for string 'foo\0'
| /j foo\x00              search for string 'foo\0' (json output)
| /! ff                   search for first occurrence not matching, command modifier
| /!x 00                  inverse hexa search (find first byte != 0x00)
| /+ /bin/sh              construct the string with chunks
| //                      repeat last search
| /a jmp eax              assemble opcode and search its bytes
| /A jmp                  find analyzed instructions of this type (/A? for help)
| /b                      search backwards, command modifier, followed by other command
| /B                      search recognized RBin headers
| /c jmp [esp]            search for asm code matching the given string
| /ce rsp,rbp             search for esil expressions matching
| /C[ar]                  search for crypto materials
| /d 101112               search for a deltified sequence of bytes
| /e /E.F/i               match regular expression
| /E esil-expr            offset matching given esil expressions %%= here
| /f                      search forwards, command modifier, followed by other command
| /F file [off] [sz]      search contents of file with offset and size
| /g[g] [from]            find all graph paths A to B (/gg follow jumps, see search.count and
anal.depth)
| /h[t] [hash] [len]      find block matching this hash. See ph
| /i foo                  search for string 'foo' ignoring case
| /m magicfile            search for matching magic file (use blocksize)
| /M                      search for known filesystems and mount them automatically
| /o [n]                  show offset of n instructions backward
| /O [n]                  same as /o, but with a different fallback if anal cannot be used
| /p patternsize          search for pattern of given size
| /P patternsize          search similar blocks
| /r[erwx][?] sym.printf  analyze opcode reference an offset (/re for esil)
| /R [grepopcode]         search for matching ROP gadgets, semicolon-separated
| /s                      search for all syscalls in a region (EXPERIMENTAL)
| /v[1248] value          look for an `cfg.bigendian` 32bit value
| /V[1248] min max        look for an `cfg.bigendian` 32bit value in range
| /w foo                  search for wide string 'f\0o\0o\0'
| /wi foo                 search for wide string ignoring case 'f\0o\0o\0'
| /x ff..33               search for hex string ignoring some nibbles
| /x ff0033               search for hex string
| /x ff43:ffd0            search for hexpair with mask
| /z min max              search for strings of given size
```

Because everything is treated as a file in radare2, it does not matter whether you search in a socket, a remote device, in process memory, or a file.

Note that '/\*' is not a command - it starts a multiline comment. Type '\*/' to end the comment after it's opened.

## Search Options

Options are controlled by the `search.` variables.

```
[0x00000000]> e??search
        search.align: only catch aligned search hits
        search.chunk: chunk size for /+ (default size is asm.bits/8
   search.contiguous: accept contiguous/adjacent search hits
     search.distance: search string distance
    search.esilcombo: stop search after N consecutive hits
        search.flags: all search results are flagged, otherwise only printed
         search.from: search start address
           search.in: specify search boundaries
        search.kwidx: store last search index count
      search.maxhits: maximum number of hits (0: no limit)
      search.overlap: look for overlapped search hits
       search.prefix: prefix name in search hits label
         search.show: show search results
           search.to: search end address
      search.verbose: make the output of search commands verbose
```

Perhaps the most important search variable is `search.in` - it controls where
your search is occurring. If you aren't finding hits you expect, check this
variable first. Note the difference between `map` and `maps` - `map` will only
search the map that you are currently in, while `maps` will search all memory
maps, with options to narrow the search by permissions.

```
[0x00000000]> e search.in=?
raw
block
bin.section
bin.sections
bin.sections.rwx
bin.sections.r
bin.sections.rw
bin.sections.rx
bin.sections.wx
bin.sections.x
io.map
io.maps
io.maps.rwx
io.maps.r
io.maps.rw
io.maps.rx
io.maps.wx
io.maps.x
dbg.stack
dbg.heap
dbg.map
dbg.maps
dbg.maps.rwx
dbg.maps.r
dbg.maps.rw
dbg.maps.rx
dbg.maps.wx
dbg.maps.x
anal.fcn
anal.bb
```
