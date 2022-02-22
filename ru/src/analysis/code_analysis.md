# Анализ кода

Анализ кода является распространенным методом, используемым для извлечения информации из ассемблерного кода.

Radare2 включает различные методы анализа кода, реализованные в ядре и доступные в разных командах.

Все функциональные возможности r2 доступны как с API, так и с помощью команд. Это дает вам возможность реализовать свои собственные циклы анализа, используя любой язык программирования, даже при помощи командной строки r2, скриптов в ОС, реализовать собственные алгоритмы при помощи нативных плагинов.

Анализ покажет внутренние структуры данных для идентификации основных блоков, вызовов функций и извлечения информации на уровне оп-кода.

Наиболее распространенной последовательностью команд анализа radare2 является `aa`, что расшифровывается как «analyze all». Это относится ко всем символам и точкам входа. Если в двоичном файле удалены (stripped) символы, нужно использовать другие команды, такие как `aaa`, `aab`, `aar`, `aac` и им подобные.

Изучите, что делает каждая команда и результаты после их выполнения, чтобы подобрать лучший набор для ваших нужд.

```
[0x08048440]> aa
[0x08048440]> pdf @ main
		   ; DATA XREF from 0x08048457 (entry0)
/ (fcn) fcn.08048648 141
|     ;-- main:
|     0x08048648    8d4c2404     lea ecx, [esp+0x4]
|     0x0804864c    83e4f0       and esp, 0xfffffff0
|     0x0804864f    ff71fc       push dword [ecx-0x4]
|     0x08048652    55           push ebp
|     ; CODE (CALL) XREF from 0x08048734 (fcn.080486e5)
|     0x08048653    89e5         mov ebp, esp
|     0x08048655    83ec28       sub esp, 0x28
|     0x08048658    894df4       mov [ebp-0xc], ecx
|     0x0804865b    895df8       mov [ebp-0x8], ebx
|     0x0804865e    8975fc       mov [ebp-0x4], esi
|     0x08048661    8b19         mov ebx, [ecx]
|     0x08048663    8b7104       mov esi, [ecx+0x4]
|     0x08048666    c744240c000. mov dword [esp+0xc], 0x0
|     0x0804866e    c7442408010. mov dword [esp+0x8], 0x1 ;  0x00000001
|     0x08048676    c7442404000. mov dword [esp+0x4], 0x0
|     0x0804867e    c7042400000. mov dword [esp], 0x0
|     0x08048685    e852fdffff   call sym..imp.ptrace
|        sym..imp.ptrace(unk, unk)
|     0x0804868a    85c0         test eax, eax
| ,=< 0x0804868c    7911         jns 0x804869f
| |   0x0804868e    c70424cf870. mov dword [esp], str.Don_tuseadebuguer_ ;  0x080487cf
| |   0x08048695    e882fdffff   call sym..imp.puts
| |      sym..imp.puts()
| |   0x0804869a    e80dfdffff   call sym..imp.abort
| |      sym..imp.abort()
| `-> 0x0804869f    83fb02       cmp ebx, 0x2
|,==< 0x080486a2    7411         je 0x80486b5
||    0x080486a4    c704240c880. mov dword [esp], str.Youmustgiveapasswordforusethisprogram_ ;  0x0804880c
||    0x080486ab    e86cfdffff   call sym..imp.puts
||       sym..imp.puts()
||    0x080486b0    e8f7fcffff   call sym..imp.abort
||       sym..imp.abort()
|`--> 0x080486b5    8b4604       mov eax, [esi+0x4]
|     0x080486b8    890424       mov [esp], eax
|     0x080486bb    e8e5feffff   call fcn.080485a5
|        fcn.080485a5() ; fcn.080484c6+223
|     0x080486c0    b800000000   mov eax, 0x0
|     0x080486c5    8b4df4       mov ecx, [ebp-0xc]
|     0x080486c8    8b5df8       mov ebx, [ebp-0x8]
|     0x080486cb    8b75fc       mov esi, [ebp-0x4]
|     0x080486ce    89ec         mov esp, ebp
|     0x080486d0    5d           pop ebp
|     0x080486d1    8d61fc       lea esp, [ecx-0x4]
\     0x080486d4    c3           ret
```

В этом примере анализируется весь файл (`aa`), а затем печатается дизассемблирование функции `main()` (`pdf`).
Команда `aa` относится к семейству команд автоанализа и выполняет только самые основные этапы автоматического анализа. В radare2 существует множество различных типов команд автоанализа с
различной глубиной анализа, включая частичную эмуляцию: `aa`, `aaa`, `aab`, `aaaa`, ...
Существует также сопоставление этих команд с параметрами интерфейса командной строки r2: `r2 -A`, `r2 -AA` и так далее.

Здравый смысл заключается в том, что полностью автоматизированный анализ может дать противоречивые результаты. radare2 предоставляет отдельные команды для конкретных этапов анализа, что позволяет контролировать процесс анализа. Существует сокровищница конфигурационных переменных для управления результатами анализа. Их можной найти в конфигурационных переменых пространства имен `anal.*` и `emu.*` .

## Анализ функций

Одной из наиболее важных «базовых» команд анализа является набор команд `af`. `af` значит "анализ функций" (analyze function). С помощью этой команды вы можете запустить автоматический анализ конкретной
функции, но и вручную тоже можно.

```
[0x00000000]> af?
Usage: af
| af ([name]) ([addr])                  analyze functions (start at addr or $$)
| afr ([name]) ([addr])                 analyze functions recursively
| af+ addr name [type] [diff]           hand craft a function (requires afb+)
| af- [addr]                            clean all function analysis data (or function at addr)
| afa                                   analyze function arguments in a call (afal honors dbg.funcarg)
| afb+ fcnA bbA sz [j] [f] ([t]( [d]))  add bb to function @ fcnaddr
| afb[?] [addr]                         List basic blocks of given function
| afbF([0|1])                           Toggle the basic-block 'folded' attribute
| afB 16                                set current function as thumb (change asm.bits)
| afC[lc] ([addr])@[addr]               calculate the Cycles (afC) or Cyclomatic Complexity (afCc)
| afc[?] type @[addr]                   set calling convention for function
| afd[addr]                             show function + delta for given offset
| afF[1|0|]                             fold/unfold/toggle
| afi [addr|fcn.name]                   show function(s) information (verbose afl)
| afj [tableaddr] [count]               analyze function jumptable
| afl[?] [ls*] [fcn name]               list functions (addr, size, bbs, name) (see afll)
| afm name                              merge two functions
| afM name                              print functions map
| afn[?] name [addr]                    rename name for function at address (change flag too)
| afna                                  suggest automatic name for current offset
| afo[?j] [fcn.name]                    show address for the function name or current offset
| afs[!] ([fcnsign])                    get/set function signature at current address (afs! uses cfg.editor)
| afS[stack_size]                       set stack frame size for function at current address
| afsr [function_name] [new_type]       change type for given function
| aft[?]                                type matching, type propagation
| afu addr                              resize and analyze function from current address until addr
| afv[absrx]?                           manipulate args, registers and variables in function
| afx                                   list function references
```
Используйте `afl` для перечисления функций, найденных в анализе.

Есть много полезных команд под `afl`, таких как `aflj`, который перечисляет функцию в формате JSON и `aflm`, который перечисляет функции в синтаксисе, аналогичном makefiles.

Есть также `afl=`, отображающий ASCII-арт-схему памяти с занимаемым местом функциями.

Остальные команды можно найти под `afl?`.

Некоторые из наиболее сложных задач при выполнении анализа функций — это слияние, обрезка и изменение размера.
Как и в случае с другими командами анализа, есть два режима: полуавтоматический и ручной.
Для полуавтоматического можно использовать `afm <function name>` для объединения текущей функции с другой, указанной по имени в аргументе,  `aff` перенастроить функцию после проведенных изменений или редактирования функции, `afu <address>` - изменение размера и анализ текущей функции до указанного адреса.

Помимо этих полуавтоматических способов редактирования / анализа функции,  можно создавать ее в ручном режиме с помощью команды `af+` и редактировать основные блоки с помощью команд `afb`.
Перед изменением основных блоков функции рекомендуется проверить уже имеющиеся:

```
[0x00003ac0]> afb
0x00003ac0 0x00003b7f 01:001A 191 f 0x00003b7f
0x00003b7f 0x00003b84 00:0000 5 j 0x00003b92 f 0x00003b84
0x00003b84 0x00003b8d 00:0000 9 f 0x00003b8d
0x00003b8d 0x00003b92 00:0000 5
0x00003b92 0x00003ba8 01:0030 22 j 0x00003ba8
0x00003ba8 0x00003bf9 00:0000 81
```

### Анализ функции вручную
Давайте сначала подготовим двоичный файл, например:
```C
int code_block()
{
  int result = 0;

  for(int i = 0; i < 10; ++i)
    result += 1;

  return result;
}
```
затем скомпилируйте его `gcc -c example.c -m32 -O0 -fno-pie`, получим объектный файл `example.o`. откройте его с помощью radare2.

поскольку он не проанализирован, команда `pdf` не будет показывать дизассемблирование:
```
$ r2 example.o
[0x08000034]> pdf
p: Cannot find function at 0x08000034
[0x08000034]> pd
            ;-- section..text:
            ;-- .text:
            ;-- code_block:
            ;-- eip:
            0x08000034      55             push ebp                    ; [01] -r-x section size 41 named .text
            0x08000035      89e5           mov ebp, esp
            0x08000037      83ec10         sub esp, 0x10
            0x0800003a      c745f8000000.  mov dword [ebp - 8], 0
            0x08000041      c745fc000000.  mov dword [ebp - 4], 0
        ,=< 0x08000048      eb08           jmp 0x8000052
       .--> 0x0800004a      8345f801       add dword [ebp - 8], 1
       :|   0x0800004e      8345fc01       add dword [ebp - 4], 1
       :`-> 0x08000052      837dfc09       cmp dword [ebp - 4], 9
       `==< 0x08000056      7ef2           jle 0x800004a
            0x08000058      8b45f8         mov eax, dword [ebp - 8]
            0x0800005b      c9             leave
            0x0800005c      c3             ret

```
Наша цель состоит в том, чтобы вручную создать функцию со следующей структурой

![analyze_one](analyze_one.png)


создайте функцию в 0x8000034 с именем code_block:
```
[0x8000034]> af+ 0x8000034 code_block
```

В большинстве случаев используем инструкции jump или call в качестве границ блоков кода. таким образом, диапазон первого блока составляет от `0x08000034 push ebp` до `0x08000048 jmp 0x8000052`.
используйте команду `afb+`, чтобы добавить его.

```
[0x08000034]> afb+ code_block 0x8000034 0x800004a-0x8000034 0x8000052
```

Обратите внимание, что базовым синтаксисом `afb+` является `afb+ function_address block_address block_size [jump] [fail]`. последняя инструкция этого блока указывает на новый адрес (jmp 0x8000052), поэтому добавляем адрес назначения перехода (0x8000052), чтобы отразить информацию о переходе.

следующий блок (0x08000052 ~ 0x08000056) является скорее условным оператором if, который включает две ветви. Он перейдет к 0x800004a если `jle - меньше или равно`, в противном случае (условие fail) перейдет к следующей инструкции - 0x08000058.:

```
[0x08000034]> afb+ code_block 0x8000052 0x8000058-0x8000052 0x800004a 0x8000058
```

следуйте потоку управления и создайте оставшиеся два блока (две ветви):
```
[0x08000034]> afb+ code_block 0x800004a 0x8000052-0x800004a 0x8000052
[0x08000034]> afb+ code_block 0x8000058 0x800005d-0x8000058
```

проверьте результат:
```
[0x08000034]> afb
0x08000034 0x0800004a 00:0000 22 j 0x08000052
0x0800004a 0x08000052 00:0000 8 j 0x08000052
0x08000052 0x08000058 00:0000 6 j 0x0800004a f 0x08000058
0x08000058 0x0800005d 00:0000 5
[0x08000034]> VV
```

![handcraft_one](handcraft_one.png)

Для этого есть две очень важные команды: `afc` и `afB`. Последнее является обязательной командой для некоторых платформ, таких как ARM. Он предоставляет способ изменения "bitness" конкретной функции. , позволяя выбирать между режимами ARM и Thumb.

Команда `afc` позволяет вручную указать соглашение о вызове функции. Более подробную информацию о его использовании в [calling_conventions](calling_conventions.md).

## Рекурсивный анализ

Существует пять важных команд полуавтоматизированного анализа по всей программе:

- `aab` - выполнение базово-блочного анализа (алгоритм "Nucleus")
- `aac` - анализ вызовов функций из одной (выбранной или текущей функции)
- `aaf` - анализ всех вызовов функций
- `aar` - анализ ссылок на данные
- `aad` - анализ указателей на ссылки указателей

Это только общие полуавтоматические алгоритмы поиска ссылок. Radare2 обеспечивает широкий выбор инструментов ручного задания ссылок. Детальное управление разметкой - использование команды `ax` .

```
Usage: ax[?d-l*]   # see also 'afx?'
| ax              list refs
| ax*             output radare commands
| ax addr [at]    add code ref pointing to addr (from curseek)
| ax- [at]        clean all refs/refs from addr
| ax-*            clean all refs/refs
| axc addr [at]   add generic code ref
| axC addr [at]   add code call ref
| axg [addr]      show xrefs graph to reach current function
| axg* [addr]     show xrefs graph to given address, use .axg*;aggv
| axgj [addr]     show xrefs graph to reach current function in json format
| axd addr [at]   add data ref
| axq             list refs in quiet/human-readable format
| axj             list refs in json format
| axF [flg-glob]  find data/code references of flags
| axm addr [at]   copy data/code references pointing to addr to also point to curseek (or at)
| axt [addr]      find data/code references to this address
| axf [addr]      find data/code references from this address
| axv [addr]      list local variables read-write-exec references
| ax. [addr]      find data/code references from and to this address
| axff[j] [addr]  find data/code references from this function
| axs addr [at]   add string ref
```

Наиболее часто используемыми командами группы `ax` являются `axt` и `axf`, особенно в составе различных сценариев r2pipe.
 Допустим, видим строку в разделе данных или кода и хотим найти все места
ссылок на нее, используем `axt`:

```
[0x0001783a]> pd 2
;-- str.02x:
; STRING XREF from 0x00005de0 (sub.strlen_d50)
; CODE XREF from 0x00017838 (str.._s_s_s + 7)
0x0001783a     .string "%%%02x" ; len=7
;-- str.src_ls.c:
; STRING XREF from 0x0000541b (sub.free_b04)
; STRING XREF from 0x0000543a (sub.__assert_fail_41f + 27)
; STRING XREF from 0x00005459 (sub.__assert_fail_41f + 58)
; STRING XREF from 0x00005f9e (sub._setjmp_e30)
; CODE XREF from 0x0001783f (str.02x + 5)
0x00017841 .string "src/ls.c" ; len=9
[0x0001783a]> axt
sub.strlen_d50 0x5de0 [STRING] lea rcx, str.02x
(nofunc) 0x17838 [CODE] jae str.02x
```

Есть также несколько полезных команд под `axt`. Используйте `axtg` для генерации команд radare2, которые помогут вам создавать графики в соответствии с XREF.

```
[0x08048320]> s main
[0x080483e0]> axtg
agn 0x8048337 "entry0 + 23"
agn 0x80483e0 "main"
age 0x8048337 0x80483e0
```

Используйте `axt*` для разделения команд radare2 и установки флагов на соответствующих XREF.

Также под `ax` находится `axg`, который находит путь между двумя точками в файле, показывая график XREFs для достижения местоположения или функции. Например:

```
:> axg sym.imp.printf
- 0x08048a5c fcn 0x08048a5c sym.imp.printf
  - 0x080483e5 fcn 0x080483e0 main
  - 0x080483e0 fcn 0x080483e0 main
    - 0x08048337 fcn 0x08048320 entry0
  - 0x08048425 fcn 0x080483e0 main
```
Используйте `axg*` для генерации команд radare2, которые помогут вам создавать графики с использованием команд `agn` и `age` в соответствии с XREF.

Помимо предопределенных алгоритмов для идентификации функций есть способ указать "прелюдию" функции с параметром конфигурации `anal.prelude`. Например `e anal.prelude = 0x554889e5` что означает

```
push rbp
mov rbp, rsp
```

на платформе x86\_64. Она должна быть указана _перед_ любыми командами анализа.

## Конфигурация

Radare2 позволяет изменять поведение практически любых этапов анализа или команд.
Существуют различные типы параметров конфигурации:

- Управление потоком
- Управление основными блоками
- Элемент управления ссылками
- IO/Диапазоны
- Управление анализом таблиц переходов
- Конкретные параметры платформы/процессора

### Конфигурация потока управления

Двумя наиболее часто используемыми вариантами изменения поведения анализа потока управления в radare2 являются: `anal.hasext` и `anal.jmp.after`. Первый позволяет заставить radare2 продолжить анализ после окончания функции, даже если следующий кусок кода не вызвается откуда-либо, что означает анализ всех доступных функций. Последний позволяет заставить radare2 продолжать анализ даже после безусловных переходов.

В дополнение к ним мы также `anal.jmp.indir` для отслеживания косвенных переходов, продолжая анализ; `anal.pushret` для анализа `push ...; ret` sequence как переход; `anal.nopskip` для пропуска NOP последовательности в начале функции.

На данный момент radare2 также позволяет изменять максимальный базовый размер блока с помощью опции `anal.bb.maxsize`. Значение по умолчанию работает в большинстве случаев использования, но полезно увеличить его при работе с обфускированным кодом. Обратите внимание, что некоторые из настроек управления базовыми блоками  могут быть изменеы в будущем в пользу более автоматизированных способов их установки.

Для некоторых необычных двоичных файлов или архитектур есть опция `anal.noncode`. По умолчанию Radare2 не пытается анализировать разделы данных в виде кода. Но в некоторых случаях – вредоносное ПО, упакованные двоичные файлы, двоичные файлы для встраиваемых систем - требуют проводить такй анализ. Thus - this option.

### Управление ссылками

Наиболее важные настройки, кардинально меняющие результаты анализа. Некоторые настройки можно отключить для экономии времени и памяти при анализе больших двоичных файлов.

- `anal.jmp.ref` - разрешить создание ссылок на безусловные переходы
- `anal.jmp.cref` - то же самое, но для условных переходов
- `anal.datarefs` - для отслеживания ссылок на данные в коде
- `anal.refstr` - поиск строк в ссылках на данные
- `anal.strings` - поиск строк и создание ссылок

Управление ссылками на строки по умолчанию отключен, так как увеличивает время анализа.

### Диапазоны анализа

Настройки:

- `anal.limits` - позволяет использовать пределы диапазона для операций анализа
- `anal.from` - начальный адрес предельного диапазона
- `anal.to` - соответствующий конец предельного диапазона
- `anal.in` - границы поиска для анализа. Вы можете установить [его/их] на `io.maps`, `io.sections.exec`, `dbg.maps` и многие другие. Например:
   - Чтобы проанализировать конкретную карту памяти с помощью `anal.from` и `anal.to`, установите `anal.in = dbg.maps`.
   - Для анализа в границах, задайте `anal.from` и `anal.to`, установите `anal.in=range`.
   - Для анализа в текущем отображаемом сегменте или разделе можно поместить `anal.in=bin.segment` или `anal.in=bin.section` соответственно.
   - Для анализа в текущей карте памяти укажите `anal.in=dbg.map`.
   - Для анализа в стеке или куче можно задать `anal.in=dbg.stack` или `anal.in=dbg.heap`.
   - Для анализа в текущей функции или базовом блоке можно указать `anal.in=anal.fcn` или `anal.in=anal.bb`.

Пожалуйста, смотрите полный список `e anal.in=??`.

### Таблицы переходов

Таблицы переходов являются одной из самых сложных целей в двоичном реверс-инжиниринге. Их есть сотни различных типов, конечный результат зависит от компилятора/компоновщика и этапов оптимизации LTO.
Radare2 позволяет включить некоторые экспериментальные алгоритмы обнаружения таблиц прыжков с использованием `anal.jmp.tbl`. В конце концов, алгоритмы переходят в циклы анализа таблиц переходов по умолчанию на каждой поддерживаемой платформе/тесткейсе.
Еще две настройки также могут повлиять на результаты анализа таблиц переходов:

- `anal.jmp.indir` - отслеживать непрямые переходы, некоторые таблицы переходов опираются на них
- `anal.datarefs` - отслеживать ссылки на данные, некоторые таблицы переходов используют их

### Элементы управления, специфичные для платформы

Существует две распространенные проблемы при анализе встроенных прхитектур: обнаружение ARM/Thumb и значения MIPS GP. В случае двоичных файлов ARM radare2 поддерживает автоматическое обнаружение переключателей режима ARM/Thumb, но имейте ввиду, что он использует частичную эмуляцию ESIL, тем самым замедляя процесс анализа. Если результаты вам не нравиться, определенные режимы для функций могут быть переопределены `afB` .

Проблема MIPS GP еще сложнее. Известно, что значение GP может быть разным не только
для всей программы, но также и для отдельных функций. Частично проблему решают настройки `anal.gp` и `anal.gpfixed`. Первый устанавливает значение GP для всей программы или конкретной
функция. Второй позволяет «фиксировать» значение GP, если какой-то код захочет изменить его, всегда сбрасывая его, если факт смены обнаружен. Они в значительной степени экспериментальны и могут быть изменены в
будущем в пользу более автоматизированного анализа.

## Визуальный подход

Один из самых простых способов увидеть и проверить изменения команд анализа и переменных заключается в выполнении прокрутки в специальном визуальном режиме `Vv`, позволяющем выполнять функции предварительного просмотра:

![vv](code_analysis_vv.png)

Когда надо проверить, как изменения анализа влияют на результат в случае больших функций, вместо этого можно использовать мини-карту, позволяющую видеть общий граф управления на том же размере экрана. Чтобы войти в режим мини-карты введите `VV`, затем дважды нажмите клавишу `p`:

![vv2](code_analysis_vv2.png)

Этот режим позволяет увидеть разборку каждого узла в отдельности, просто перемещаясь между ними с помощью клавиши `Tab` .

## Подсказки по анализу

Нередки случаи, когда результаты анализа не идеальны даже после того, как вы попробовали все до единого, включая параметры конфигурации. Именно здесь появляется механизм «analysis hints» radare2. Это позволяет переопределить некоторые основные свойства оп-кода или метаданных или даже переписать всю строку оп-кода. Эти команды расположены в пространстве имен `ah`:

```
Usage: ah[lba-]  Analysis Hints
| ah?                show this help
| ah? offset         show hint of given offset
| ah                 list hints in human-readable format
| ah.                list hints in human-readable format from current offset
| ah-                remove all hints
| ah- offset [size]  remove hints at given offset
| ah* offset         list hints in radare commands format
| aha ppc @ 0x42     force arch ppc for all addrs >= 0x42 or until the next hint
| aha 0 @ 0x84       disable the effect of arch hints for all addrs >= 0x84 or until the next hint
| ahb 16 @ 0x42      force 16bit for all addrs >= 0x42 or until the next hint
| ahb 0 @ 0x84       disable the effect of bits hints for all addrs >= 0x84 or until the next hint
| ahc 0x804804       override call/jump address
| ahd foo a0,33      replace opcode string
| ahe 3,eax,+=       set vm analysis string
| ahf 0x804840       override fallback address for call
| ahF 0x10           set stackframe size at current offset
| ahh 0x804840       highlight this address offset in disasm
| ahi[?] 10          define numeric base for immediates (2, 8, 10, 10u, 16, i, p, S, s)
| ahj                list hints in JSON
| aho call           change opcode type (see aho?) (deprecated, moved to "ahd")
| ahp addr           set pointer hint
| ahr val            set hint for return value of a function
| ahs 4              set opcode size=4
| ahS jz             set asm.syntax=jz for this opcode
| aht [?] <type>     Mark immediate as a type offset (deprecated, moved to "aho")
| ahv val            change opcode's val field (useful to set jmptbl sizes in jmp rax)
```

Один из наиболее распространенных случаев - установка определенной числовой базы для немедленных чисел:

```
[0x00003d54]> ahi?
Usage: ahi [2|8|10|10u|16|bodhipSs] [@ offset]   Define numeric base
| ahi <base>  set numeric base (2, 8, 10, 16)
| ahi 10|d    set base to signed decimal (10), sign bit should depend on receiver size
| ahi 10u|du  set base to unsigned decimal (11)
| ahi b       set base to binary (2)
| ahi o       set base to octal (8)
| ahi h       set base to hexadecimal (16)
| ahi i       set base to IP address (32)
| ahi p       set base to htons(port) (3)
| ahi S       set base to syscall (80)
| ahi s       set base to string (1)

[0x00003d54]> pd 2
0x00003d54      0583000000     add eax, 0x83
0x00003d59      3d13010000     cmp eax, 0x113
[0x00003d54]> ahi d
[0x00003d54]> pd 2
0x00003d54      0583000000     add eax, 131
0x00003d59      3d13010000     cmp eax, 0x113
[0x00003d54]> ahi b
[0x00003d54]> pd 2
0x00003d54      0583000000     add eax, 10000011b
0x00003d59      3d13010000     cmp eax, 0x113
```

Примечательно, что некоторые этапы анализа или команды добавляют подсказки внутреннего анализа, которые можно проверить с помощью команды `ah` :

```
[0x00003d54]> ah
 0x00003d54 - 0x00003d54 => immbase=2
[0x00003d54]> ah*
 ahi 2 @ 0x3d54
```

Иногда нам нужно переопределить адрес перехода или вызова, например, в случае замысловатого перемещения, неизвестное radare2, можно изменить значение вручную.
Текущую аналитическую информацию о конкретном оп-коде можно проверить с помощью команды `ao` .
Используем команду `ahc` для выполнения такого изменения:

```
[0x00003cee]> pd 2
0x00003cee      e83d080100     call sub.__errno_location_530
0x00003cf3      85c0           test eax, eax
[0x00003cee]> ao
address: 0x3cee
opcode: call 0x14530
mnemonic: call
prefix: 0
id: 56
bytes: e83d080100
refptr: 0
size: 5
sign: false
type: call
cycles: 3
esil: 83248,rip,8,rsp,-=,rsp,=[],rip,=
jump: 0x00014530
direction: exec
fail: 0x00003cf3
stack: null
family: cpu
stackop: null
[0x00003cee]> ahc 0x5382
[0x00003cee]> pd 2
0x00003cee      e83d080100     call sub.__errno_location_530
0x00003cf3      85c0           test eax, eax
[0x00003cee]> ao
address: 0x3cee
opcode: call 0x14530
mnemonic: call
prefix: 0
id: 56
bytes: e83d080100
refptr: 0
size: 5
sign: false
type: call
cycles: 3
esil: 83248,rip,8,rsp,-=,rsp,=[],rip,=
jump: 0x00005382
direction: exec
fail: 0x00003cf3
stack: null
family: cpu
stackop: null
[0x00003cee]> ah
 0x00003cee - 0x00003cee => jump: 0x5382
```

Как видите, несмотря на неизменный вид дизассемблирования, адрес перехода в опкоде был изменен (настройка `jump`).

Если что-то из ранее описанного не помогло, можно просто переопределить показанное дизассемблирование чем-либо, что вам нравиться:

```
[0x00003d54]> pd 2
0x00003d54      0583000000     add eax, 10000011b
0x00003d59      3d13010000     cmp eax, 0x113
[0x00003d54]> "ahd myopcode bla, foo"
[0x00003d54]> pd 2
0x00003d54                     myopcode bla, foo
0x00003d55      830000         add dword [rax], 0
```
