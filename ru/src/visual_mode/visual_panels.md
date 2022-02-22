# Визуальные панели

## Концепция

Визуальные панели характеризуются следующими основными функциональными возможностями:

1. Разделение экрана,
2. Отображение нескольких экранов, таких как символы, регистры, стек, а также пользовательские панели,
3. Меню охватывает все эти часто используемые команды, так что вам не нужно запоминать ни одну из них.

Для CUI разработаны полезные графические интерфейсы в качестве меню, то есть визуальные панели.

Доступ к панелям можно получить с помощью команды `v` или с помощью `!` из визуального режима.

## Обзор

![Обзор панелей](panels_overview.png)

## Команды
```
|Visual Ascii Art Panels:
| |      split the current panel vertically
| -      split the current panel horizontally
| :      run r2 command in prompt
| ;      add/remove comment
| _      start the hud input mode
| \      show the user-friendly hud
| ?      показать справку
| !      запустить игру r2048
| .      seek to PC or entrypoint
| *      show decompiler in the current panel
| "      create a panel from the list and replace the current one
| /      highlight the keyword
| (      toggle snow
| &      toggle cache
| [1-9]  follow jmp/call identified by shortcut (like ;[1])
| ' '    (space) toggle graph / panels
| tab    go to the next panel
| Enter  start Zoom mode
| a      toggle auto update for decompiler
| b      browse symbols, flags, configurations, classes, ...
| c      toggle cursor
| C      toggle color
| d      define in the current address. Same as Vd
| D      show disassembly in the current panel
| e      change title and command of current panel
| f      set/add filter keywords
| F      remove all the filters
| g      go/seek to given offset
| G      go/seek to highlight
| i      insert hex
| hjkl   move around (left-down-up-right)
| HJKL   move around (left-down-up-right) by page
| m      select the menu panel
| M      open new custom frame
| n/N    seek next/prev function/flag/hit (scr.nkey)
| p/P    rotate panel layout
| q      quit, or close a tab
| Q      close all the tabs and quit
| r      toggle callhints/jmphints/leahints
| R      randomize color palette (ecr)
| s/S    step in / step over
| t/T    tab prompt / close a tab
| u/U    undo / redo seek
| w      start Window mode
| V      go to the graph mode
| xX     show xrefs/refs of current function from/to data/code
| z      swap current panel with the first one
```

## Базовые функции

Используйте клавишу `TAB` для перемещения по панелям, пока не дойдете до целевой. Клавиши `hjkl` - прокручивание панели, на которой вы сейчас находитесь. Используйте `S` и `s` для step over/in, и все панели будут динамически обновляться во время отладки. На панелях «Регистры» или «Стек» значения можно редактировать, вставив шестнадцатеричный формат. Об этом будет рассказано позже.

Использование `tab` позволяет вам перемещаться между панелями, настоятельно рекомендуется использовать `m` для открытия меню.
Как обычно, вы можете использовать `hjkl` для перемещения по меню и найдете там тонны полезных вещей. Также можно нажать `"` для быстрого просмотра различных настроек просмотра предложений и изменения содержимого выбранной панели.

## Разделение экрана

Символы `|` - вертикальное и `-` - горизонтальное разделение. Можно удалить любую панель, нажав клавишу `X`.

Размер разделенных панелей может быть изменен из оконного режима, доступ к которому осуществляется с помощью `w`.

## Команды оконного режима
```
|Справка по панели оконный режима:
| ?      показать эту справку
| ??     show the user-friendly hud
| Enter  start Zoom mode
| c      toggle cursor
| hjkl   move around (left-down-up-right)
| JK     resize panels vertically
| HL     resize panels horizontally
| q      quit Window mode
```

## Изменение значений

На панели «Регистр» или «Стек» можно редактировать значения. Используйте `c` для активации режима курсора, перемещать курсор - `hjkl`, как обычно. Затем нажмите `i`, как и в режиме вставки vim, чтобы вставить значение.

## Вкладки
Визуальные панели также предлагают вкладки для быстрого доступа к нескольким формам информации. Нажмите клавишу `t`, чтобы войти в режим вкладок. Номера вкладок будут видны в правом верхнем углу.

По умолчанию показывается одна вкладка, можно нажать `t` для создания новой вкладки с теми же панелями, и `T`, чтобы создать новую чистую панель.

Для обхода вкладок можно ввести номер вкладки.

И нажатие `-` удаляет вкладку, в которой вы находитесь.

## Сохранение макетов
Можно сохранить пользовательский макет визуальных панелей, выбрав опцию «Сохранить макет» в меню «Файл», либо выполнив:
```
v= test
```
Где `test` — это имя, под которым вы хотите сохранить настройки.

Сохраненный макет можно открыть, передав имя в качестве параметра `v`:
```
v test
```
Дополнительные инструкции можно узнать в разделе `v?`.
