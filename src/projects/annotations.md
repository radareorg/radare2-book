## Annotations

Annotations are a handy feature introduced in Radare2 5.9.6 that allows users to associate cross-project metadata with specific functions. Unlike comments or other metadata that are tied to a specific project or session, annotations are persistent and stored globally. This makes them particularly useful for tracking notes and observations across different projects without needing to worry about manually saving them.

The annotations are associated with the filename and function address, and it's stored in a dedicated cache directory, making them available even after you leave the session. You can think of annotations as a place to store function-specific notes, decompilation output, or other important information that you want to keep handy.

```
R2_CACHEDIR=~/.local/share/radare2/cache/
${R2_CACHEDIR}/ano.${filename}.{func_address}.txt
```

### Using the ano

Annotations can be managed using the `ano` command. Below is an overview of the available options:

```console
[0x00000000]> ano?
Usage: ano  [*] # function anotations
| ano                show or edit annotations for the current function
| ano-*              remove all annotations for current file
| ano-$$             remove annotations for current function
| ano*               dump all annotations in ano= commands
| ano=[base64:]text  set anotation text in base64 for current function
| anoe               edit annotation using cfg.editor
| anos               show current function annotation
| anol               display first line of function annotation if any
[0x00000000]>
```

### Key Features

**Persistent Across Sessions:**

Annotations are stored globally in the XDG cache directory. This ensures that they are accessible across different sessions, even if no project is used.

**Multiline Annotations:**

Annotations can contain multiple lines of text, making them ideal for storing detailed notes, such as decompilation output, comments, or any other observations about a function.

Only the first line of the annotation will be displayed in the disassembly. Take this in mind when writing them to make them look nicer in the disassembly listing.

You can disable this feature by toggling the `e asm.anos` eval variable.

**Cross-Project:**

Since annotations are not tied to any specific project, they can be shared across different projects that analyze the same binary. This is useful when working with multiple teams or revisiting an old analysis.

### Examples

#### Setting an Annotation for a Function

To add an annotation to the current function, you can simply use the `ano=` command with base64-encoded text:

```console
[0x00000000]> ano=[b64text]
```

#### Editing and Viewing Annotations

If you want to manually edit the annotation for the current function, use the `anoe` command, which allows you to modify the annotation interactively:

```console
[0x00000000]> anoe
```

You can also view the full annotation using `anos`:

```console
[0x00000000]> anos
```

If you only want to see the first line of the annotation (which is useful for quick reference), you can use `anol`:

```console
[0x00000000]> anol
```

#### Removing Annotations

To remove all annotations for the current file, you can use the `ano-*` command:

```console
[0x00000000]> ano-*
```

### Annotations In Action

*Using Annotations to Cache Decompilation Output*

Annotations can also be used to improve efficiency when working with decompiled code. For example, the `-e cache=true` setting in Radare2 enables the caching of decompiled output. This prevents Radare2 from having to re-decompile the same function multiple times, thus saving time during the analysis.

Here's an example of how this works:

* Decompiling a function using AI requires consuming tokens or cpu time, which tends to be slow and expensive.
* By enabling caching with `-e cache=true`, Decai will store the decompilation output in an annotation. The next time you view the same function, the cached annotation will be used instead of calling the decompiler again.

This is particularly helpful when working with large binaries or performing repetitive decompilation tasks.

```bash
$ r2pm -ci decai # install the decai decompiler
$ r2 -c 'decai -e cache=true' <binary>
```

By leveraging annotations in this way, you can significantly reduce the overhead of reprocessing functions during analysis.

### Conclusion

Annotations are an essential tool for efficiently managing function-specific metadata across multiple sessions and projects. Whether you are adding notes, decompilation output, or general observations, annotations allow you to persist important information and retrieve it at any time. The ability to use annotations for caching decompilation results further enhances the analysis workflow, saving both time and effort.
