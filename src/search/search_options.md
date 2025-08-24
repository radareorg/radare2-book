## Search Options

The radare2 search engine can be configured through several configuration variables, modifiable with the `e` command.

```console
e cmd.hit = x         ; radare2 command to execute on every search hit
e search.distance = 0 ; search string distance
e search.in = [foo]   ; specify search boundaries. Supported values are listed under e search.in=??
e search.align = 4    ; only show search results aligned by specified boundary.
e search.from = 0     ; start address
e search.to = 0       ; end address
e search.asmstr = 0   ; search for string instead of assembly
e search.flags = true ; if enabled, create flags on hits
```

The `search.align` variable is used to limit valid search hits to certain alignment. For example, with `e search.align=4` you will see only hits found at 4-bytes aligned offsets.

The `search.flags` boolean variable instructs the search engine to flag hits so that they can be referenced later. If a currently running search is interrupted with `Ctrl-C` keyboard sequence, current search position is flagged with `search_stop`.

The `search.in` variable specifies search boundaries. To search entire memory, use `e search.in = dbg.maps`. The default value is `dbg.map`.

Options are controlled by the `search.` variables.

```console
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

```console
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
