# Программа rafind2

Rafind2 — это командная строка библиотеки `r_search`. Она позволяет искать строки, последовательности байтов с двоичными масками и т.д.

```
$ rafind2 -h
Usage: rafind2 [-mXnzZhqv] [-a align] [-b sz] [-f/t from/to] [-[e|s|S] str] [-x hex] -|file|dir ..
 -a [align] only accept aligned hits
 -b [size]  set block size
 -e [regex] search for regex matches (can be used multiple times)
 -f [from]  start searching from address 'from'
 -h         show this help
 -i         identify filetype (r2 -nqcpm file)
 -j         output in JSON
 -m         magic search, file-type carver
 -M [str]   set a binary mask to be applied on keywords
 -n         do not stop on read errors
 -r         print using radare commands
 -s [str]   search for a specific string (can be used multiple times)
 -S [str]   search for a specific wide string (can be used multiple times). Assumes str is UTF-8.
 -t [to]    stop search at address 'to'
 -q         quiet - do not show headings (filenames) above matching contents (default for searching a single file)
 -v         print version and exit
 -x [hex]   search for hexpair string (909090) (can be used multiple times)
 -X         show hexdump of search results
 -z         search for zero-terminated strings
 -Z         show string found on each search hit
```

Вот как его использовать, сначала найдем "lib" внутри двоичного файла `/bin/ls`.
```
$ rafind2 -s lib /bin/ls
0x5f9
0x675
0x679
...
$
```
Обратите внимание, что выходные данные довольно минималистичны и показывают смещения там, где находится строка `lib`. Можно использовать этот вывод для подачи других инструментов.

Результаты подсчета:

```
$ rafind2 -s lib /bin/ls | wc -l
```

Отображение результатов с контекстом:

```
$ export F=/bin/ls
$ for a in `rafind2 -s lib $F` ; do \
    r2 -ns $a -qc'x 32' $F ; done
0x000005f9  6c69 622f 6479 6c64 .. lib/dyld........
0x00000675  6c69 622f 6c69 6275 .. lib/libutil.dyli
0x00000679  6c69 6275 7469 6c2e .. libutil.dylib...
0x00000683  6c69 6200 000c 0000 .. lib......8......
0x000006a5  6c69 622f 6c69 626e .. lib/libncurses.5
0x000006a9  6c69 626e 6375 7273 .. libncurses.5.4.d
0x000006ba  6c69 6200 0000 0c00 .. lib.......8.....
0x000006dd  6c69 622f 6c69 6253 .. lib/libSystem.B.
0x000006e1  6c69 6253 7973 7465 .. libSystem.B.dyli
0x000006ef  6c69 6200 0000 0000 .. lib......&......
```

rafind2 также может быть использован в качестве замены `file` для идентификации mimetype файла с помощью внутренней магической базы данных radare2.

```
$ rafind2 -i /bin/ls
0x00000000 1 Mach-O
```

Также утилита работает как замена `strings`, аналогично тому, что вы делаете с rabin2 -z, но без заботы о разборе заголовков и подчинении двоичным разделам.

```
$ rafind2 -z /bin/ls| grep http
0x000076e5 %http://www.apple.com/appleca/root.crl0\r
0x00007ae6 https://www.apple.com/appleca/0
0x00007fa9 )http://www.apple.com/certificateauthority0
0x000080ab $http://crl.apple.com/codesigning.crl0
```
