# Отладчик

Отладчики реализуются при помощи плагинов ввода-вывода. Поэтому radare2 может обрабатывать различные типы URI, порождая, присоединяясь и управляя процессами. Полный список плагинов доступен при использовании флага `r2 -L` при запуске. Плагины с буквой  "d" в первом столбце ("rwd") таблицы поддерживают отладку. Например:

```
r_d  debug       Отладить программу или процесс по pid. dbg:///bin/ls, dbg://1388 (LGPL3)
rwd  gdb         Подсоединиться к серверу gdb, 'qemu -s', gdb://localhost:1234 (LGPL3)
```

Существуют разные бэкенды для многих целевых архитектур и операционных систем, например, GNU/Linux, Windows, MacOS X, (Net,Free,Open)BSD и Solaris.

Память процесса обрабатывается как обычный файл. Все отображаемые страницы памяти отлаживаемой программы и ее библиотек читаются и интерпретируются как код или структуры данных.

Связь между radare и уровнем ввода-вывода отладчика обрамлена в вызовы `system()`, принимающий строку в качестве аргумента и выполняющий ее как команду. Затем ответ буферизуется в консоли вывода, его содержимое может быть дополнительно обработано скриптом. Доступ к системе ввода-вывода осуществляется с помощью `=!`. Большинство подключаемых модулей ввода-вывода снабжены инструкциями, доступными при помощи `=!?` и `=!help`. Например:

```
$ r2 -d /bin/ls
...
[0x7fc15afa3cc0]> =!help
Usage: =!cmd args
 =!ptrace   - использовать ptrace io
 =!mem      - использовать /proc/pid/mem io, если возможно
 =!pid      - показать целевой pid
 =!pid <#>  - выбрать новый pid
```

Как правило, команды отладчика переносимы между архитектурами и операционными системами. Radare пытается поддерживать одну и ту же функциональность для всех целевых архитектур и операционных систем, но некоторые вещи приходится решать отдельно. Они включают внедрение шелл-кодов и обработку исключений. Например, в архитектурах MIPS нет аппаратной поддержки пошагового запуска инструкций. В этом случае radare2 предоставляет собственную реализацию пошагового выполнения, используя сочетание анализа кода и программных точек останова.

Базовая справка по отладчику доступна по команде 'd?':

```
Usage: d   # Debug commands
| db[?]                    Breakpoints commands
| dbt[?]                   Display backtrace based on dbg.btdepth and dbg.btalgo
| dc[?]                    Continue execution
| dd[?]                    File descriptors (!fd in r1)
| de[-sc] [perm] [rm] [e]  Debug with ESIL (see de?)
| dg <file>                Generate a core-file (WIP)
| dH [handler]             Transplant process to a new handler
| di[?]                    Show debugger backend information (See dh)
| dk[?]                    List, send, get, set, signal handlers of child
| dL[?]                    List or set debugger handler
| dm[?]                    Show memory maps
| do[?]                    Open process (reload, alias for 'oo')
| doo[args]                Reopen in debug mode with args (alias for 'ood')
| doof[file]               Reopen in debug mode from file (alias for 'oodf')
| doc                      Close debug session
| dp[?]                    List, attach to process or thread id
| dr[?]                    Cpu registers
| ds[?]                    Step, over, source line
| dt[?]                    Display instruction traces
| dw <pid>                 Block prompt until pid dies
| dx[?]                    Inject and run code on target process (See gs)
```

Чтобы перезапустить сеанс отладки, можно ввести `oo` или `оо+` в зависимости от задачи.

```
oo                 заново открыть текущий файл (kill+fork в отладчике)
oo+                заново открыть текущий файл в режиме перезаписи
```
