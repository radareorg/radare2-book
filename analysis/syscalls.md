## Syscalls

You can manually search for assembly that looks like a syscall operation. On my platform syscalls are called with the `svc` op, though yours might be `syscall`
```
[0x0001ece0]> /c svc
...
0x000187c2   # 2: svc 0x76
0x000189ea   # 2: svc 0xa9
0x00018a0e   # 2: svc 0x82
...
```
Syscalls detection is driven by `asm.os` `asm.bits` and `asm.arch` so these all need to be set up properly. Do `asl` to see if syscall support is set up properly and as you expect on your system, this is the list of syscalls radare supports for your platform.
```
[0x0001ece0]> asl
...
sd_softdevice_enable = 0x80.16
sd_softdevice_disable = 0x80.17
sd_softdevice_is_enabled = 0x80.18
...
```

Radare will do a `/c` style search for you and annotate calls if you have an esil stack set up with for instance `aei` or `aeim`
```
[0x0001ece0]> aei
[0x0001ece0]> /as
0x000187c2 sd_ble_gap_disconnect
0x000189ea sd_ble_gatts_sys_attr_set
0x00018a0e sd_ble_gap_sec_info_reply
...
```
this takes forever on my binaries so assuming I have permission set up on the maps, I search in executable code only with `/as @e:search.in=io.maps.x`

You can enable more output so it prints arguments for you for you in the Visual and along side dissaembled code by doing `asm.emu=1`
```
[0x0001ece0]> e asm.emu=1
[0x0001ece0]> s 0x000187c2
[0x000187c2]> pdf~svc
| |   0x000187c2   svc 0x76  ; 118 = sd_ble_gap_disconnect
[0x000187c2]> 
```

If you used `aae` (or `aaaa` which calls `aae`) it'll stick syscalls in a flagspace which is super handy to list or search them
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

or you can interactively search through the strings/flags in the system with `V_`
```
0> syscall.sd_ble_gap_disconnect
 - 0x000187b2  syscall.sd_ble_gap_disconnect
   0x000187c2  syscall.sd_ble_gap_disconnect.0
   0x00018a16  syscall.sd_ble_gap_disconnect.1
   0x00018b32  syscall.sd_ble_gap_disconnect.2
   0x0002ac36  syscall.sd_ble_gap_disconnect.3
```
