# r2yara

r2yara is the integration of [Yara](https://virustotal.github.io/yara/) into radare2. Yara is a powerful binary analysis tool using rules to identity or classify patterns in a binaries.

## Installation

r2yara is maintained in a separate repository. It can be installed with the `r2pm` package manager:

```console
$ r2pm -ci r2frida
```

The package manager handle automatically the installation and compilation of the Yara library.

## Usage

As soon as r2yara is installed, two sets of commands are available in r2. The first set of command are invoked with the command `yara`:

```console
[0x00000000]> yara
Usage: yara  [action] [args..]   load and run yara rules inside r2
| yara add [file]   Add yara rules from file, or open $EDITOR with yara rule template
| yara clear        Clear all rules
| yara help         Show this help (same as 'yara?')
| yara list         List all rules
| yara scan[S]      Scan the current file, if S option is given it prints matching strings
| yara show [name]  Show rules containing name
| yara tag [name]   List rules with tag 'name'
| yara tags         List tags from the loaded rules
| yara version      Show version information about r2yara and yara
```

The second kind of shorter commands start with `yr`:

```console
[0x00000000]> yr
Usage: yr [action] [args..]   load and run yara rules inside r2
| yr [file]        add yara rules from file
| yr               same as yr?
| yr-*             unload all the rules
| yr?              show this help (same as 'yara?')
| yrg[?][-sxf]     generate yara rule
| yrl              list loaded rules
| yrs[q]           scan the current file, suffix with 'q' for quiet mode
| yrt ([tagname])  list tags from loaded rules, or list rules from given tag
| yrv              show version information about r2yara and yara
```

### Rule management

The plugin loads automatically the rules files from the folder `rules-yara3` in the folder `$R2PM_PLUGDIR`. The already loaded rules can be listed:

```console
[0x00055a40]> yrl
HelloWorld
BLOWFISH_Constants
MD5_Constants
RC6_Constants
RIPEMD160_Constants
SHA1_Constants
SHA256_Constants
SHA512_Constants
WHIRLPOOL_Constants
```

The rules can be cleared:

```console
[0x00055a40]> yr-
INFO: Rules cleared
[0x00055a40]> yrl
```

Other rules can be added from an external files.

```console
[0x00055a40]> yr r2yara/rules/crypto.yara
```

### Scan

The current binary can be scanned with the currently loaded rules to see if any rule matches a pattern in the binary:

```console
[0x00055a40]> yrs
BLOWFISH_Constants
0x004960c0: yara3.BLOWFISH_Constants_0 : a60b31d1
0x004960c4: yara3.BLOWFISH_Constants_1 : acb5df98
0x004960c8: yara3.BLOWFISH_Constants_2 : db72fd2f
0x004960cc: yara3.BLOWFISH_Constants_3 : b7df1ad0
0x004964c0: yara3.BLOWFISH_Constants_4 : e9707a4b
0x004968c8: yara3.BLOWFISH_Constants_5 : 1c264cf6
SHA256_Constants
0x001936c4: yara4.SHA256_Constants_0 : 982f8a42
0x0019372b: yara4.SHA256_Constants_1 : 91443771
0x0019379d: yara4.SHA256_Constants_2 : cffbc0b5
0x001937e5: yara4.SHA256_Constants_3 : a5dbb5e9
```

As soon as a pattern is identified, a flag is created at the pattern address.

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
