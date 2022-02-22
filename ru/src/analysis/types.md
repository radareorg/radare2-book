# Типы данных

Radare2 поддерживает описание типов данных при помощи синтаксиса языка C.
Описания анализируются синтаксическим анализатором, совместимым с C11, и сохраняются в внутренний SDB, и доступны для интроспекции при помощи команды `k`.

Большинство связанных команд находятся в пространстве имен `t`:

```
[0x00000000]> t?
| Usage: t   # cparse types commands
| t                          List all loaded types
| tj                         List all loaded types as json
| t <type>                   Show type in 'pf' syntax
| t*                         List types info in r2 commands
| t- <name>                  Delete types by its name
| t-*                        Remove all types
| tail [filename]            Output the last part of files
| tc [type.name]             List all/given types in C output format
| te[?]                      List all loaded enums
| td[?] <string>             Load types from string
| tf                         List all loaded functions signatures
| tk <sdb-query>             Perform sdb query
| tl[?]                      Show/Link type to an address
| tn[?] [-][addr]            manage noreturn function attributes and marks
| to -                       Open cfg.editor to load types
| to <path>                  Load types from C header file
| toe [type.name]            Open cfg.editor to edit types
| tos <path>                 Load types from parsed Sdb database
| tp  <type> [addr|varname]  cast data at <address> to <type> and print it (XXX: type can contain spaces)
| tpv <type> @ [value]       Show offset formatted for given type
| tpx <type> <hexpairs>      Show value for type with specified byte sequence (XXX: type can contain spaces)
| ts[?]                      Print loaded struct types
| tu[?]                      Print loaded union types
| tx[f?]                     Type xrefs
| tt[?]                      List all loaded typedefs
```

Обратите внимание, что базовые (атомарные) типы не соответствуют стандарту C - нет `char`, `_Bool` и `short`. Эти типы могут быть разными на разных платформах, radare2 использует `строго определенные` типы, такие как `int8_t` или `uint64_t`, и конвертирует `int` в `int32_t` или в `int64_t` в зависимости от бинарной или отлаживаемой платформы/компилятора.

Базовые типы перечисляются с помощью команды `t`. Для типов структур нужно использовать `ts`, для юнионов - `tu`, а для перечислений — `te`.

```
[0x00000000]> t
char
char *
double
float
gid_t
int
int16_t
int32_t
int64_t
int8_t
long
long long
pid_t
short
size_t
uid_t
uint16_t
uint32_t
uint64_t
uint8_t
unsigned char
unsigned int
unsigned short
void *
```

### Загрузка типов

Есть три способа определить новый тип:
* Непосредственно из командной строки с помощью команы `td`.
* Из файла с помощью `to <имя файла>`.
* Открыть `$EDITOR` для ввода определений с помощью `to -`

```
[0x00000000]> "td struct foo {char* a; int b;}"
[0x00000000]> cat ~/radare2-regressions/bins/headers/s3.h
struct S1 {
    int x[3];
    int y[4];
    int z;
};
[0x00000000]> to ~/radare2-regressions/bins/headers/s3.h
[0x00000000]> ts
foo
S1
```

radare2 позволяет органризовать директорий для хранения описанй типов и включений.

```
[0x00000000]> e? dir.types
dir.types: Default path to look for cparse type files
[0x00000000]> e dir.types
/usr/include
```

### Печати структур

Обратите внимание, что ниже мы использовали команду `ts`, преобразующая описание типа C (или, если быть точным, представление SDB)
 в последовательность `pf`-команд. Смотрите подробнее о [печати структур](../basic_commands/print_modes.md).

Команда `tp` использует строку `pf` для печати всех полей типа по текущему смещению/данному адресу:

```
[0x00000000]> "td struct foo {char* a; int b;}"
[0x00000000]> wx 68656c6c6f000c000000
[0x00000000]> wz world @ 0x00000010 ; wx 17 @ 0x00000016
[0x00000000]> px
[0x00000000]> ts foo
pf zd a b
[0x00000000]> tp foo
 a : 0x00000000 = "hello"
 b : 0x00000006 = 12
[0x00000000]> tp foo @ 0x00000010
 a : 0x00000010 = "world"
 b : 0x00000016 = 23
```

Можно также заполнить свои данные в структуре и распечатать их, используя команду `tpx`.

```
[0x00000000]> tpx foo 414243440010000000
 a : 0x00000000 = "ABCD"
 b : 0x00000005 = 16
```

### Типы ссылок

Команда `tp` просто выполняет приведение типов. Но если надо связать какой-то адрес или переменную с выбранным типом можно использовать команду `tl` для сохранения этого отношения в SDB.

```
[0x000051c0]> tl S1 = 0x51cf
[0x000051c0]> tll
(S1)
 x : 0x000051cf = [ 2315619660, 1207959810, 34803085 ]
 y : 0x000051db = [ 2370306049, 4293315645, 3860201471, 4093649307 ]
 z : 0x000051eb = 4464399
```

Причем ссылка будет показана в дизассемблированном выводе или визуальном режиме:

```
[0x000051c0 15% 300 /bin/ls]> pd $r @ entry0
 ;-- entry0:
 0x000051c0      xor ebp, ebp
 0x000051c2      mov r9, rdx
 0x000051c5      pop rsi
 0x000051c6      mov rdx, rsp
 0x000051c9      and rsp, 0xfffffffffffffff0
 0x000051cd      push rax
 0x000051ce      push rsp
(S1)
 x : 0x000051cf = [ 2315619660, 1207959810, 34803085 ]
 y : 0x000051db = [ 2370306049, 4293315645, 3860201471, 4093649307 ]
 z : 0x000051eb = 4464399
 0x000051f0      lea rdi, loc._edata         ; 0x21f248
 0x000051f7      push rbp
 0x000051f8      lea rax, loc._edata         ; 0x21f248
 0x000051ff      cmp rax, rdi
 0x00005202      mov rbp, rsp
```

После того, как структура связана, radare2 пытается распространить смещение структуры в функции по текущему смещению, чтобы запустить этот анализ во всей программе или в любых целевых функциях после того, как все структуры будут связаны - команда `aat`:

```
[0x00000000]> aa?
| aat [fcn]           Analyze all/given function to convert immediate to linked structure offsets (see tl?)
```

Иногда эмуляция может быть неточной, например, как показано ниже:

````
|0x000006da  push rbp
|0x000006db  mov rbp, rsp
|0x000006de  sub rsp, 0x10
|0x000006e2  mov edi, 0x20               ; "@"
|0x000006e7  call sym.imp.malloc         ;  void *malloc(size_t size)
|0x000006ec  mov qword [local_8h], rax
|0x000006f0  mov rax, qword [local_8h]

````

Возвращаемое значение `malloc` может отличаться между двумя эмуляциями, нужно установить подсказку для возвращаемого значения вручную, используя команду `ahr`, потом запускать `tl` или `aat` после настройки подсказки.

```
[0x000006da]> ah?
| ahr val            установить подсказку для возвращаемого значения функции
```

### Structure Immediates

Есть еще один важный аспект использования типов в Radare2 — использование `aht`, можно изменять immediate-значение в коде операции на смещение структуры.
Рассмотрим простой пример [RSI]-относительной адресации

```
[0x000052f0]> pd 1
0x000052f0      mov rax, qword [rsi + 8]    ; [0x8:8]=0
```
Здесь `8` - некоторое смещение в памяти, где `rsi` вероятно, держит некоторый указатель структуры. Представьте, что у нас есть следующие структуры
```

[0x000052f0]> "td struct ms { char b[8]; int member1; int member2; };"
[0x000052f0]> "td struct ms1 { uint64_t a; int member1; };"
[0x000052f0]> "td struct ms2 { uint16_t a; int64_t b; int member1; };"
```
Теперь нам нужно установить правильное смещение члена структуры вместо `8` в этой инструкции.
Сначала нужно перечислить доступные типы, соответствующие этому смещению:

```
[0x000052f0]> ahts 8
ms.member1
ms1.member1
```

Обратите внимание, что `ms2` не указан, так как в нем нет элементов со смещением `8`.
После перечисления доступных вароантов, можем связать их с выбранным смещением по адресу текущий адрес:

```
[0x000052f0]> aht ms1.member1
[0x000052f0]> pd 1
0x000052f0      488b4608       mov rax, qword [rsi + ms1.member1]    ; [0x8:8]=0
```

### Управление перечислениями

* Печать всех полей в перечислении с использованием `te` команда

```
[0x00000000]> "td enum Foo {COW=1,BAR=2};"
[0x00000000]> te Foo
COW = 0x1
BAR = 0x2
```

* Поиск соответствующего члена перечисления для заданного битового поля и наоборот

```
[0x00000000]> te Foo 0x1
COW
[0x00000000]> teb Foo COW
0x1
```

## Внутреннее представительство

Чтобы увидеть внутреннее представление типов используйте команду `tk`:

```
[0x000051c0]> tk~S1
S1=struct
struct.S1=x,y,z
struct.S1.x=int32_t,0,3
struct.S1.x.meta=4
struct.S1.y=int32_t,12,4
struct.S1.y.meta=4
struct.S1.z=int32_t,28,0
struct.S1.z.meta=0
[0x000051c0]>
```

Определение примитивных типов требует понимания основных форматов `pf`, список спецификаторов формата - `pf??`:

```
-----------------------------------------------------
| format | explanation                              |
|---------------------------------------------------|
|  b     |  byte (unsigned)                         |
|  c     |  char (signed byte)                      |
|  d     |  0x%%08x hexadecimal value (4 bytes)     |
|  f     |  float value (4 bytes)                   |
|  i     |  %%i integer value (4 bytes)             |
|  o     |  0x%%08o octal value (4 byte)            |
|  p     |  pointer reference (2, 4 or 8 bytes)     |
|  q     |  quadword (8 bytes)                      |
|  s     |  32bit pointer to string (4 bytes)       |
|  S     |  64bit pointer to string (8 bytes)       |
|  t     |  UNIX timestamp (4 bytes)                |
|  T     |  show Ten first bytes of buffer          |
|  u     |  uleb128 (variable length)               |
|  w     |  word (2 bytes unsigned short in hex)    |
|  x     |  0x%%08x hex value and flag (fd @ addr)  |
|  X     |  show formatted hexpairs                 |
|  z     |  \0 terminated string                    |
|  Z     |  \0 terminated wide string               |
-----------------------------------------------------

```
there are basically 3 mandatory keys for defining basic data types:
`X=type`
`type.X=format_specifier`
`type.X.size=size_in_bits`
For example, let's define `UNIT`, according to [Microsoft documentation](https://msdn.microsoft.com/en-us/library/windows/desktop/aa383751%28v=vs.85%29.aspx#UINT)
`UINT` is just equivalent of standard C `unsigned int` (or `uint32_t` in terms of TCC engine).
Он будет определяться как:

```
UINT=type
type.UINT=d
type.UINT.size=32
```

Теперь есть необязательная запись:

`X.type.pointto=Y`

Это можно использовать только в случае указателя `type.Х=р`, хорошим примером является определение LPFILETIME - это указатель на `_FILETIME`, которая оказывается структурой.
Предполагая, что мы нацелены только на 32-разрядную машину Windows, она будет определена следующим образом:

```
LPFILETIME=type
type.LPFILETIME=p
type.LPFILETIME.size=32
type.LPFILETIME.pointto=_FILETIME
```

Последнее поле не является обязательным, поскольку иногда в структурах данных внутренности бывают проприетарными, и у нас нет для них четкого представления.

Также есть еще одна необязательная запись:

```
type.UINT.meta=4
```

Эта запись предназначена для интеграции с синтаксическим анализатором C и содержит информацию о классе типов: целочисленный размер, знаковый/беззнаковый и т.д.

### Структуры

Это основные ключи для структур (всего два элемента):

```
X=struct
struct.X=a,b
struct.X.a=a_type,a_offset,a_number_of_elements
struct.X.b=b_type,b_offset,b_number_of_elements
```

Первая строка используется для определения структуры, называемой `X`, вторая строка определяет элементы `X` как значения, разделенные запятыми. После этого просто определяем информацию о каждом элементе.

Для примера. у нас может быть такая структура:

```
struct _FILETIME {
	DWORD dwLowDateTime;
	DWORD dwHighDateTime;
}
```

полагая, что `DWORD` определено, структура будет выглядеть так

```
 _FILETIME=struct
struct._FILETIME=dwLowDateTime,dwHighDateTime
struct._FILETIME.dwLowDateTime=DWORD,0,0
struct._FILETIME.dwHighDateTime=DWORD,4,0
```

Обратите внимание, что поле количества элементов используется только в случае массивов для определения количества элементов в массивах, по умолчанию оно равно нулю.

### Юнионы

Юнионы определяются точно так же, как структуры, с той лишь разницей, что вы заменяете слово `struct` на `union`.

### Прототипы функций

Представление прототипов функций является наиболее подробным и наиболее важным среди всех определений. На самом деле, это тот, который используется непосредственно для сопоставления типов.

```
X=func
func.X.args=NumberOfArgs
func.x.arg0=Arg_type,arg_name
.
.
.
func.X.ret=Return_type
func.X.cc=calling_convention
```

Это и так понятно. Определим strncasecmp в качестве примера для архитектуры x86 для машин с Linux. Согласно справочным манам, strncasecmp определяется следующим образом:

```
int strcasecmp(const char *s1, const char *s2, size_t n);
```

При преобразовании его в представление sdb он будет выглядеть следующим образом:

```
strcasecmp=func
func.strcasecmp.args=3
func.strcasecmp.arg0=char *,s1
func.strcasecmp.arg1=char *,s2
func.strcasecmp.arg2=size_t,n
func.strcasecmp.ret=int
func.strcasecmp.cc=cdecl
```

Обратите внимание, что `.cc` часть является необязательной, и если она не используется, то использоваться соглашение о вызовах по умолчанию для вашей целевой архитектуры.
Есть еще один дополнительный необязательный ключ

```
func.x.noreturn=true/false
```

Этот ключ используется для обозначения функций, которые не возвращаются после вызова, например `exit` и `_exit`.
