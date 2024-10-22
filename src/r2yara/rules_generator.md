## Rule generator

r2yara allows the creation of YARA rules inside radare2 with the `yrg` group of commands:

```console
[0x00000000]> yrg?
Usage: yrg [action] [args..]   load and run yara rules inside r2
| yrg-          delete last pattern added to the yara rule
| yrg-*         delete all the patterns in the current rule
| yrgs ([len])  add string (optionally specify the length)
| yrgx ([len])  add hexpairs of blocksize (or custom length)
| yrgf ([len])  add function bytepattern signature
| yrgz          add all strings referenced from current function
```

The current state of the YARA rule can be displayed with the `yrg` command:

```console
[0x00000000]> yrg
WARN: See 'yrg?' to find out which subcommands use to append patterns to the rule
rule rulename : test {
  meta:
    author = "user"
    description = "My first yara rule"
    date = "2024-10-22"
    version = "0.1"
}
```

Let's assumed we have found an interesting string during reversing:

```console
[0x00132700]> ps
expand 32-byte k
```

To add into this pattern to the current YARA rule, the command `yrgs` can be used together with the length of the pattern to add:

```console
[0x00132700]> yrgs 16
[0x00132700]> yrg
rule rulename : test {
  meta:
    author = "sylvain"
    description = "My first yara rule"
    date = "2024-10-22"
    version = "0.1"
  strings:
    $ = "expand 32-byte k"
  condition:
    all of them
```

The rule parameters can be changed in the configuration. For example to change the name of the rule, use the following command:

```console
[0x00132700]> e yara.rule = Salsa20
```

Once the rule is ready, it can be added to other active YARA rules:

```console
[0x00132700]> yr+
INFO: Rule successfully added
[0x00132700]> yrl
Salsa20
```
