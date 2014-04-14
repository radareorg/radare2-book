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