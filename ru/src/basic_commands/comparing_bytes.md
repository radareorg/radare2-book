## Сравнение байтов

Для большинства общих задач обратного проектирования, таких как поиск различий между двумя двоичными файлами, какие байты были изменены, поиск различий в графах результатов анализа кода и другие операции сравнения, можно реализовывать при помощи radiff2:

```
$ radiff2 -h
```

Внутри r2 функциональные возможности, предоставляемые radiff2, доступны при помощи команды `c`.

Команда `c` (сокращение от «compare») позволяет сравнивать массивы байтов из разных источников. Команда принимает ввод в нескольких форматах, а затем сравнивает его со значениями, найденными в текущей позиции поиска.

```
[0x00404888]> c?
Usage: c[?dfx] [argument]   # Compare
| c [string]               Compare a plain with escaped chars string
| c* [string]              Same as above, but printing r2 commands instead
| c1 [addr]                Compare 8 bits from current offset
| c2 [value]               Compare a word from a math expression
| c4 [value]               Compare a doubleword from a math expression
| c8 [value]               Compare a quadword from a math expression
| cat [file]               Show contents of file (see pwd, ls)
| cc [at]                  Compares in two hexdump columns of block size
| ccc [at]                 Same as above, but only showing different lines
| ccd [at]                 Compares in two disasm columns of block size
| ccdd [at]                Compares decompiler output (e cmd.pdc=pdg|pdd)
| cf [file]                Compare contents of file at current seek
| cg[?] [o] [file]         Graphdiff current file and [file]
| cu[?] [addr] @at         Compare memory hexdumps of $$ and dst in unified diff
| cud [addr] @at           Unified diff disasm from $$ and given address
| cv[1248] [hexpairs] @at  Compare 1,2,4,8-byte (silent return in $?)
| cV[1248] [addr] @at      Compare 1,2,4,8-byte address contents (silent, return in $?)
| cw[?] [us?] [...]        Compare memory watchers
| cx [hexpair]             Compare hexpair string (use '.' as nibble wildcard)
| cx* [hexpair]            Compare hexpair string (output r2 commands)
| cX [addr]                Like 'cc' but using hexdiff output
| cd [dir]                 chdir
| cl|cls|clear             Clear screen, (clear0 to goto 0, 0 only)
```

Чтобы сравнить содержимое памяти в текущей позиции поиска с заданной строкой значений, используйте`cx`:

```
[0x08048000]> p8 4
7f 45 4c 46

[0x08048000]> cx 7f 45 90 46
Compare 3/4 equal bytes
0x00000002 (byte=03)   90 ' '  ->  4c 'L'
[0x08048000]>
```

Еще одна команда группы `с` - `cc` «compare code».
Сравнение последовательности байтов с последовательностью в памяти:

```
[0x4A13B8C0]> cc 0x39e8e089 @ 0x4A13B8C0
```

Сравнение содержимого двух функций, указанных по именам:

```
[0x08049A80]> cc sym.main2 @ sym.main
```

Команда `с8` сравнивает quadword по текущему смещению (в приведенном ниже примере 0x00000000) с математическим выражением:

```
[0x00000000]> c8 4

Compare 1/8 equal bytes (0%)
0x00000000 (byte=01)   7f ' '  ->  04 ' '
0x00000001 (byte=02)   45 'E'  ->  00 ' '
0x00000002 (byte=03)   4c 'L'  ->  00 ' '
```

Параметр number может быть математическим выражением, в котором используются имена флагов и все, что разрешено в выражении:

```
[0x00000000]> cx 7f469046

Compare 2/4 equal bytes
0x00000001 (byte=02)   45 'E'  ->  46 'F'
0x00000002 (byte=03)   4c 'L'  ->  90 ' '
```

Вы можете использовать команду сравнения, чтобы найти различия между текущим блоком и файлом, ранее сброшенным на диск:

```
r2 /bin/true
[0x08049A80]> s 0
[0x08048000]> cf /bin/true
Compare 512/512 equal bytes
```
