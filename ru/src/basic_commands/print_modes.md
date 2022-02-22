# Режимы отображения данных

Одной из ключевых особенностей radare2 является отображение информации во многих форматах. Цель состоит в том, чтобы предложить выбор вариантов отображения для наилучшей интерпретации двоичных данных.

Двоичные данные представляются в виде целых чисел, shorts, longs, floats, timestamps, шестнадцатеричных строк или более сложных форматов, таких как структуры C, списки дизассемблирования, списки декомпиляции, результатов внешней обработки ...

Ниже приведен список доступных режимов печати, перечисленных `в p?`:

```
[0x00005310]> p?
|Usage: p[=68abcdDfiImrstuxz] [arg|len] [@addr]
| p[b|B|xb] [len] ([S])   bindump N bits skipping S bytes
| p[iI][df] [len]         print N ops/bytes (f=func) (see pi? and pdi)
| p[kK] [len]             print key in randomart (K is for mosaic)
| p-[?][jh] [mode]        bar|json|histogram blocks (mode: e?search.in)
| p2 [len]                8x8 2bpp-tiles
| p3 [file]               print stereogram (3D)
| p6[de] [len]            base64 decode/encode
| p8[?][j] [len]          8bit hexpair list of bytes
| p=[?][bep] [N] [L] [b]  show entropy/printable chars/chars bars
| pa[edD] [arg]           pa:assemble  pa[dD]:disasm or pae: esil from hex
| pA[n_ops]               show n_ops address and type
| pb[?] [n]               bitstream of N bits
| pB[?] [n]               bitstream of N bytes
| pc[?][p] [len]          output C (or python) format
| pC[aAcdDxw] [rows]      print disassembly in columns (see hex.cols and pdi)
| pd[?] [sz] [a] [b]      disassemble N opcodes (pd) or N bytes (pD)
| pf[?][.nam] [fmt]       print formatted data (pf.name, pf.name $<expr>)
| pF[?][apx]              print asn1, pkcs7 or x509
| pg[?][x y w h] [cmd]    create new visual gadget or print it (see pg? for details)
| ph[?][=|hash] ([len])   calculate hash for a block
| pj[?] [len]             print as indented JSON
| pm[?] [magic]           print libmagic data (see pm? and /m?)
| po[?] hex               print operation applied to block (see po?)
| pp[?][sz] [len]         print patterns, see pp? for more help
| pq[?][is] [len]         print QR code with the first Nbytes
| pr[?][glx] [len]        print N raw bytes (in lines or hexblocks, 'g'unzip)
| ps[?][pwz] [len]        print pascal/wide/zero-terminated strings
| pt[?][dn] [len]         print different timestamps
| pu[?][w] [len]          print N url encoded bytes (w=wide)
| pv[?][jh] [mode]        show variable/pointer/value in memory
| pwd                     display current working directory
| px[?][owq] [len]        hexdump of N bytes (o=octal, w=32bit, q=64bit)
| pz[?] [len]             print zoom view (see pz? for help)
[0x00005310]>
```

Совет: при использовании вывода json можно добавлять `~{}` к команде, чтобы получить красиво напечатанную версию вывода:

```
[0x00000000]> oj
[{"raised":false,"fd":563280,"uri":"malloc://512","from":0,"writable":true,"size":512,"overlaps":false}]
[0x00000000]> oj~{}
[
	{
		"raised": false,
		"fd": 563280,
		"uri": "malloc://512",
		"from": 0,
		"writable": true,
		"size": 512,
		"overlaps": false
	}
]
```

Для получения дополнительной информации о функциональным возможностям `~` смотрите инструкцию в `?@?`, а также главу [Формат команды](../first_steps/command_format.md) в книге.

### Шестнадцатеричный вид

`px` дает удобный вывод, показывающий 16 пар чисел в строке со смещениями и raw-представлениями:

![hexprint](print_modes_px.png)

#### Шестнадцатеричный дамп слов (32 бита)

![wordprint](print_modes_pxw.png)

#### 8-битовый шестнадцатеричный список байтов

```
[0x00404888]> p8 16
31ed4989d15e4889e24883e4f0505449
```

#### Шестнадцатеричный дамп из Quad-слов (64 бита)

![pxq](print_modes_pxq.png)

### Форматы даты/времени

В настоящее время поддерживаются следующие режимы вывода timestamp-ов:

```
[0x00404888]> pt?
|Usage: pt [dn]  print timestamps
| pt.  print current time
| pt   print UNIX time (32 bit `cfg.bigendian`) Since January 1, 1970
| ptd  print DOS time (32 bit `cfg.bigendian`) Since January 1, 1980
| pth  print HFS time (32 bit `cfg.bigendian`) Since January 1, 1904
| ptn  print NTFS time (64 bit `cfg.bigendian`) Since January 1, 1601
```

Например, вы можете представить текущий буфер как временные метки во времени NTFS:

```
[0x08048000]> e cfg.bigendian = false
[0x08048000]> pt 4
29:04:32948 23:12:36 +0000
[0x08048000]> e cfg.bigendian = true
[0x08048000]> pt 4
20:05:13001 09:29:21 +0000
```

Как видите, порядок байтов влияет на результат. После того, как вы напечатали timestamp, можно профильтовать grep-ом выходные данные, например, по годам:

```
[0x08048000]> pt ~1974 | wc -l
15
[0x08048000]> pt ~2022
27:04:2022 16:15:43 +0000
```

Формат даты по умолчанию можно настроить с помощью переменной `cfg.datefmt`. Правила форматирования следуют хорошо известному формату strftime(3). Ознакомьтесь со справочной страницей, вот наиболее важное:

```
%a  Сокращенное название дня недели в соответствии с текущим языковым стандартом.
%A  Полное название дня недели в соответствии с текущим языковым стандартом.
%d  День месяца в виде десятичного числа (диапазон от 01 до 31).
%D  Эквивалентно %m/%d/%y.  (Yecch — только для американцев).
%H  Час в виде десятичного числа с использованием 24-часового формата (диапазон от 00 до 23).
%I  Час в виде десятичного числа с использованием 12-часового формата (диапазон от 01 до 12).
%m  Месяц в виде десятичного числа (диапазон от 01 до 12).
%M  Минута в виде десятичного числа (диапазон от 00 до 59).
%p  Либо "AM", либо "PM" в соответствии с заданным значением времени.
%s  Количество секунд со времен Эпохи, 1970-01-01 00:00:00 +0000 (UTC). (ТЗ)
%S Секунда в виде десятичного числа (диапазон от 00 до 60).  (Диапазон составляет до 60 секунд, что позволяет время от времени использовать високосные секунды.)
%T  Время в 24-часовой нотации (%H:%M:%S).  (SU)
%y  Год в виде десятичного числа без века (диапазон от 00 до 99).
%Y  Год в виде десятичного числа, включая век.
%z  Числовой часовой пояс +hhmm или -hhmm (то есть часовое и минутное смещение от UTC). (SU)
%Z  Название или аббревиатура часового пояса.
```

### Основные типы.

Существуют режимы печати, доступные для всех базовых типов. Если вас интересует более сложная структура, введите `pf??` для символов формата и `pf???` для примеров:

```
[0x00499999]> pf??
|pf: pf[.k[.f[=v]]|[v]]|[n]|[0|cnt][fmt] [a0 a1 ...]
| Format:
|  b       byte (unsigned)
|  B       resolve enum bitfield (see t?)
|  c       char (signed byte)
|  C       byte in decimal
|  d       0xHEX value (4 bytes) (see 'i' and 'x')
|  D       disassemble one opcode
|  e       temporally swap endian
|  E       resolve enum name (see t?)
|  f       float value (4 bytes)
|  F       double value (8 bytes)
|  i       signed integer value (4 bytes) (see 'd' and 'x')
|  n       next char specifies size of signed value (1, 2, 4 or 8 byte(s))
|  N       next char specifies size of unsigned value (1, 2, 4 or 8 byte(s))
|  o       octal value (4 byte)
|  p       pointer reference (2, 4 or 8 bytes)
|  q       quadword (8 bytes)
|  r       CPU register `pf r (eax)plop`
|  s       32bit pointer to string (4 bytes)
|  S       64bit pointer to string (8 bytes)
|  t       UNIX timestamp (4 bytes)
|  T       show Ten first bytes of buffer
|  u       uleb128 (variable length)
|  w       word (2 bytes unsigned short in hex)
|  x       0xHEX value and flag (fd @ addr) (see 'd' and 'i')
|  X       show formatted hexpairs
|  z       null terminated string
|  Z       null terminated wide string
|  ?       структура данных 'pf ? (struct_name)example_name`
|  *       next char is pointer (honors asm.bits)
|  +       toggle show flags for each offset
|  :       skip 4 bytes
|  .       skip 1 byte
|  ;       rewind 4 bytes
|  ,       rewind 1 byte
```

Используйте тройной вопросительный знак `pf???`, чтобы получить некоторые примеры с использованием строк формата печати.

```
[0x00499999]> pf???
|pf: pf[.k[.f[=v]]|[v]]|[n]|[0|cnt][fmt] [a0 a1 ...]
| Examples:
| pf 3xi foo bar                               3-array of struct, each with named fields: 'foo' as hex, and 'bar' as int
| pf B (BitFldType)arg_name`                   bitfield type
| pf E (EnumType)arg_name`                     enum type
| pf.obj xxdz prev next size name              Define the obj format as xxdz
| pf obj=xxdz prev next size name              Same as above
| pf *z*i*w nb name blob                       Print the pointers with given labels
| pf iwq foo bar troll                         Print the iwq format with foo, bar, troll as the respective names for the fields
| pf 0iwq foo bar troll                        Same as above, but considered as a union (all fields at offset 0)
| pf.plop ? (troll)mystruct                    Use structure troll previously defined
| pfj.plop @ 0x14                              Apply format object at the given offset
| pf 10xiz pointer length string               Print a size 10 array of the xiz struct with its field names
| pf 5sqw string quad word                     Print an array with sqw struct along with its field names
| pf {integer}? (bifc)                         Print integer times the following format (bifc)
| pf [4]w[7]i                                  Print an array of 4 words and then an array of 7 integers
| pf ic...?i foo bar "(pf xw yo foo)troll" yo  Print nested anonymous structures
| pf ;..x                                      Print value located 6 bytes from current offset
| pf [10]z[3]i[10]Zb                           Print an fixed size str, widechar, and var
| pfj +F @ 0x14                                Print the content at given offset with flag
| pf n2                                        print signed short (2 bytes) value. Используйте N вместо n для печати беззнаковых значений
| pf [2]? (plop)structname @ 0                 Prints an array of structs
| pf eqew bigWord beef                         Swap endianness and print with given labels
| pf.foo rr (eax)reg1 (eip)reg2                Create object referencing to register values
| pf tt troll plop                             print time stamps with labels troll and plop
```
Ниже приведены примеры:
```
[0x4A13B8C0]> pf i
0x00404888 = 837634441
```
```
[0x4A13B8C0]> pf
0x00404888 = 837634432.000000
```

### Высокоуровневые представления в виде языков программирования

Допустимые форматы печати для языков:

* `pc`     C
* `pc*`    print 'wx' r2 commands
* `pch`    C half-words (2 byte)
* `pcw`    C words (4 byte)
* `pcd`    C dwords (8 byte)
* `pci`    C array of bytes with instructions
* `pca`    GAS .byte blob
* `pcA`    .bytes with instructions in comments
* `pcs`    string
* `pcS`    shellscript that reconstructs the bin
* `pcj`    json
* `pcJ`    javascript
* `pco`    Objective-C
* `pcp`    python
* `pck`    kotlin
* `pcr`    rust
* `pcv`    JaVa
* `pcV`    V (vlang.io)
* `pcy`    yara
* `pcz`    Swift

Если нужно создать файл .c, содержащий BLOB, используйте команду `pc`. Размер по умолчанию такой же, как и в многих других командах: размер блока можно изменить с помощью команды `b` .

Также можно просто временно переопределить этот размер блока, выразив его в качестве аргумента.

```
[0xB7F8E810]> pc 32
#define _BUFFER_SIZE 32
unsigned char buffer[_BUFFER_SIZE] = {
0x89, 0xe0, 0xe8, 0x49, 0x02, 0x00, 0x00, 0x89, 0xc7, 0xe8, 0xe2, 0xff, 0xff, 0xff, 0x81, 0xc3, 0xd6, 0xa7, 0x01, 0x00, 0x8b, 0x83, 0x00, 0xff, 0xff, 0xff, 0x5a, 0x8d, 0x24, 0x84, 0x29, 0xc2 };
```

Структура cstring может использоваться во многих языках программирования, а не только в C.

```
[0x7fcd6a891630]> pcs
"\x48\x89\xe7\xe8\x68\x39\x00\x00\x49\x89\xc4\x8b\x05\xef\x16\x22\x00\x5a\x48\x8d\x24\xc4\x29\xc2\x52\x48\x89\xd6\x49\x89\xe5\x48\x83\xe4\xf0\x48\x8b\x3d\x06\x1a
```

### Строки

Строки, вероятно, являются одной из наиболее важных точек входа при начале реверс-инжениринга программы, поскольку они обычно ссылаются на информацию о действиях функций (утверждения, отладка или информационные сообщения ...). Поэтому radare поддерживает различные строковые форматы:

```
[0x00000000]> ps?
|Usage: ps[bijqpsuwWxz+] [N]  Print String
| ps       print string
| ps+[j]   print libc++ std::string (same-endian, ascii, zero-terminated)
| psb      print strings in current block
| psi      print string inside curseek
| psj      print string in JSON format
| psp[j]   print pascal string
| psq      alias for pqs
| pss      print string in screen (wrap width)
| psu[zj]  print utf16 unicode (json)
| psw[j]   print 16bit wide string
| psW[j]   print 32bit wide string
| psx      show string with escaped chars
| psz[j]   print zero-terminated string
```

Большинство строк заканчиваются нулем. Ниже приведен пример использования отладчика для продолжения выполнения программы до тех пор, пока не выполнится системный вызов 'open'. Как только контроль за процесом восстановлен, получаем аргументы, переданные в системный вызов, на которые указывает %ebx. В случае вызова 'open' это строка с нулевым окончанием, проверим с помощью `psz`.

```
[0x4A13B8C0]> dcs open
0x4a14fc24 syscall(5) open ( 0x4a151c91 0x00000000 0x00000000 ) = 0xffffffda
[0x4A13B8C0]> dr
  eax  0xffffffda    esi  0xffffffff    eip    0x4a14fc24
  ebx  0x4a151c91    edi  0x4a151be1    oeax   0x00000005
  ecx  0x00000000    esp  0xbfbedb1c    eflags 0x200246
  edx  0x00000000    ebp  0xbfbedbb0    cPaZstIdor0 (PZI)
[0x4A13B8C0]>
[0x4A13B8C0]> psz @ 0x4a151c91
/etc/ld.so.cache
```

### Печать содержимого памяти

Печать различных упакованных типов данных делется с помощью команды `pf`:

```
[0xB7F08810]> pf xxS @ rsp
0x7fff0d29da30 = 0x00000001
0x7fff0d29da34 = 0x00000000
0x7fff0d29da38 = 0x7fff0d29da38 -> 0x0d29f7ee /bin/ls
```

Это можно использовать для просмотра аргументов, передаваемых функции. Передайте 'строку, описывающую формат в памяти' в качестве аргумента в `pf` и временно измените текущую позицию поиска/смещение с помощью `@`. Также можно определить массивы структур с помощью `pf`. Для этого добавьте к строке форматирования префикс с числовым значением. Можно также определять имена для каждого поля структуры, добавив их в виде списка аргументов, разделенных пробелами.

```
[0x4A13B8C0]> pf 2*xw pointer type @ esp
0x00404888 [0] {
   pointer :
(*0xffffffff8949ed31)      type : 0x00404888 = 0x8949ed31
   0x00404890 = 0x48e2
}
0x00404892 [1] {
(*0x50f0e483)    pointer : 0x00404892 = 0x50f0e483
	 type : 0x0040489a = 0x2440
}
```

Практический пример использования `pf` в двоичном файле плагина GStreamer:

```
$ radare2 /usr/lib/gstreamer-1.0/libgstflv.so
[0x00006020]> aa; pdf @ sym.gst_plugin_flv_get_desc
[x] Analyze all flags starting with sym. and entry0 (aa)
sym.gst_plugin_flv_get_desc ();
[...]
      0x00013830      488d0549db0000  lea rax, section..data.rel.ro ; 0x21380
      0x00013837      c3              ret
[0x00006020]> s section..data.rel.ro
[0x00021380]> pf ii*z*zp*z*z*z*z*z*z major minor name desc init version license source package origin release_datetime
            major : 0x00021380 = 1
            minor : 0x00021384 = 18
             name : (*0x19cf2)0x00021388 = "flv"
             desc : (*0x1b358)0x00021390 = "FLV muxing and demuxing plugin"
             init : 0x00021398 = (qword)0x0000000000013460
          version : (*0x19cae)0x000213a0 = "1.18.2"
          license : (*0x19ce1)0x000213a8 = "LGPL"
           source : (*0x19cd0)0x000213b0 = "gst-plugins-good"
          package : (*0x1b378)0x000213b8 = "GStreamer Good Plugins (Arch Linux)"
           origin : (*0x19cb5)0x000213c0 = "https://www.archlinux.org/"
 release_datetime : (*0x19cf6)0x000213c8 = "2020-12-06"
```

### Дизассемблирование

Команда `pd` используется для дизассемблирования кода. Она принимает числовое значение, указывающее, сколько инструкций должно быть разобрано. Команда `pD` аналогична, но вместо нескольких инструкций она декомпилирует заданное количество байтов.

* `d` : дизассемблирование N оп-кодов, количество оп-кодов
* `D` : asm.arch disassembler   bsize bytes

```
[0x00404888]> pd 1
;-- entry0:
0x00404888    31ed         xor ebp, ebp
```

### Выбор целевой архитектуры

Целевая архитектура дизассемблера определяется переменной `asm.arch`. Можно использовать `e asm.arch=??` для перечисления всех доступных архитектур.

```
[0x00005310]> e asm.arch=??
_dAe  _8_16      6502        LGPL3   6502/NES/C64/Tamagotchi/T-1000 CPU
_dAe  _8         8051        PD      8051 Intel CPU
_dA_  _16_32     arc         GPL3    Argonaut RISC Core
a___  _16_32_64  arm.as      LGPL3   as ARM Assembler (use ARM_AS environment)
adAe  _16_32_64  arm         BSD     Capstone ARM disassembler
_dA_  _16_32_64  arm.gnu     GPL3    Acorn RISC Machine CPU
_d__  _16_32     arm.winedbg LGPL2   WineDBG's ARM disassembler
adAe  _8_16      avr         GPL     AVR Atmel
adAe  _16_32_64  bf          LGPL3   Brainfuck
_dA_  _32        chip8       LGPL3   Chip8 disassembler
_dA_  _16        cr16        LGPL3   cr16 disassembly plugin
_dA_  _32        cris        GPL3    Axis Communications 32-bit embedded processor
adA_  _32_64     dalvik      LGPL3   AndroidVM Dalvik
ad__  _16        dcpu16      PD      Mojang's DCPU-16
_dA_  _32_64     ebc         LGPL3   EFI Bytecode
adAe  _16        gb          LGPL3   GameBoy(TM) (z80-like)
_dAe  _16        h8300       LGPL3   H8/300 disassembly plugin
_dAe  _32        hexagon     LGPL3   Qualcomm Hexagon (QDSP6) V6
_d__  _32        hppa        GPL3    HP PA-RISC
_dAe  _0         i4004       LGPL3   Intel 4004 microprocessor
_dA_  _8         i8080       BSD     Intel 8080 CPU
adA_  _32        java        Apache  Java bytecode
_d__  _32        lanai       GPL3    LANAI
...
```

### Настройка дизассемблера

Существует несколько вариантов настроек вывода дизассемблера. Все эти опции описаны в `e?` asm.`

```
[0x00005310]> e? asm.
asm.anal: Analyze code and refs while disassembling (see anal.strings)
asm.arch: Set the arch to be used by asm
asm.assembler: Set the plugin name to use when assembling
asm.bbline: Show empty line after every basic block
asm.bits: Word size in bits at assembler
asm.bytes: Display the bytes of each instruction
asm.bytespace: Separate hexadecimal bytes with a whitespace
asm.calls: Show callee function related info as comments in disasm
asm.capitalize: Use camelcase at disassembly
asm.cmt.col: Column to align comments
asm.cmt.flgrefs: Show comment flags associated to branch reference
asm.cmt.fold: Fold comments, toggle with Vz
...
```

В настоящее время существует 136 переменных конфигурации `asm.`, не будем перечислять их все.

### Синтаксис дизассемблера

Переменная `asm.syntax` используется для изменения стиля синтаксиса сборки, используемого механизмом дизассемблера. Чтобы переключиться между представлениями Intel и AT&T:

```
e asm.syntax = intel
e asm.syntax = att
```

Вы также можете попробовать `asm.pseudo`, являющихся экспериментальным представлением псевдокода, и `asm.esil`, выводящий [ESIL](../disassembling/esil.md) ('Evaluable Strings Intermediate Language'). Цель ESIL состоит в том, чтобы иметь удобное представление семантики оп-кодов. Такие варианты представления данных полезны для интерпретированы отдельных инструкций.
