# Obtaining Hashes within Radare2 Session

To calculate a checksum of current block when running rarare2, use the '#' command. Pass an algorithm name to it as a parameter. An example session:

    $ radare2 /bin/ls
    [0x08049790]> bf entry0
    [0x08049790]> #md5
    d2994c75adaa58392f953a448de5fba7

You can use all hashing algorithms supported by `rahash2`: md4, md5, crc16, crc32, sha1, sha256, sha384, sha512, par, xor, xorpair, mod255, hamdist, entropy.
The `#` command accepts an optional numeric argument to specify length of byte range to be hashed, instead of default block size. For example:

    [0x08049A80]> #md5 32
    9b9012b00ef7a94b5824105b7aaad83b
    [0x08049A80]> #md5 64
    a71b087d8166c99869c9781e2edcf183
    [0x08049A80]> #md5 1024
    a933cc94cd705f09a41ecc80c0041def
    [0x08049A80]> 
