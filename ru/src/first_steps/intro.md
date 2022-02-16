## Базовое использование Radare2

Кривая обучения обычно несколько крута в только в самом начале. После часа использования уже можно легко понять, как работает большинство функций, и как можно комбинировать различные инструменты радара. Вам предлагается прочитать оставшуюся часть этой книги, чтобы понять, как работают некоторые нетривиальные вещи, и в конечном итоге улучшить свои навыки.

![learning_curve](learning_curve.png)

Перемещение по адресам, исследование и правка загруженного двоичного файла выполняются при помощи трех простых действий: перемещение по адресному пространству (в некоторый адрес), отображение (буфера) и внесение изменений (запись, добавление).

Команда «seek» обозначается аббревиатурой `s`, она принимает выражение в качестве аргумента. Выражение может быть следующего вида - `10`, `+0x25`, или даже `[0x100+ptr_table]`. Если вы хотите работать с блочными файлами, вы можете зафиксировать размер блока с помощью команды `b` и осуществлять перемещение по памяти вперед или назад по адресам байтов. Использование `s++` и `s--` аналогично позволяет перемещаться по памяти.

Если в radare2 открыть исполняемый файл, то по умолчанию он откроется в режиме виртуальной адресации (VA), и секции файла будут отображены в их виртуальные адреса. В режиме VA основа перемещения по адресам - этот виртуальный адрес, а начальная позиция устанавливается в точку входа исполняемого файла. Использование ключа `-n` позволяет отменить загрузку в данном режиме, при этом radare2 откроет файл в не-VA режиме. В режиме не-VA перемещение по адресам осуществляется относительно начала файла.

Команда 'print' также имеет сокращенный вариант `p` и включает несколько вариантов функционирования, вторая буква указывает как раз требуемый режим отображения. Наиболее часто встречаются `px`, отображающий данные в виде шестнадцатеричного дампа, `pd` - дизассемблирование кода.

Чтобы radare2 мог сохранять изменения в исследуемых файлах, надо указать флаг `-w` в командной строке при открытии файла. Команда `w` используется для записи строк в исходный файл, шестнадцатеричных кодов (режим `x`) или даже ассемблерные коды операций (режим `a`). Примеры:

```
> w hello world         ; строка
> wx 90 90 90 90        ; шеснадцатеричные данные
> wa jmp 0x8048140      ; ассемблирование
> wf inline.bin         ; запись содержимого из файла
```

Добавление символа `?` к любой команде показывает подсказку, например, `p?`.
Если написать `?*`, то получим список команд, начинающихся с заданной строки, например, `p?*`.

Визуальный режим запускается вводом `V<enter>`. По клавише `q` можно выйти из этого режима и вернуться в привычный режим командной строки.

В визуальном режиме для перемещений используются клавиши HJKL (влево, вниз, вверх и вправо, соответственно). Также можно использовать эти клавиши в режиме курсора, режим переключается клавшей `c`. Чтобы выбрать диапазон байтов в режиме курсора, одновременно нажмите `Shift` и клавиши навигации HJKL, в результате radare2 выделит область.

В визуальном режиме также можно переписывать байты, нажав `i`. Нажатием клавиши `TAB` осуществляется перемещение между панелями шестнадцатеричных кодов (посередине) и строчным представлением (справа). Нажатие `q` в панели шестнадцатеричного представления приводит к возвращению в визуальный режим. Нажатием `p` или `P` приводит к циклическому переключению различных режимов визуального представления. Существует второй по важности визуальный режим — интерфейс панелей, похожий на curses, доступный при помощи комбинации `V!`.