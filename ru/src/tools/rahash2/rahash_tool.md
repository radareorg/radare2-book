## Examples

The rahash2 tool can be used to calculate checksums and has functions of byte streams, files, text strings.

```
$ rahash2 -h
Usage: rahash2 [-rBhLkv] [-b S] [-a A] [-c H] [-E A] [-s S] [-f O] [-t O] [file] ...
 -a algo     comma separated list of algorithms (default is 'sha256')
 -b bsize    specify the size of the block (instead of full file)
 -B          show per-block hash
 -c hash     compare with this hash
 -e          swap endian (use little endian)
 -E algo     encrypt. Use -S to set key and -I to set IV
 -D algo     decrypt. Use -S to set key and -I to set IV
 -f from     start hashing at given address
 -i num      repeat hash N iterations
 -I iv       use give initialization vector (IV) (hexa or s:string)
 -S seed     use given seed (hexa or s:string) use ^ to prefix (key for -E)
             (- will slurp the key from stdin, the @ prefix points to a file
 -k          show hash using the openssh's randomkey algorithm
 -q          run in quiet mode (-qq to show only the hash)
 -L          list all available algorithms (see -a)
 -r          output radare commands
 -s string   hash this string instead of files
 -t to       stop hashing at given address
 -x hexstr   hash this hexpair string instead of files
 -v          show version information
```

To obtain an MD5 hash value of a text string, use the `-s` option:

```
$ rahash2 -q -a md5 -s 'hello world'
5eb63bbbe01eeed093cb22bb8f5acdc3
```

It is possible to calculate hash values for contents of files. But do not attempt to do it for very large files because rahash2 buffers the whole input in memory before computing the hash.

To apply all algorithms known to rahash2, use `all` as an algorithm name:

```
$ rahash2 -a all /bin/ls
/bin/ls: 0x00000000-0x000268c7 md5: 767f0fff116bc6584dbfc1af6fd48fc7
/bin/ls: 0x00000000-0x000268c7 sha1: 404303f3960f196f42f8c2c12970ab0d49e28971
/bin/ls: 0x00000000-0x000268c7 sha256: 74ea05150acf311484bddd19c608aa02e6bf3332a0f0805a4deb278e17396354
/bin/ls: 0x00000000-0x000268c7 sha384: c6f811287514ceeeaabe73b5b2f54545036d6fd3a192ea5d6a1fcd494d46151df4117e1c62de0884cbc174c8db008ed1
/bin/ls: 0x00000000-0x000268c7 sha512: 53e4950a150f06d7922a2ed732060e291bf0e1c2ac20bc72a41b9303e1f2837d50643761030d8b918ed05d12993d9515e1ac46676bc0d15ac94d93d8e446fa09
/bin/ls: 0x00000000-0x000268c7 md4: fdfe7c7118a57c1ff8c88a51b16fc78c
/bin/ls: 0x00000000-0x000268c7 xor: 42
/bin/ls: 0x00000000-0x000268c7 xorpair: d391
/bin/ls: 0x00000000-0x000268c7 parity: 00
/bin/ls: 0x00000000-0x000268c7 entropy: 5.95471783
/bin/ls: 0x00000000-0x000268c7 hamdist: 00
/bin/ls: 0x00000000-0x000268c7 pcprint: 22
/bin/ls: 0x00000000-0x000268c7 mod255: ef
/bin/ls: 0x00000000-0x000268c7 xxhash: 76554666
/bin/ls: 0x00000000-0x000268c7 adler32: 7704fe60
/bin/ls: 0x00000000-0x000268c7 luhn: 01
/bin/ls: 0x00000000-0x000268c7 crc8smbus: 8d
/bin/ls: 0x00000000-0x000268c7 crc15can: 1cd5
/bin/ls: 0x00000000-0x000268c7 crc16: d940
/bin/ls: 0x00000000-0x000268c7 crc16hdlc: 7847
/bin/ls: 0x00000000-0x000268c7 crc16usb: 17bb
/bin/ls: 0x00000000-0x000268c7 crc16citt: 67f7
/bin/ls: 0x00000000-0x000268c7 crc24: 3e7053
/bin/ls: 0x00000000-0x000268c7 crc32: c713f78f
/bin/ls: 0x00000000-0x000268c7 crc32c: 6cfba67c
/bin/ls: 0x00000000-0x000268c7 crc32ecma267: b4c809d6
/bin/ls: 0x00000000-0x000268c7 crc32bzip2: a1884a09
/bin/ls: 0x00000000-0x000268c7 crc32d: d1a9533c
/bin/ls: 0x00000000-0x000268c7 crc32mpeg2: 5e77b5f6
/bin/ls: 0x00000000-0x000268c7 crc32posix: 6ba0dec3
/bin/ls: 0x00000000-0x000268c7 crc32q: 3166085c
/bin/ls: 0x00000000-0x000268c7 crc32jamcrc: 38ec0870
/bin/ls: 0x00000000-0x000268c7 crc32xfer: 7504089d
/bin/ls: 0x00000000-0x000268c7 crc64: b6471d3093d94241
/bin/ls: 0x00000000-0x000268c7 crc64ecma: b6471d3093d94241
/bin/ls: 0x00000000-0x000268c7 crc64we: 8fe37d44a47157bd
/bin/ls: 0x00000000-0x000268c7 crc64xz: ea83e12c719e0d79
/bin/ls: 0x00000000-0x000268c7 crc64iso: d243106d9853221c
```
