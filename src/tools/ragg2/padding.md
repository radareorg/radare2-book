## Padding and Patching

If you want to append/patch some bytes to your output, here are a few options ragg2 offers.

```
 -B [hexpairs]   append some hexpair bytes
 -C [file]       append contents of file
 -d [off:dword]  patch dword (4 bytes) at given offset
 -D [off:qword]  patch qword (8 bytes) at given offset
 -n [dword]      append 32bit number (4 bytes)
 -N [dword]      append 64bit number (8 bytes)
 -p [padding]    add padding after compilation (padding=n10s32)
                 ntas : begin nop, trap, 'a', sequence
                 NTAS : same as above, but at the end
 -S [string]     append a string
 -w [off:hex]    patch hexpairs at given offset
```

