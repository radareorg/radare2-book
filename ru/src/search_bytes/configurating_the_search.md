## Настройка параметров поиска

Поисковая система radare2 настраивается с помощью нескольких конфигурационных переменных, изменяемых с помощью команды `e`.
```
e cmd.hit = x         ; radare2 command to execute on every search hit
e search.distance = 0 ; search string distance
e search.in = [foo]   ; pecify search boundarie. Поддерживаемые значения перечислены в разделе e search.in=??
e search.align = 4    ; only show search results aligned by specified boundary.
e search.from = 0     ; start address
e search.to = 0       ; end address
e search.asmstr = 0   ; search for string instead of assembly
e search.flags = true ; if enabled, create flags on hits
```
Переменная `search.align` используется для ограничения допустимых поисковых запросов определенным выравниванием. Например, при помощи `e search.align=4` будут выведены только результаты, найденные при 4-байтовых смещениях.

Логическая переменная `search.flags` инструктирует поисковую систему помечать результаты (hits), чтобы на них ссылаться позже. Если текущий поиск прерывается с помощью последовательности клавиш `Ctrl-C`, текущая позиция поиска помечается `search_stop`.
