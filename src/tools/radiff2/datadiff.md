## Data Diffing

Data diffing with radiff2 allows you to compare binary data between files of different sizes. This is useful for identifying differences at the byte level, regardless of file length.

For example, comparing two files with `radiff2 -x` shows the differences in two column hexdump+ascii format:

```console
$ cat 1
hello
$ cat 2
hallo
$ radiff2 -x 1 2
  offset     0 1 2 3 4 5 6 7 01234567    0 1 2 3 4 5 6 7 01234567
0x00000000! 68656c6c6f0a     hello.     68616c6c6f0a     hallo.
```

Also in hexII format:

```console
$ radiff2 -X 1 2
0x00000000! .h.e.l.l.o0a         .h.a.l.l.o0a    
```

or even the unified diff format using the `-U` flag:

```console
$ radiff2 -U 1 2
--- /tmp/r_diff.61dd4e41da041	2024-07-22 14:07:37.682683431 +0200
+++ /tmp/r_diff.61dd4e41da06b	2024-07-22 14:07:37.682683431 +0200
@@ -1 +1 @@
-hello
+hallo
```

Let's understand the output because in your terminal you'll see some green and red highlighting the added or removed bytes from the byte-to-byte comparison.

* `!` sign after the offset explains if the block is equal or not
* hexdump portion of file 1
* hexdump portion of file 2

When comparing files of different sizes, we will need to use the `-d` flag which performs a delta-diffing algorithm, trying to find the patterns of bytes that has been added or removed when a specific change is found.

```console
$ cat 1 
hello
$ cat 3
helloworld
$ radiff2 1 3
INFO: File size differs 6 vs 11
INFO: Buffer truncated to 6 byte(s) (5 not compared)
0x00000005 0a => 77 0x00000005
$ radiff2 -d 1 3
INFO: File size differs 6 vs 11
0x00000000 68656c6c6f0a => 68656c6c6f776f726c640a 0x00000000
$
```

For JSON output, use radiff2 -j -d to get detailed diff information in JSON format:

```console
$ radiff2 -j -d 1 3 |jq .
INFO: File size differs 6 vs 11
{
  "files": [
    {
      "filename": "1",
      "size": 6,
      "sha256": "5891b5b522d5df086d0ff0b110fbd9d21bb4fc7163af34d08286a2e846f6be03"
    },
    {
      "filename": "3",
      "size": 11,
      "sha256": "8cd07f3a5ff98f2a78cfc366c13fb123eb8d29c1ca37c79df190425d5b9e424d"
    }
  ],
  "changes": [
    {
      "addr": 0,
      "from": "68656c6c6f0a",
      "to": "68656c6c6f776f726c640a"
    }
  ]
}
$
```

These examples demonstrate how radiff2 can effectively highlight differences in files of varying lengths, providing clear insights into changes at the binary level.
