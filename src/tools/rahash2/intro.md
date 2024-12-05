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
$ rahash2 -B -b 1M -a sha256 /bin/ls
```

### Hashing with rabin2

The rabin2 tool parses the binary headers of the files, but it also have the ability to use the rhash plugins to compute checksum of sections in the binary.

```console
$ rabin2 -K md5 -S /bin/ls
```

### Obtaining hashes within radare2 session

Hashes can also be printed within a radare2 session as explained in the [Print Section](../../commandline/print_modes.md).
