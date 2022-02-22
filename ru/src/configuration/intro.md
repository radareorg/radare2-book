# Конфигурация

Ядро читает `~/.config/radare2/radare2rc` во время запуска. Можете добавлять `е` команды в этот файл для настройки конфигурацию Radare2 по своему вкусу.

Для предотвращения загрузки этого файла при запуске, установите флаг командной строки `-N`.

Вся конфигурация radare2 выполняется с помощью команд установки `переменных среды`. Типичный файл конфигурации запуска выглядит так:
```sh
$ cat ~/.radare2rc
e scr.color = 1
e dbg.bep   = loader
```
Конфигурацию также можно изменить с помощью флагов формата `-е` <config=value> в строке параметров при звапуске. Таким образом, можно подправить конфигурацию из командной строки, сохранив файл .radare2rc нетронутым. Например, чтобы начать с пустой конфигурации, а затем настроить `scr.color` а также `asm.syntax` можно использовать следующую строку:
```sh
$ radare2 -N -e scr.color=1 -e asm.syntax=intel -d /bin/ls
```
Внутри конфигурация хранится в хеш-таблице. Переменные сгруппированы в пространства имен: `cfg.`, `file.`, `dbg.`, `scr.` и так далее.

Чтобы получить список всех переменных конфигурации, просто введите `е` в командной строке r2. Чтобы ограничить вывод выбранным пространством имен, передайте его с точкой в конце `е <имя пространства>.` Например, `е file.` отобразит все переменные, определенные внутри пространства имен «file».

Чтобы получить инструкции о команде `e` - `e?`:

```
Usage: e [var[=value]]  Evaluable vars
| e?asm.bytes     show description
| e??             list config vars with description
| e a             get value of var 'a'
| e a=b           set var 'a' the 'b' value
| e var=?         print all valid values of var
| e var=??        print all valid values of var with description
| e.a=b           same as 'e a=b' but without using a space
| e,k=v,k=v,k=v   comma separated k[=v]
| e-              reset config vars
| e*              dump config vars in r commands
| e!a             invert the boolean value of 'a' var
| ec [k] [color]  set color for given key (prompt, offset, ...)
| eevar           open editor to change the value of var
| ed              open editor to change the ~/.radare2rc
| ej              list config vars in JSON
| env [k[=v]]     get/set environment variable
| er [key]        set config key as readonly. no way back
| es [space]      list all eval spaces [or keys]
| et [key]        show type of given config variable
| ev [key]        list config vars in verbose format
| evj [key]       list config vars in verbose format in JSON
```

Более простая альтернатива команде `e` доступна из визуального режима. Набеите `Ve` чтобы войти в этот режим, используйте стрелки (вверх, вниз, влево, вправо) для навигации по конфигурации и `q`, чтобы выйти из него. В начале экран визуального редактирования конфигурации выглядит так:

```
[EvalSpace]

    >  anal
       asm
       scr
       asm
       bin
       cfg
       diff
       dir
       dbg
       cmd
       fs
       hex
       http
       graph
       hud
       scr
       search
       io
```

Для значений конфигурации, которые могут принимать одно из нескольких значений, вы можете использовать `=?`, оператор для получения списка допустимых значений:

```
[0x00000000]> e scr.nkey = ?
scr.nkey = fun, hit, flag
```
