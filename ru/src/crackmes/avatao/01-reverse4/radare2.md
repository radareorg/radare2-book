.radare2
--------

Решим задачу обратной инженерии, используя [radar2](http://www.radare.org/r/), свободный инструмет реверс-инжиниринга с открытым исходным кодом. Я впервые узнал о r2 еще в 2011 году,  когда участвовал в большом проекте, где пришлось взламывать огромный статический ELF размером 11 МБ. Нужен был инструмент, позволяющий легко вносить исправления в Linux ELF. Тогда я использовал r2 вместе с IDA и то только для небольших задач,  мне с первого взгляда понравилась вся концепция. С тех пор radare2 сильно развился, и я решил уделять немного времени  решению задач crackme при помощи этого инструмента, также писать по результатам статьи. Что ж, этот CTF дал мне прекрасную возможность :)

Статья нацелена на то, чтобы показать некоторые особенности r2 в процессе решения задачи crackme. Я объясню каждую использованную команду r2 в виде цитат, подобных этой:

> ***Совет от r2:*** Всегда используй ? или флаг -h - больше информации!

Если владеете r2 и просто интересуетесь задачками на взлом, пропустите эти разделы! Обращаю внимание, что из-за стиля дальнейшего изложения я собираюсь детально разбирать приемы, которые обычно не делаются во время CTF, так как нет на это достаточно времени (например, пометить **каждую** область памяти в соответствии с ее предназначением). С небольшими исполняемыми файлами crackme можно получить результат, не вдаваясь в детали.

Дам несколько советов в изучении radare2, и, раз вы интересуетесь реверс-инженирингом, вас как мимнимум должен заинтересовать r2 :):

Пакет включает множество дополнительных инструментов и огромное количество функций. Все они очень хорошо документированы. Призываю вас ознакомиться с руководством и использовать встроенную систему справки, добляя в конец команды ? to any command)
extensively! Пример:

```
[0x00000000]> ?
Usage: [.][times][cmd][~grep][@[@iter]addr!size][|>pipe] ; ...
Append '?' to any char command to get detailed help
Prefix with number to repeat command N times (f.ex: 3x)
|%var =valueAlias for 'env' command
| *off[=[0x]value]     Pointer read/write data/values (see ?v, wx, wv)
| (macro arg0 arg1)    Manage scripting macros
| .[-|(m)|f|!sh|cmd]   Define macro or load r2, cparse or rlang file
| = [cmd]              Run this command via rap://
| /                    Search for bytes, regexps, patterns, ..
| ! [cmd]              Run given command as in system(3)
| # [algo] [len]       Calculate hash checksum of current block
| #!lang [..]          Hashbang to run an rlang script
| a                    Perform analysis of code
| b                    Get or change block size

...

[0x00000000]> a?
|Usage: a[abdefFghoprxstc] [...]
| ab [hexpairs]     analyze bytes
| aa                analyze all (fcns + bbs) (aa0 to avoid sub renaming)
| ac [cycles]       analyze which op could be executed in [cycles]
| ad                analyze data trampoline (wip)
| ad [from] [to]    analyze data pointers to (from-to)
| ae [expr]         analyze opcode eval expression (see ao)
| af[rnbcsl?+-*]    analyze Functions
| aF                same as above, but using anal.depth=1

...
```

Проект находится в стадии активной разработки - нет дня без фиксации в репозитории GitHub. Как говорит файле readme, всегда нужно использовать версию git!

Некоторые рекомендуемые материалы для чтения:

- [Шпаргалка от pwntester-а](https://github.com/pwntester/cheatsheets/blob/master/radare2.md)
- [Книга Radare2](https://www.gitbook.com/book/radareorg/radare2-book/details)
- [Блог Radare2](http://radare.today)
- [Radare2 Wiki](https://github.com/radareorg/radare2/wiki)
