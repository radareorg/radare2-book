rabin2 allows you to do some modifications/extraction operations on a file. You do so with **-O** option 
and one of the predefined strings to specify the required operation. All the possible strings are:

```
$ rabin2 -O h
Operation string:
  Change Entrypoint: e/0x8048000
  Dump Symbols: d/s/1024
  Dump Section: d/S/.text
  Resize Section: r/.data/1024
  Remove RPATH: R
  Add Library: a/l/libfoo.dylib
  Change Permissions: p/.data/rwx
  Show LDID entitlements: C
```
Let's run some examples.

Change the entry point to 0x8041111 of *somefile.bin*:

``` $rabin2  e/0x8041111  somefile.bin ```

Set permissions rwx (instead of the default rw) on the .bss segment of *somefile.bin*:

```$rabin2 p/.bss/rwx   somefile.bin```

Dump a given section (*.text*) of the file (*somefile.bin*) as a hexadecimal stream:

```$rabin2 d/S/.text somefile.bin```
