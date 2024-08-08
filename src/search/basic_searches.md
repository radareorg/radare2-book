## Basic Search

A basic search for a plain text string in a file would be something like:

```console
$ r2 -q -c "/ lib" /bin/ls
Searching 3 bytes from 0x00400000 to 0x0041ae08: 6c 69 62 
hits: 9
0x00400239 hit0_0 "lib64/ld-linux-x86-64.so.2"
0x00400f19 hit0_1 "libselinux.so.1"
0x00400fae hit0_2 "librt.so.1"
0x00400fc7 hit0_3 "libacl.so.1"
0x00401004 hit0_4 "libc.so.6"
0x004013ce hit0_5 "libc_start_main"
0x00416542 hit0_6 "libs/"
0x00417160 hit0_7 "lib/xstrtol.c"
0x00417578 hit0_8 "lib"
```

As can be seen from the output above, radare2 generates a "hit" flag for every entry found. You can then use the `ps` command to see the strings stored at the offsets marked by the flags in this group, and they will have names of the form `hit0_<index>`:

```console
[0x00404888]> / ls
...
[0x00404888]> ps @ hit0_0
lseek
```

You can search for wide-char strings (e.g., unicode letters) using the `/w` command:

```console
[0x00000000]> /w Hello
0 results found.
```

To perform a case-insensitive search for strings use `/i`:

```console
[0x0040488f]> /i Stallman
Searching 8 bytes from 0x00400238 to 0x0040488f: 53 74 61 6c 6c 6d 61 6e
[# ]hits: 004138 < 0x0040488f  hits = 0
```

It is possible to specify hexadecimal escape sequences in the search string by prepending them with `\x`:

```console
[0x00000000]> / \x7FELF
```

if, instead, you are searching for a string of hexadecimal values, you're probably better of using the `/x` command:

```console
[0x00000000]> /x 7F454C46
```

If you want to mask some nibble during the search you can use the symbol **.** to allow any nibble value to match:

```console
[0x00407354]> /x 80..80
0x0040d4b6 hit3_0 800080
0x0040d4c8 hit3_1 808080
0x004058a6 hit3_2 80fb80
```

You may not know some bit values of your hexadecimal pattern. Thus you may use a bit mask on your pattern. Each bit set to one in the mask indicates to search the bit value in the pattern. A bit set to zero in the mask indicates that the value of a matching value can be 0 or 1:

```console
[0x00407354]> /x 808080:ff80ff
0x0040d4c8 hit4_0 808080
0x0040d7b0 hit4_1 808080
0x004058a6 hit4_2 80fb80    
```

You can notice that the command `/x 808080:ff00ff` is equivalent to the command `/x 80..80`.

Once the search is done, the results are stored in the `searches` flag space.

```console
[0x00000000]> fs
0    0 . strings
1    0 . symbols
2    6 . searches

[0x00000000]> f
0x00000135 512 hit0_0
0x00000b71 512 hit0_1
0x00000bad 512 hit0_2
0x00000bdd 512 hit0_3
0x00000bfb 512 hit0_4
0x00000f2a 512 hit0_5
```

To remove "hit" flags after you do not need them anymore, use the `f- hit*` command.

Often, during long search sessions, you will need to launch the latest search more than once. You can use the `//` command to repeat the last search.

```console
[0x00000f2a]> //     ; repeat last search
```
