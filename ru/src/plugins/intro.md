# Plugins

radare2 is implemented on top of a bunch of libraries, almost every of those
libraries support plugins to extend the capabilities of the library or add
support for different targets.

This section aims to explain what are the plugins, how to write them and use them

## Types of plugins
```
$ ls libr/*/p | grep : | awk -F / '{ print $2 }'
anal      # analysis plugins
asm       # assembler/disassembler plugins
bin       # binary format parsing plugins
bp        # breakpoint plugins
core      # core plugins (implement new commands)
crypto    # encrypt/decrypt/hash/...
debug     # debugger backends
egg       # shellcode encoders, etc
fs        # filesystems and partition tables
io        # io plugins
lang      # embedded scripting languages
parse     # disassembler parsing plugins
reg       # arch register logic
```

## Listing plugins

Some r2 tools have the `-L` flag to list all the plugins associated to the
functionality.
```
rasm2 -L    # list asm plugins
r2 -L       # list io plugins
rabin2 -L   # list bin plugins
rahash2 -L  # list hash/crypto/encoding plugins
```
There are more plugins in r2land, we can list them from inside r2, and this is
done by using the `L` suffix.

Those are some of the commands:
```
L          # list core plugins
iL         # list bin plugins
dL         # list debug plugins
mL         # list fs plugins
ph         # print support hash algoriths
```

You can use the `?` as value to get the possible values in the associated eval vars.

```
e asm.arch=?   # list assembler/disassembler plugins
e anal.arch=?  # list analysis plugins
```
## Notes

Note there are some inconsistencies that most likely will be fixed in the future radare2 versions.

