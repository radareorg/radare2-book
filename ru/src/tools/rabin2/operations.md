rabin2 позволяет выполнять некоторые операции модификации/извлечения файла. Делается это с помощью флага **-O** и одной из предопределенных строк для указания требуемой операции. Все возможные строки:

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
Давайте рассмотрим несколько примеров.

Изменить точку входа на 0x8041111 в файле *somefile.bin*:

``` $rabin2  e/0x8041111  somefile.bin ```

Установка разрешения rwx (вместо rw по умолчанию) для сегмента .bss файла *somefile.bin*:

```$rabin2 p/.bss/rwx   somefile.bin```

Сделать дамп заданного раздела (*.text*) файла (*somefile.bin*) в виде шестнадцатеричного потока:

```$rabin2 d/S/.text somefile.bin```
