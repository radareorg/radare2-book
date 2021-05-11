## Pattern Matching Search

The `/p` command allows you to apply repeated pattern searches on IO backend storage. It is possible to identify repeated byte sequences without explicitly specifying them. The only command's parameter sets minimum detectable pattern length.
Here is an example:
```
[0x00000000]> /p 10
```
This command output will show different patterns found and how many times each of them is encountered.

It is possible to search patterns with a known difference between consecutive bytes with `\d` command. For example, the command to search all the patterns with the first and second bytes having the first bit which differs and the second and third bytes with the second bit which differs is:

```
[0x00000000]> /d 0102
Searching 2 bytes in [0x0-0x400]
hits: 2
0x00000118 hit2_0 9a9b9d
0x00000202 hit2_1 a4a5a7
```
