#Chapter 18: Rahash2

The it is quite easy to calculate a hash checksum of the current block using the '#' command.


    $ radare2 /bin/ls
    [0x08049790]> bf entry0
    [0x08049790]> #md5
    d2994c75adaa58392f953a448de5fba7

In the same way you can also calculate other hashing algorithms that are supported by 'rahash': md4, md5, crc16, crc32, sha1, sha256, sha384, sha512, par, xor, xorpair, mod255, hamdist, entropy, all.

The '#' command can accept a numeric argument to define the length in bytes to be hashed.

    [0x08049A80]> #md5 32
    9b9012b00ef7a94b5824105b7aaad83b
    [0x08049A80]> #md5 64
    a71b087d8166c99869c9781e2edcf183
    [0x08049A80]> #md5 1024
    a933cc94cd705f09a41ecc80c0041def
    [0x08049A80]> 

##18.1 Rahash2 tool

The rahash tool is the used by radare to realize these calculations. It

    $ rahash2 -h
    Usage: rahash2 [-rBhLkv] [-b sz] [-a algo] [-s str] [-f from] [-t to] [file] ...
    -a algo     comma separated list of algorithms (default is 'sha256')
    -b bsize    specify the size of the block (instead of full file)
    -B          show per-block hash
    -e          swap endian (use little endian)
    -f from     start hashing at given address
    -i num      repeat hash N iterations
    -S seed     use given seed (hexa or s:string) use ^ to prefix
    -k          show hash using the openssh's randomkey algorithm
    -q          run in quiet mode (only show results)
    -L          list all available algorithms (see -a)
    -r          output radare commands
    -s string   hash this string instead of files
    -t to       stop hashing at given address
    -v          show version information

It permits the calculation of the hashes from strings or files.

    $ rahash2 -q -a md5 -s 'hello world'
    5eb63bbbe01eeed093cb22bb8f5acdc3

It is possible to hash the full contents of a file . But dont do this for large files like disks or so, because rahash stores the buffer in memory before calculating the checksum instead of doing it progressively.

    $ rahash2 -a all /bin/ls
    /bin/ls: 0x00000000-0x0001ae08 md5: b5607b4dc7d896c0fab5c4a308239161
    /bin/ls: 0x00000000-0x0001ae08 sha1: c8f5032c2dce807c9182597082b94f01a3bec495
    /bin/ls: 0x00000000-0x0001ae08 sha256: 978317d58e3ed046305df92a19f7d3e0bfcb3c70cad979f24fee289ed1d266b0
    /bin/ls: 0x00000000-0x0001ae08 sha384: 9e946efdbebb4e0ca00c86129ce2a71ee734ac30b620336c381aa929dd222709e4cf7a800b25fbc7d06fe3b184933845
    /bin/ls: 0x00000000-0x0001ae08 sha512: 076806cedb5281fd15c21e493e12655c55c52537fc1f36e641b57648f7512282c03264cf5402b1b15cf03a20c9a60edfd2b4f76d4905fcec777c297d3134f41f
    /bin/ls: 0x00000000-0x0001ae08 crc16: 4b83
    /bin/ls: 0x00000000-0x0001ae08 crc32: 6e316348
    /bin/ls: 0x00000000-0x0001ae08 md4: 3a75f925a6a197d26bc650213f12b074
    /bin/ls: 0x00000000-0x0001ae08 xor: 3e
    /bin/ls: 0x00000000-0x0001ae08 xorpair: 59
    /bin/ls: 0x00000000-0x0001ae08 parity: 01
    /bin/ls: 0x00000000-0x0001ae08 entropy: 0567f925
    /bin/ls: 0x00000000-0x0001ae08 hamdist: 00
    /bin/ls: 0x00000000-0x0001ae08 pcprint: 23
    /bin/ls: 0x00000000-0x0001ae08 mod255: 1e
    /bin/ls: 0x00000000-0x0001ae08 xxhash: 138c936d
    /bin/ls: 0x00000000-0x0001ae08 adler32: fca7131b