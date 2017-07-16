# Visual Assembler

You can use Visual Mode to assemble code using `A`.
For example let's replace the `push` by a `jmp`:

![Before](../pics/before.png)

Notice the preview of the disassembly and arrows:

![After](../pics/after.png)

You need to open the file in writing mode (`r2 -w` or `oo+`) in order to patch the file.
You can also use the cache mode: `e io.cache = true` and `wc?`.

Remember that patching files in debug mode only patch the memory not the file.