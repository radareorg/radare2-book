## IO Configuration

The IO implementation is very complex and can be configured in many ways to serve the way the user needs. This chapter will introduce you to some of the most important configuration options under the eval.

```console
[0x100003a84]> e??io.
             io.0xff: use this value instead of 0xff to fill unallocated areas
             io.aslr: disable ASLR for spawn and such
           io.autofd: change fd when opening a new file
          io.basemap: create a map at base address 0 when opening a file
            io.cache: change both of io.cache.{read,write}
       io.cache.auto: automatic cache all reads in the IO backend
      io.cache.nodup: do not cache duplicated cache writes
       io.cache.read: enable read cache for vaddr (or paddr when io.va=0)
      io.cache.write: enable write cache for vaddr (or paddr when io.va=0)
             io.exec: see !!r2 -h~-x
               io.ff: fill invalid buffers with 0xff instead of returning error
             io.mask: mask addresses before resolving as maps
          io.overlay: honor io overlay
             io.pava: use EXPERIMENTAL paddr -> vaddr address mode
           io.pcache: io.cache for p-level
      io.pcache.read: enable read-cache
     io.pcache.write: enable write-cache
          io.unalloc: check each byte if it's allocated
       io.unalloc.ch: char to display if byte is unallocated
               io.va: use virtual address layout
```

### io.unalloc

When set to true it will be showing `?` instead of 0xff in the hexdump/disasm views if there's no associated map. This causes the dump to be a bit slower, but probably more real.

See `io.0xff` and `io.unalloc.ch` for reference

### io.cache

Enables the cache layer for the whole memory address space. This means that you can write and patch anywhere in memory and the underlying files won't be modified.

See `io.cache.read` and `io.cache.write`

Note that enabling the read cache will speedup readings from slow or remote endpoints, this is handy when performing analysis via GDB, so the read data from the remote process will be read once.

### io.pcache

Enables a physical layer cache associated with each map. This way it is possible to keep the correct behaviour when accessing unallocated regions or multi-map regions honoring the proper permissions, unlike `io.cache`.

See `io.pcache.read` and `io.pcache.write`

### io.va

When set to false, it will seek around physical addresses on the currently selected file descriptor, instead of the whole virtual address.
