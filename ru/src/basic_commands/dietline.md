# Dietline

Radare2 поставляется с [readline](https://en.wikipedia.org/wiki/GNU_Readline)-подобной командной строкой ввода, реализованная на основе "бережливого" принципа навигацией по истории и редактирования содержимого. Она позволяет пользователям передвигать курсор, перебирать предыдущие команды, обеспечивает автодополнение. Благодаря портативности Radare2, Dietline обеспечивает единый пользовательский интерфейс для всех поддерживаемых платформ. Он используется во всех подоболочках Rarade2 - основной командной строки, оболочки SDB, визуальных подсказок и подсказкзок смещения. Также реализованы наиболее распространенные функции и сочетания клавиш, совместимые с GNU Readline.

Dietline поддерживает два основных режима конфигурации: режимы Emacs и Vi.

Он также поддерживает знаменитый `Ctrl-R`, обратный поиск по истории. Клавиша `TAB` позволяет прокручивать варианты автодополнения.

# Автодополнение

В каждой оболочке и Radare2 поддерживается автодополнение команд. Есть несколько режимов - файлы, флаги и ключи/пространства имен SDB. Простой способ выбора возможных вариантов завершения - прокручиваемый всплывающий виджет. Он включается с помощью `scr.prompt.popup`, просто установите его в `true`.

# Режим Emacs (по умолчанию)

По умолчанию режим dietline совместим с привязками клавиш режима readline, подобного Emacs. Таким образом, активны:

## Перемещение
- `Ctrl-a` - move to the beginning of the line
- `Ctrl-e` - move to the end of the line
- `Ctrl-b` - move one character backward
- `Ctrl-f` - move one character forward

## Удаление
- `Ctrl-w` - delete the previous word
- `Ctrl-u` - delete the whole line
- `Ctrl-h` - delete a character to the left
- `Ctrl-d` - delete a character to the right
- `Alt-d` - cuts the character after the cursor

## Killing and Yanking
- `Ctrl-k` - kill the text from point to the end of the line.
- `Ctrl-x` - kill backward from the cursor to the beginning of the current line.
- `Ctrl-t` - kill from point to the end of the current word, or if between words, to the end of the next word. Границы слова такие же, как в forward-word.
- `Ctrl-w` - kill the word behind point, using white space as a word boundary. The killed text is saved on the kill-ring.
- `Ctrl-y` - yank the top of the kill ring into the buffer at point.
- `Ctrl-]` - rotate the kill-ring, and yank the new top. Вращение можно делать только если предыдущая команда была yank или yank-pop.

## История
- `Ctrl-r` - the reverse search in the command history

# Режим Vi

Radare2 также поставляется с режимом vi, подключаемый при помощи `scr.prompt.vi`. В этом режиме доступны сочетания клавиш:

## Вход в командные режимы
- `ESC` - enter into the control mode
- `i` - enter into the insert mode

## Перемещение
- `j` - acts like up arrow key
- `k` - acts like down arrow key
- `a` - move cursor forward and enter into insert mode
- `I` - move to the beginning of the line and enter into insert mode
- `A` - move to the end of the line and enter into insert mode
- `^` - move to the beginning of the line
- `0` - move to the beginning of the line
- `$` - move to the end of the line
- `h` - move one character backward
- `l` - move one character forward

## Deleting and Yanking
- `x` - cuts the character
- `dw` - delete the current word
- `diw` - deletes the current word.
- `db` - delete the previous word
- `D` - delete the whole line
- `dh` - delete a character to the left
- `dl` - delete a character to the right
- `d$` - kill the text from point to the end of the line.
- `d^` - kill backward from the cursor to the beginning of the current line.
- `de` - kill from point to the end of the current word, or if between words, to the end of the next word. Границы слова такие же, как в forward-word.
- `p` - yank the top of the kill ring into the buffer at point.
- `c` - acts similar to d based commands, but goes into insert mode in the end by prefixing the commands with numbers, the command is performed multiple times.

Если непонятно, в каком режиме вы находитесь, установите `scr.prompt.mode=true`, произойдет обновление цвета подсказки в зависимости от режима vi.
