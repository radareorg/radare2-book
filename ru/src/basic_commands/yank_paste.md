## Сохранение/Вставка

Radare2 имеет свой буфер обмена для сохранения и записи частей памяти, загруженных из текущего слоя ввода-вывода.

Этим буфером обмена можно манипулировать с помощью команды `y` (yank).

Двумя основными операциями являются:

* Копирование (yank)
* Вставка (paste)

Операция  yank прочитает N байт (заданных аргументом) в буфер обмена. Затем используя команду `yy`, делаем вставку сохраненного ранее блока в файл.

Можно сохранять/втавлять (yank/paste) байты в визуальном режиме, выбирая их в режиме курсора (`Vc`), а затем используя привязки клавиш `y` и `Y`, являющиеся псевдонимами для команд `y` и `yy` интерфейса командной строки.

```
[0x00000000]> y?
Usage: y[ptxy] [len] [[@]addr]   # See wd? для memcpy, то же самое, что и 'yf'.
| y!              открытие cfg.editor для редактирования содержания буфера обмена
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

Пример сессии:

```
[0x00000000]> s 0x100    ; seek at 0x100
[0x00000100]> y 100      ; yanks 100 bytes from here
[0x00000200]> s 0x200    ; seek 0x200
[0x00000200]> yy         ; pastes 100 bytes
```

Можно выполнить сохранение и вставку в одной строке, используя команду `yt` (yank-to). Вот её синтаксис:

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
