## Searching Backwards

Sometimes you want to find a keyword backwards. This is, before the current offset, to do this you can seek back and search forward by adding some search.from/to restrictions, or use the `/b` command.

```
[0x100001200]> / nop
0x100004b15 hit0_0 .STUWabcdefghiklmnopqrstuvwxbin/ls.
0x100004f50 hit0_1 .STUWabcdefghiklmnopqrstuwx1] [file .
[0x100001200]> /b nop
[0x100001200]> s 0x100004f50p
[0x100004f50]> /b nop
0x100004b15 hit2_0 .STUWabcdefghiklmnopqrstuvwxbin/ls.
[0x100004f50]>
```

Note that `/b` is doing the same as `/`, but backward, so what if we want to use `/x` backward? We can use `/bx`, and the same goes for other search subcommands:

```
[0x100001200]> /x 90
0x100001a23 hit1_0 90
0x10000248f hit1_1 90
0x1000027b2 hit1_2 90
0x100002b2e hit1_3 90
0x1000032b8 hit1_4 90
0x100003454 hit1_5 90
0x100003468 hit1_6 90
0x10000355b hit1_7 90
0x100003647 hit1_8 90
0x1000037ac hit1_9 90
0x10000389c hit1_10 90
0x100003c5c hit1_11 90

[0x100001200]> /bx 90
[0x100001200]> s 0x10000355b
[0x10000355b]> /bx 90
0x100003468 hit3_0 90
0x100003454 hit3_1 90
0x1000032b8 hit3_2 90
0x100002b2e hit3_3 90
0x1000027b2 hit3_4 90
0x10000248f hit3_5 90
0x100001a23 hit3_6 90
[0x10000355b]>
```