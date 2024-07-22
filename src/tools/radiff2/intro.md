## Radiff2

radiff2 is a powerful tool within the radare2 suite designed for binary diffing. It can be somehow compared to the well known `diff` utility from UNIX, but with focus on comparing binary files.

It supports several types of diffing, including 1:1 binary diffing, delta diffing, code analysis diffing, and binary data (bindata) diffing.

These features allow users to compare two binaries at various levels, from raw data to disassembled code, providing insights into changes and differences between them. Additionally, radiff2 supports architecture and bits specification, graph diffing, and more, making it a versatile tool for reverse engineering tasks.

Many of these diffing features are also available in the `c` command within the radare2 shell. Opening the door to compare data from disk or process memory when using any io backend at any time without leaving the current session.

You can learn more about this tool by checking the help message or reading the manpage with `man radiff2`.

```
$ radiff2 -h
Usage: radiff2 [-abBcCdjrspOxuUvV] [-A[A]] [-g sym] [-m graph_mode][-t %] [file] [file]
  -a [arch]  specify architecture plugin to use (x86, arm, ..)
  -A [-A]    run aaa or aaaa after loading each binary (see -C)
  -b [bits]  specify register size for arch (16 (thumb), 32, 64, ..)
  -B         output in binary diff (GDIFF)
  -c         count of changes
  -C         graphdiff code (columns: off-A, match-ratio, off-B) (see -A)
  -d         use delta diffing
  -D         show disasm instead of hexpairs
  -e [k=v]   set eval config var value for all RCore instances
  -g [sym|off1,off2]   graph diff of given symbol, or between two offsets
  -G [cmd]   run an r2 command on every RCore instance created
  -i         diff imports of target files (see -u, -U and -z)
  -j         output in json format
  -n         print bare addresses only (diff.bare=1)
  -m [aditsjJ]  choose the graph output mode
  -O         code diffing with opcode bytes only
  -p         use physical addressing (io.va=0)
  -q         quiet mode (disable colors, reduce output)
  -r         output in radare commands
  -s         compute edit distance (no substitution, Eugene W. Myers O(ND) diff algorithm)
  -ss        compute Levenshtein edit distance (substitution is allowed, O(N^2))
  -S [name]  sort code diff (name, namelen, addr, size, type, dist) (only for -C or -g)
  -t [0-100] set threshold for code diff (default is 70%)
  -x         show two column hexdump diffing
  -X         show two column hexII diffing
  -u         unified output (---+++)
  -U         unified output using system 'diff'
  -v         show version information
  -V         be verbose (current only for -s)
  -z         diff on extracted strings
  -Z         diff code comparing zignatures

Graph Output formats: (-m [mode])
  <blank/a>  Ascii art
  s          r2 commands
  d          Graphviz dot
  g          Graph Modelling Language (gml)
  j          json
  J          json with disarm
  k          SDB key-value
  t          Tiny ascii art
  i          Interactive ascii art
```


### Practical examples

Here are a few practical examples of how to use radiff2:

To compare two binaries:

```sh
radiff2 bin1 bin2
```

To use graph diffing to compare functions by name:

```sh
radiff2 -g main bin1 bin2
```

To count the number of changes between two binaries:

```sh
radiff2 -c bin1 bin2
```

To output the diff in a unified format:

```sh
radiff2 -u bin1 bin2
```

To compare the opcodes of two functions:

```sh
radiff2 -O bin1 bin2
```

### Generating and Applying Patches

To compare two binaries and generate a patch file, you can use the following command:

```sh
$ echo hello > 1
$ echo hallo > 2
$ radiff2 -r 1 2
wx 61 @ 0x00000001
$ radiff2 -r 2 1
wx 65 @ 0x00000001
$
```

Note the `-r` flag will generate an r2 script, which can then be used to generate one binary from the other one using radare2 like this:

```sh
$ rahash2 -a md5 1 2
1: 0x00000000-0x00000005 md5: b1946ac92492d2347c6235b4d2611184
2: 0x00000000-0x00000005 md5: aee97cb3ad288ef0add6c6b5b5fae48a

$ radiff2 -r 1 2 > patch.r2
$ radare2 -qnw -i patch.r2 1

$ rahash2 -a md5 1 2
1: 0x00000000-0x00000005 md5: aee97cb3ad288ef0add6c6b5b5fae48a
2: 0x00000000-0x00000005 md5: aee97cb3ad288ef0add6c6b5b5fae48a
```

Let's explain the radare2 flags one by one:

* -q : quit after executing the script
* -n : do not load/parse binary headers
* -w : open in read-write mode
* -i : specify which script to run
