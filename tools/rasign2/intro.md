## Rasign2

The `rasign2` tool allows you to quickly create signature files. To create a SDB signature file named `libc_sigs2.sdb` for the `libc.so.6` binary, simply run:

```
$ rasign2 -o libc_sigs2.sdb libc.so.6
[x] Analyze all flags starting with sym. and entry0 (aa)
generated zignatures: 2870
```

The above is equivalent to:

```
$ r2 libc.so.6
[0x00024330]> aa # analyze the file finding functions
[x] Analyze all flags starting with sym. and entry0 (aa)
[0x00024330]> zg  # generate zignatures for discovered functions
generated zignatures: 2870
[0x00024330]> zos libc_sigs.sdb  # write zignatures to a file
```

The '-a' flag can be added to increase the amount of analysis performed. This will result in more functions discovered and more zignatures created.

```
$ rasign2 -a -o /tmp/libc_sigs2.sdb libc.so.6
[x] Analyze all flags starting with sym. and entry0 (aa)
[x] Analyze function calls (aac)
[x] Analyze len bytes of instructions for references (aar)
[x] Check for objc references
[x] Check for vtables
[x] Type matching analysis for all functions (aaft)
[x] Propagate noreturn information
[x] Use -AA or aaaa to perform additional experimental analysis.
generated zignatures: 2955
```

### Output types

There are 3 different output methods. Using `-o` to create an SDB is shown above. The `-r` flag will print the discovered signatures to stdout as r2 commands.

```
$ rasign2 -r hello_world  |grep main
[x] Analyze all flags starting with sym. and entry0 (aa)
generated zignatures: 17
za main b 554889e54883ec20488d051b010000488945e8488d0516010000488945f0488b45e84889c7e88cfeffff8945e0488b45f04889c7e87dfeffff8945e48b55e08b45e401d083c00148984889c7e875feffff488945f848837df800743e488b55e8488b45f84889d64889c7e827feffff488b55f0488b45f84889d64889c7e854feffff488b45f84889c7e818feffff488b45f84889c7e8ecfdffff90c9c3:ffffffffffffffffff000000000000ffffffffff000000000000ffffffffffffffffffffffff00000000ffffffffffffffffffffff00000000ffffffffffffffffffffffffffffffffffffffff00000000ffffffffffffffffffff00ffffffffffffffffffffffffffffff00000000ffffffffffffffffffffffffffffff00000000ffffffffffffffff00000000ffffffffffffffff00000000ffffff
za main g cc=2 nbbs=3 edges=3 ebbs=1 bbsum=157
za main o 0x000007aa
za main r sym.imp.strlen sym.imp.strlen sym.imp.malloc sym.imp.strcpy sym.imp.strcat sym.imp.puts sym.imp.free
za main v b-24 b-16 b-32 b-28 b-8
za main t func.main.ret=int func.main.args=3 func.main.arg.0="int,argc" func.main.arg.1="char **,argv" func.main.arg.2="char **,envp"
za main h 44004dffff87483150d4f315ea8426b4d0c471ce4c56176c874513b24d0266b4
```

The `-j` flag will print results in JSON format.

```
rasign2 -j hello_world
[x] Analyze all flags starting with sym. and entry0 (aa)
generated zignatures: 17
[{"name":"main","bytes":"554889e54883ec20488d051b01000048894...
```

See the help menu for more details:

```
rasign2 -h
Usage: rasign2 [options] [file]
 -a [-a]          add extra 'a' to analysis command
 -o sigs.sdb      add signatures to file, create if it does not exist
 -r               show output in radare commands
 -j               show signatures in json
 -q               quiet mode
 -v               show version information
 -h               help menu
Examples:
  rasign2 -o libc.sdb libc.so.6
```
