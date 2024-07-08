## Yank/Paste

Radare2 has an internal clipboard to save and write portions of memory loaded from the current io layer.

This clipboard can be manipulated with the `y` command.

The two basic operations are

* copy (yank)
* paste

The yank operation will read N bytes (specified by the argument) into the clipboard. We can later use the `yy` command to paste what we read before into a file.

You can yank/paste bytes in visual mode selecting them with the cursor mode (`Vc`) and then using the `y` and `Y` key bindings which are aliases for `y` and `yy` commands of the command-line interface.

```
[0x00000000]> y?
Usage: y[ptxy] [len] [[@]addr]   # See wd? for memcpy, same as 'yf'.
| y!              open cfg.editor to edit the clipboard
| y 16 0x200      copy 16 bytes into clipboard from 0x200
| y 16 @ 0x200    copy 16 bytes into clipboard from 0x200
| y 16            copy 16 bytes into clipboard
| y               show yank buffer information (srcoff len bytes)
| y*              print in r2 commands what's been yanked
| yf 64 0x200     copy file 64 bytes from 0x200 from file
| yfa file copy   copy all bytes from file (opens w/ io)
| yfx 10203040    yank from hexpairs (same as ywx)
| yj              print in JSON commands what's been yanked
| yp              print contents of clipboard
| yq              print contents of clipboard in hexpairs
| ys              print contents of clipboard as string
| yt 64 0x200     copy 64 bytes from current seek to 0x200
| ytf file        dump the clipboard to given file
| yw hello world  yank from string
| ywx 10203040    yank from hexpairs (same as yfx)
| yx              print contents of clipboard in hexadecimal
| yy 0x3344       paste clipboard
| yz [len]        copy nul-terminated string (up to blocksize) into clipboard
```

Sample session:

```
[0x00000000]> s 0x100    ; seek at 0x100
[0x00000100]> y 100      ; yanks 100 bytes from here
[0x00000200]> s 0x200    ; seek 0x200
[0x00000200]> yy         ; pastes 100 bytes
```

You can perform a yank and paste in a single line by just using the `yt` command (yank-to). The syntax is as follows:

```
[0x4A13B8C0]> x
   offset   0 1  2 3  4 5  6 7  8 9  A B  0123456789AB
0x4A13B8C0, 89e0 e839 0700 0089 c7e8 e2ff ...9........
0x4A13B8CC, ffff 81c3 eea6 0100 8b83 08ff ............
0x4A13B8D8, ffff 5a8d 2484 29c2           ..Z.$.).

[0x4A13B8C0]> yt 8 0x4A13B8CC @ 0x4A13B8C0

[0x4A13B8C0]> x
   offset   0 1  2 3  4 5  6 7  8 9  A B  0123456789AB
0x4A13B8C0, 89e0 e839 0700 0089 c7e8 e2ff ...9........
0x4A13B8CC, 89e0 e839 0700 0089 8b83 08ff ...9........
0x4A13B8D8, ffff 5a8d 2484 29c2           ..Z.$.).
```
