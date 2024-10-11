## Annotations

Comment annotations (See the `ano` command) it's a cross-project metadata feature introduced in radare2 5.9.6.

```console
[0x00000000]> ano?
Usage: ano  [*] # function anotations
| ano            show or edit annotations for the current function
| ano-*          remove all annotations of current file
| ano*           dump all annotations in ano= commands
| ano=[b64text]  set anotation text in base64 for current function
| anoe           edit annotation
| anos           show annotation
| anol           show first line of function annotation if any
[0x00000000]>
```

The annotations are not tied to projects or sessions. They are stored in your home as separate files, and they are associated with each function.

This is useful because you can have a multiline comment for each function where you drop some notes, paste the decompilation output, etc and you can leave the shell without worrying about saving it later.

These files are stored in the cache subdirectory of the radare2 datadir: `~/.local/share/radare2/cache`.
