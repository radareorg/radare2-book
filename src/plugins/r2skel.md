# r2skel

This project ships basic skeletons to start writing projects that work with radare2.

The repository have different type of plugins and scripts organized by language and cathegories depending on its functionalities.

* Tool: r2, r2ai, r2frida, ..
* Language: C, r2js, Node, Python, TypeScript, ..
* Purpose: Plugin, Script, Bindings, Native APIs
* Module: Core, Bin, Arch, Debug, ..

## Installation

```console
$ r2pm -ci r2skel
```

## Usage Example

If the r2pm bindir is not in your PATH you can spawn the tool with `r2pm -r`:

```console
$ r2pm -r r2skel
r2skel [-lL] | [template] [new-directory]
Options:
 -l - list all the available project templates
 -L - list all the languages
Templates:
 - r2-plugin-arch-py
 - r2-plugin-bin-c
 - r2-plugin-core-c
 - r2-plugin-core-js
 - r2-plugin-core-py
 - r2-plugin-core-ts
 - r2-plugin-io-c
 - r2-plugin-io-js
 - r2-plugin-io-py
 - r2-plugin-parse-c
 - r2-plugin-parse-js
 - r2-program-libr-c
 - r2-program-libr-v
 - r2-script-lang-c
 - r2-script-lang-nim
 - r2-script-r2papi-ts
 - r2-script-r2pipe-nodejs
 - r2-script-r2pipe-rust
 - r2-script-r2pipe-swift
 - r2-script-r2pipe-ts
 - r2ai-script-cmd-py
 - r2frida-plugin-cmd-js
$
```

Now you are ready to create your first example:

```console
$ r2pm -r r2skel r2-plugin-core-js coreplug
$ cd coreplug
$ make
$ r2 -i coreplug.js --
[0x00000000]> coreplug
...
```
