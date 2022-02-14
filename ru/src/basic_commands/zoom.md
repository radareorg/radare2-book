## Zoom

The zoom is a print mode that allows you to get a global view of the whole file or a memory map on a single screen. In this mode, each byte represents `file_size/block_size` bytes of the file. Use the `pz` command, or just use `Z` in the visual mode to toggle the zoom mode.

The cursor can be used to scroll faster through the zoom out view. Pressing `z` again will zoom-in at the cursor position.

```
[0x004048c5]> pz?
|Usage: pz [len] print zoomed blocks (filesize/N)
| e zoom.maxsz  max size of block
| e zoom.from   start address
| e zoom.to     end address
| e zoom.byte   specify how to calculate each byte
| pzp           number of printable chars
| pzf           count of flags in block
| pzs           strings in range
| pz0           number of bytes with value '0'
| pzF           number of bytes with value 0xFF
| pze           calculate entropy and expand to 0-255 range
| pzh           head (first byte value); This is the default mode
```

Let's see some examples:

```
[0x08049790]> e zoom.byte=h
[0x08049790]> pz // or default pzh
0x00000000  7f00 0000 e200 0000 146e 6f74 0300 0000
0x00000010  0000 0000 0068 2102 00ff 2024 e8f0 007a
0x00000020  8c00 18c2 ffff 0080 4421 41c4 1500 5dff
0x00000030  ff10 0018 0fc8 031a 000c 8484 e970 8648
0x00000040  d68b 3148 348b 03a0 8b0f c200 5d25 7074
0x00000050  7500 00e1 ffe8 58fe 4dc4 00e0 dbc8 b885
```
```
[0x08049790]> e zoom.byte=p
[0x08049790]> pz // or pzp
0x00000000  2f47 0609 070a 0917 1e9e a4bd 2a1b 2c27
0x00000010  322d 5671 8788 8182 5679 7568 82a2 7d89
0x00000020  8173 7f7b 727a 9588 a07b 5c7d 8daf 836d
0x00000030  b167 6192 a67d 8aa2 6246 856e 8c9b 999f
0x00000040  a774 96c3 b1a4 6c8e a07c 6a8f 8983 6a62
0x00000050  7d66 625f 7ea4 7ea6 b4b6 8b57 a19f 71a2
```
```
[0x08049790]> eval zoom.byte = flags
[0x08049790]> pz // or pzf
0x00406e65  48d0 80f9 360f 8745 ffff ffeb ae66 0f1f
0x00406e75  4400 0083 f801 0f85 3fff ffff 410f b600
0x00406e85  3c78 0f87 6301 0000 0fb6 c8ff 24cd 0026
0x00406e95  4100 660f 1f84 0000 0000 0084 c074 043c
0x00406ea5  3a75 18b8 0500 0000 83f8 060f 95c0 e9cd
0x00406eb5  feff ff0f 1f84 0000 0000 0041 8801 4983
0x00406ec5  c001 4983 c201 4983 c101 e9ec feff ff0f
```
```
[0x08049790]> e zoom.byte=F
[0x08049790]> pO // or pzF
0x00000000  0000 0000 0000 0000 0000 0000 0000 0000
0x00000010  0000 2b5c 5757 3a14 331f 1b23 0315 1d18
0x00000020  222a 2330 2b31 2e2a 1714 200d 1512 383d
0x00000030  1e1a 181b 0a10 1a21 2a36 281e 1d1c 0e11
0x00000040  1b2a 2f22 2229 181e 231e 181c 1913 262b
0x00000050  2b30 4741 422f 382a 1e22 0f17 0f10 3913
```

You can limit zooming to a range of bytes instead of the whole bytespace. Change `zoom.from` and `zoom.to` eval variables:

```
[0x00003a04]> e? zoom.
zoom.byte: Zoom callback to calculate each byte (See pz? for help)
zoom.from: Zoom start address
zoom.in: Specify  boundaries for zoom
zoom.maxsz: Zoom max size of block
zoom.to: Zoom end address
[0x00003a04]> e zoom.
zoom.byte = h
zoom.from = 0
zoom.in = io.map
zoom.maxsz = 512
zoom.to = 0
```