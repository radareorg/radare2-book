## Mapping Files

Radare's I/O subsystem allows you to map the contents of files into the same I/O space used to contain a loaded binary. New contents can be placed at random offsets.

The `o` command permits the user to open a file, this is mapped at offset 0 unless it has a known binary header and then the maps are created in virtual addresses.

Sometimes, we want to rebase a binary, or maybe we want to load or map the file in a different address.

When launching r2, the base address can be changed with the `-B` flag. But you must notice the difference when opening files with unknown headers, like bootloaders, so we need to map them using the `-m` flag (or specifying it as argument to the `o` command).

radare2 is able to open files and map portions of them at random places in memory specifying attributes like permissions and name. It is the perfect basic tooling to reproduce an environment like a core file, a debug session, by also loading and mapping all the libraries the binary depends on.

Opening files (and mapping them) is done using the `o` (open) command. Let's read the help:

```
[0x00000000]> o?
|Usage: o [com- ] [file] ([offset])
| o                         list opened files
| o-1                       close file descriptor 1
| o-!*                      close all opened files
| o--                       close all files, analysis, binfiles, flags, same as !r2 --
| o [file]                  open [file] file in read-only
| o+ [file]                 open file in read-write mode
| o [file] 0x4000 rwx       map file at 0x4000
| oa[-] [A] [B] [filename]  Specify arch and bits for given file
| oq                        list all open files
| o*                        list opened files in r2 commands
| o. [len]                  open a malloc://[len] copying the bytes from current offset
| o=                        list opened files (ascii-art bars)
| ob[?] [lbdos] [...]       list opened binary files backed by fd
| oc [file]                 open core file, like relaunching r2
| of [file]                 open file and map it at addr 0 as read-only
| oi[-|idx]                 alias for o, but using index instead of fd
| oj[?]                     list opened files in JSON format
| oL                        list all IO plugins registered
| om[?]                     create, list, remove IO maps
| on [file] 0x4000          map raw file at 0x4000 (no r_bin involved)
| oo[?]                     reopen current file (kill+fork in debugger)
| oo+                       reopen current file in read-write
| ood[r] [args]             reopen in debugger mode (with args)
| oo[bnm] [...]             see oo? for help
| op [fd]                   prioritize given fd (see also ob)
| ox fd fdx                 exchange the descs of fd and fdx and keep the mapping
```

Prepare a simple layout:

```sh
$ rabin2 -l /bin/ls
[Linked libraries]
libselinux.so.1
librt.so.1
libacl.so.1
libc.so.6

4 libraries
```

Map a file:

```
[0x00001190]> o /bin/zsh 0x499999
```

List mapped files:

```
[0x00000000]> o
- 6 /bin/ls @ 0x0 ; r
- 10 /lib/ld-linux.so.2 @ 0x100000000 ; r
- 14 /bin/zsh @ 0x499999 ; r
```

Print hexadecimal values from /bin/zsh:

```
[0x00000000]> px @ 0x499999
```

Unmap files using the `o-` command. Pass the required file descriptor to it as an argument:

```
[0x00000000]> o-14
```

You can also view the ascii table showing the list of the opened files:

```
[0x00000000]> ob=
```
