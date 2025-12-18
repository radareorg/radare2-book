## First Steps

This chapter covers practical usage of r2renef with detailed command examples and workflows.

## Radare2 Commands

Once connected to a process, you can use standard radare2 commands on live memory.

### Disassembly

Disassemble at current position:

```console
[0x7f8a1c2b0]> pd 10
            0x7f8a1c2b0      fd7bbfa9       stp x29, x30, [sp, -0x10]!
            0x7f8a1c2b4      fd030091       mov x29, sp
            0x7f8a1c2b8      00000094       bl sym.init_function
            0x7f8a1c2bc      e00313aa       mov x0, x19
            0x7f8a1c2c0      fd7bc1a8       ldp x29, x30, [sp], 0x10
            0x7f8a1c2c4      c0035fd6       ret
```

Analyze and disassemble a function:

```console
[0x7f8a1c2b0]> af
[0x7f8a1c2b0]> pdf
```

### Hex Dump

```console
[0x7f8a1c2b0]> px 64
- offset -   0 1  2 3  4 5  6 7  8 9  A B  C D  E F  0123456789ABCDEF
0x7f8a1c2b0  7f45 4c46 0201 0100 0000 0000 0000 0000  .ELF............
0x7f8a1c2c0  0300 b700 0100 0000 0000 0000 0000 0000  ................
0x7f8a1c2d0  4000 0000 0000 0000 0000 0000 0000 0000  @...............
0x7f8a1c2e0  0000 0000 4000 3800 0900 4000 1e00 1d00  ....@.8...@.....
```

### Seeking

```console
[0x7f8a1c2b0]> s 0x7f8a1c000
[0x7f8a1c000]>
```

### Writing Memory

Write bytes at current position:

```console
[0x7f8a1c2b0]> w 1f2003d5
```

## Renef Commands

All Renef-specific commands use the `:` prefix.

### Listing Applications

Show all installed applications:

```console
[0x7f8a1c2b0]> :la
com.android.settings
com.android.chrome
com.example.app
...
```

Filter applications by name:

```console
[0x7f8a1c2b0]> :la~chrome
com.android.chrome
```

### Memory Dump

Use Renef's memory dump command:

```console
[0x7f8a1c2b0]> :md 0x78b0a99060 64
Memory at 0x78b0a99060 (64 bytes):
0x78b0a99060:  ff ff ff ff ff 44 0e 30 48 0c 1d 10 9e 02 9d 04  |.....D.0H.......|
0x78b0a99070:  0a 02 78 0c 1f 30 48 0e 00 de dd 44 0b 00 00 00  |..x..0H....D....|
0x78b0a99080:  30 00 00 00 5c 0e 00 00 bc b8 01 00 d4 00 00 00  |0...\...........|
0x78b0a99090:  08 6b 92 fc ff ff ff ff ff 44 0e 80 01 48 0c 1d  |.k.......D...H..|
```

### Memory Search

Scan memory for hex patterns:

```console
[0x7f8a1c2b0]> :ms 736563726574
[+] Found at 0x7a5c150000 (libexample.so+0x50000)
```

### Execute Lua Code

Run Lua code directly:

```console
[0x7f8a1c2b0]> :exec print("Hello from r2renef!")
Hello from r2renef!
```

```console
[0x7f8a1c2b0]> :exec Module.list()
```

```console
[0x7f8a1c2b0]> :exec Module.findExport("encrypt_buffer", "libexample.so")
```

### Loading Lua Scripts

Load a script file:

```console
[0x7f8a1c2b0]> :l /path/to/script.lua
[+] Script loaded
```

### Hook Management

List active hooks:

```console
[0x7f8a1c2b0]> :hooks
[1] 0x7a5c150000 SSL_read (libssl.so)
[2] 0x7a5c151000 SSL_write (libssl.so)
```

Remove a hook:

```console
[0x7f8a1c2b0]> :unhook 1
[+] Hook removed
```

Monitor hooks in real-time:

```console
[0x7f8a1c2b0]> :watch
[*] Watching hooks... (Ctrl+C to stop)
[0x7a5c150000] SSL_read called, size=1024
[0x7a5c150000] SSL_read returned, bytes=512
```

## Practical Workflow

### Finding and Analyzing a Function

1. Find the function address:

```console
[0x7f8a1c2b0]> :exec Module.findExport("encrypt_buffer", "libexample.so")
```

1. Navigate and analyze with radare2:

```console
[0x7f8a1c2b0]> s 0x7a5c102000
[0x7a5c102000]> af
[0x7a5c102000]> pdf
```

1. Hook the function via Lua script and load with `:l`.

### Example: Monitoring Library Loading

Hook the linker to monitor when libraries are loaded:

```lua
-- dlopen_monitor.lua
local linker_name = "linker64"
local linker_base = Module.find(linker_name)

if linker_base then
    print(string.format("[+] %s found at: 0x%x", linker_name, linker_base))

    local linker_symbols = Module.symbols(linker_name)
    for _, sym in ipairs(linker_symbols) do
        if sym.name:find("do_dlopen") then
            print(string.format("[+] Hooking %s at offset 0x%x", sym.name, sym.offset))

            hook(linker_name, sym.offset, {
                onEnter = function(args)
                    local path = Memory.readString(args[0])
                    if path then
                        print("[dlopen] Loading: " .. path)
                    end
                end
            })
            break
        end
    end
end
```

Load and watch:

```console
[0x7f8a1c2b0]> :l dlopen_monitor.lua
[+] linker64 found at: 0x7a5c000000
[+] Hooking do_dlopen at offset 0x1234
[0x7f8a1c2b0]> :watch
[dlopen] Loading: /data/app/com.example.app/lib/arm64/libflutter.so
[dlopen] Loading: /system/lib64/libssl.so
```

### Runtime Patching

Patch a conditional jump to NOP:

```console
[0x7a5c105000]> pd 2
            0x7a5c105000      1f0000f1       cmp x0, 0
            0x7a5c105004      41010054       b.ne 0x7a5c10502c

[0x7a5c105004]> w 1f2003d5
[0x7a5c105004]> pd 1
            0x7a5c105004      1f2003d5       nop
```
