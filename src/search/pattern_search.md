## Pattern Search

### Pattern Matching Search

The `/p` command allows you to apply repeated pattern searches on IO backend storage. It is possible to identify repeated byte sequences without explicitly specifying them. The only command's parameter sets minimum detectable pattern length.
Here is an example:

```console
[0x00000000]> /p 10
```

This command output will show different patterns found and how many times each of them is encountered.

It is possible to search patterns with a known difference between consecutive bytes with `/d` command. For example, the command to search all the patterns with the first and second bytes having the first bit which differs and the second and third bytes with the second bit which differs is:

```console
[0x00000000]> /d 0102
Searching 2 bytes in [0x0-0x400]
hits: 2
0x00000118 hit2_0 9a9b9d
0x00000202 hit2_1 a4a5a7
```

### Magic Pattern Search

The `/m` command allows to search for known magic patterns helping to discover hidden filesystem, archives or other known file formats:

```console
[0x00000000]> /m
-- 0 120eb
0x00000000 0 hit0_0 JPEG image , JFIF standard
0x00009f64 0 hit0_1 ZIP Zip archive data, at least v2.0 to extract
0x000120d5 0 hit0_2 End of Zip archive
```

Those magic patterns a defined according to the [file command's magic pattern format](https://linux.die.net/man/5/magic). By default radare2 uses all the patterns files in the folder `libr/magic/d/default/`. Searching with all the pattern files may be long but it is possible to use a single file given as argument:

```console
[0x00000000]> /m ./libr/magic/d/default/archive
-- 0 120eb
0x00009f64 0 hit0_0 ZIP Zip archive data, at least v2.0 to extract
0x000120d5 0 hit0_1 End of Zip archive
```console

It also allows to defined and search with custom pattern files.