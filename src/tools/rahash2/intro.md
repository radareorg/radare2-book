## rahash2

Versatile command-line hashing tool that is part of the radare2 framework. It's designed to compute and verify cryptographic hashes and checksums for files, strings, or even large data streams like hard disks or network traffic.

Key features of rahash2 include:

**Multiple algorithms** Supports a wide range of hash algorithms, including MD4, MD5, SHA1, SHA256, SHA384, SHA512, CRC16, CRC32, and more.

**Flexible input** Can hash data from files, standard input, or directly from command-line strings.

**Block-based hashing** Can compute hashes for specific blocks or ranges within a file, which is ideal for forensics and checksuming large data.

**Incremental hashing** Supports hashing of data streams or large files in chunks, useful for processing data that doesn't fit in memory.

**Hash verification** and integrity checks comparing computed and provided hash.

**Multiple hash outputs at once** Can compute and display multiple hash types simultaneously for the same input.

**Integration with radare2: While it's a standalone tool, it integrates well with other radare2 utilities and can be used within r2 sessions.

**Customizable output** Offers various output formats, including raw bytes, hexadecimal strings, or radare2 commands.

**Encryption capabilities** Besides hashing, it also supports some basic encryption and decryption operations.

This is an example usage:

```console
$ rahash2 -a md5 -s "hello world"
```

Note that rahash2 also permits to read from stdin in a stream, so you don't need 4GB of ram to compute the hash of a 4GB file.

### Hashing by blocks

When doing forensics, it is useful to compute partial checksums. The reason for that is because you may want to split a huge file into small portions that are easier to identify by contents or regions in the disk.

This will spot the same hash for blocks containing the same contents. For example, if is filled with zeros.

It can also be used to find which blocks have changed between more than one sample dump.

This can be useful when analyzing ram dumps from a virtual machine for example. Use this command for this:

```console
$ rahash2 -B 1M -b -a sha256 /bin/ls
```

### Hashing with rabin2

The rabin2 tool parses the binary headers of the files, but it also have the ability to use the rhash plugins to compute checksum of sections in the binary.

```console
$ rabin2 -K md5 -S /bin/ls
```

### Obtaining hashes within radare2 session

To calculate a checksum of current block when running radare2, use the `ph` command. Pass an algorithm name to it as a parameter. An example session:

```console
$ radare2 /bin/ls
[0x08049790]> bf entry0
[0x08049790]> ph md5
d2994c75adaa58392f953a448de5fba7
```

You can use all hashing algorithms supported by `rahash2`:

```console
[0x00000000]> ph?
md5
sha1
sha256
sha384
sha512
md4
xor
xorpair
parity
entropy
hamdist
pcprint
mod255
xxhash
adler32
luhn
crc8smbus
crc15can
crc16
crc16hdlc
crc16usb
crc16citt
crc24
crc32
crc32c
crc32ecma267
crc32bzip2
crc32d
crc32mpeg2
crc32posix
crc32q
crc32jamcrc
crc32xfer
crc64
crc64ecma
crc64we
crc64xz
crc64iso
```

The `ph` command accepts an optional numeric argument to specify length of byte range to be hashed, instead of default block size. For example:

```console
[0x08049A80]> ph md5 32
9b9012b00ef7a94b5824105b7aaad83b
[0x08049A80]> ph md5 64
a71b087d8166c99869c9781e2edcf183
[0x08049A80]> ph md5 1024
a933cc94cd705f09a41ecc80c0041def
```
