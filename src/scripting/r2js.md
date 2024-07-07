## r2js

Radare2 ships the QuickJS ES6/javascript runtime by default starting on versions 5.8.x, having a complete and standard programming language brings a lot of possibilities and ease

As long as javascript is a also a common target language for transpilation from many other languages it is possible to use this runtime for other programming languages, not just Javascript.

TypeScript is probably the primary choice because it is very well integrated with Visual Studio Code (or Vim, Helix, ..) offering autocompletion and other facilities for developing your scripts.

But it is also possible to use Nim, C (via Emscripten), Vlang, and many other languages.

### Scripts

You can run r2js scripts like you do with any other script:

* Using the `-i` flag on the system shell when launching r2.
* With the `. ` command inside the radare2 shell.

The **rlang** plugin will be selected depending on the file extension. In this case the `qjs` rlang plugin handles the `.r2.js` extension.

For example:

```sh
$ r2 -i foo.r2.js /bin/ls
```

If you want to go back to the shell after running the script use the `-q` flag:

```sh
$ r2 -qi foo.r2.js /bin/ls
```

### The REPL

To enter the `r2js` repl you can use the `-j` command or flag.

```sh
0$ r2 -j
QuickJS - Type "\h" for help
[r2js]>
```

Same command/flag works in the r2 shell too:

```sh
[0x00000000]> -j
QuickJS - Type "\h" for help
[r2js]>
```

In this repl (read-eval-print-loop) shell you can run javascript statements, like the ones you would use in NodeJS.

The `<tab>` key can be used to autocomplete expressions.

### R2Pipe.r2js

The rlang plugin exposes the classic `r2.cmd` interface to interact with radare2. This means that you can run a command and get the output in response.

### R2Papi.r2js

The R2Papi apis are also embedded inside the r2, this means that you can use the high level / idiomatic APIs too.

If the global r2pipe instance is available through the `r2` object. The R2Papi one is available as `R`.

This is an example:

```javascript
var r2 = new R2Pipe();
var R = new R2Papi(r2);
```

### R2FridaCompile

Frida-tools ship a TypeScript compiler that targets ESM and generates a single file containing all the js compiled files from a TypeScript project.

Radare2 supports the same esm-blob file format used by Frida, and if you don't want to depend on Python you can also use the native one distributed with the *r2frida* plugin named `r2frida-compile`.

For example:

```sh
$ r2frida-compile -o foo.r2.js foo.ts
$ r2 -qi foo.r2.js -
```

### TypeScript

The easiest way to run typescript programs inside radare2 is by using `r2frida-compile`, but you can also use the standard `tsc`.
