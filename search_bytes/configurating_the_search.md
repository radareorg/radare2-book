## 5.2 Configurating the searchs

The search engine can be configured by the `e` interface:

    Configuration:
    e cmd.hit = x         ; command to execute on every search hit
    e search.distance = 0 ; search string distance
    e search.in = [foo]   ; boundaries to raw, block, file, section)
    e search.align = 4    ; only catch aligned search hits
    e search.from = 0     ; start address
    e search.to = 0       ; end address
    e search.asmstr = 0   ; search string instead of assembly
    e search.flags = true ; if enabled store flags on keyword hits

`search.align` variable is used to determine that the only `valid` search hits must have to fit in this alignement. For example. you can use `e search.align=4` to get only the hits found in 4-byte aligned addresses.

The `search.flag` boolean variable makes the engine setup flags when finding hits. If the search is stopped by the user with a ^C then a `search_stop` flag will be added.
