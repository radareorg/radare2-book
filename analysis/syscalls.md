# Syscalls

Radare2 allows manual search for assembly code looking like a syscall operation.
For example on ARM platform usually they are represented by the `svc` instruction,
on the others can be a different instructions, e.g. `syscall` on x86 PC.
```
[0x0001ece0]> /ad/ svc
...
0x000187c2   # 2: svc 0x76
0x000189ea   # 2: svc 0xa9
0x00018a0e   # 2: svc 0x82
...
```
Syscalls detection is driven by `asm.os`, `asm.bits`, and `asm.arch`. Be sure
to setup those configuration options accordingly. You can use `asl` command
to check if syscalls' support is set up properly and as you expect.
The command lists syscalls supported for your platform.
```
[0x0001ece0]> asl
...
sd_softdevice_enable = 0x80.16
sd_softdevice_disable = 0x80.17
sd_softdevice_is_enabled = 0x80.18
...
```

If you setup ESIL stack with `aei` or `aeim`, you can use `/as` command to search
the addresses where particular syscalls were found and list them.
```
[0x0001ece0]> aei
[0x0001ece0]> /as
0x000187c2 sd_ble_gap_disconnect
0x000189ea sd_ble_gatts_sys_attr_set
0x00018a0e sd_ble_gap_sec_info_reply
...
```
To reduce searching time it is possible to [restrict the
searching](../search_bytes/configurating_the_search.md) range for
only executable segments or sections with `/as @e:search.in=io.maps.x`

Using the [ESIL emulation](emulation.md) radare2 can print syscall arguments
in the disassembly output. To enable the linear (but very rough) emulation use
`asm.emu` configuration variable:
```
[0x0001ece0]> e asm.emu=true
[0x0001ece0]> s 0x000187c2
[0x000187c2]> pdf~svc
   0x000187c2   svc 0x76  ; 118 = sd_ble_gap_disconnect
[0x000187c2]>
```

In case of executing `aae` (or `aaaa` which calls `aae`) command
radare2 will push found syscalls to a special `syscall.` flagspace,
which can be useful for automation purpose:
```
[0x000187c2]> fs
0    0 * imports
1    0 * symbols
2 1523 * functions
3  420 * strings
4  183 * syscalls
[0x000187c2]> f~syscall
...
0x000187c2 1 syscall.sd_ble_gap_disconnect.0
0x000189ea 1 syscall.sd_ble_gatts_sys_attr_set
0x00018a0e 1 syscall.sd_ble_gap_sec_info_reply
...
```

It also can be interactively navigated through within HUD mode (`V_`)
```
0> syscall.sd_ble_gap_disconnect
 - 0x000187b2  syscall.sd_ble_gap_disconnect
   0x000187c2  syscall.sd_ble_gap_disconnect.0
   0x00018a16  syscall.sd_ble_gap_disconnect.1
   0x00018b32  syscall.sd_ble_gap_disconnect.2
   0x0002ac36  syscall.sd_ble_gap_disconnect.3
```
