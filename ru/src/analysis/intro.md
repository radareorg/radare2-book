# Анализ данных и кода

Radare2 имеет очень богатый набор команд и параметров конфигурации для выполнения анализа данных и кода, извлекать полезную информацию из двоичного файла, такую как указатели, строковые ссылки, базовые блоки, данные оп-кодов, назначения переходов, перекрестные ссылки и многое другое.
Эти операции обрабатываются семейством коман д`a` (analyze):

```
|Usage: a[abdefFghoprxstc] [...]
| aa[?]              analyze all (fcns + bbs) (aa0 to avoid sub renaming)
| a8 [hexpairs]      analyze bytes
| ab[b] [addr]       analyze block at given address
| abb [len]          analyze N basic blocks in [len] (section.size by default)
| abt [addr]         find paths in the bb function graph from current offset to given address
| ac [cycles]        analyze which op could be executed in [cycles]
| ad[?]              analyze data trampoline (wip)
| ad [from] [to]     analyze data pointers to (from-to)
| ae[?] [expr]       analyze opcode eval expression (see ao)
| af[?]              analyze Functions
| aF                 same as above, but using anal.depth=1
| ag[?] [options]    draw graphs in various formats
| ah[?]              analysis hints (force opcode size, ...)
| ai [addr]          address information (show perms, stack, heap, ...)
| an [name] [@addr]  show/rename/create whatever flag/function is used at addr
| ao[?] [len]        analyze Opcodes (or emulate it)
| aO[?] [len]        Analyze N instructions in M bytes
| ap                 find prelude for current offset
| ar[?]              like 'dr' but for the esil vm. (registers)
| as[?] [num]        analyze syscall using dbg.reg
| av[?] [.]          show vtables
| ax[?]              manage refs/xrefs (see also afx?)
```

Фактически, пространство имен `a` является одним из самых больших в инструменте radare2 и позволяет управлять разными частями процедуры анализа:

- Code flow analysis
- Анализ ссылок на данные
- Использование загруженных символов
- Управление различными типами графов, таких как CFG и граф вызовов
- Управление переменными
- Управление типами
- Эмуляция с помощью виртуальной машины ESIL
- Интроспекция оп-кодов
- Информация об объектах, например виртуальных таблиц

