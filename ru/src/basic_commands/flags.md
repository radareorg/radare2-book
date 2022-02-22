## Флаги

Флаги концептуально похожи на закладки. Они связывают имя с заданным смещением в файле. Флаги группируются в «пространства флагов». Пространство флагов — это пространство имен для флагов, объединяющее флаги со схожими характеристиками или типом. Примеры для флаговых пространств: секции (sections), регистры (registers), символы (symbols).

Создание флага:

```
[0x4A13B8C0]> f flag_name @ offset
```

Удаление флага - добавление символа `-` к команде. Большинство команд принимают `-` как аргумент-префикс как указание на удаление чего-либо.

```
[0x4A13B8C0]> f-flag_name
```

Для переключения между пространствами флагов или создания новых используйте команду `fs`:

```
[0x00005310]> fs?
|Usage: fs [*] [+-][flagspace|addr] # Manage flagspaces
| fs            display flagspaces
| fs*           display flagspaces as r2 commands
| fsj           display flagspaces in JSON
| fs *          select all flagspaces
| fs flagspace  select flagspace or create if it doesn't exist
| fs-flagspace  remove flagspace
| fs-*          remove all flagspaces
| fs+foo        push previous flagspace and set
| fs-           pop to the previous flagspace
| fs-.          удаление текущего пространства флагов
| fsq           list flagspaces in quiet mode
| fsm [addr]    move flags at given address to the current flagspace
| fss           display flagspaces stack
| fss*          display flagspaces stack in r2 commands
| fssj          display flagspaces stack in JSON
| fsr newname   rename selected flagspace
[0x00005310]> fs
0  439 * strings
1   17 * symbols
2   54 * sections
3   20 * segments
4  115 * relocs
5  109 * imports
[0x00005310]>
```

Вот несколько примеров команд:

```
[0x4A13B8C0]> fs symbols ; select only flags in symbols flagspace
[0x4A13B8C0]> f          ; list only flags in symbols flagspace
[0x4A13B8C0]> fs *       ; select all flagspaces
[0x4A13B8C0]> f myflag   ; create a new flag called 'myflag'
[0x4A13B8C0]> f-myflag  ; delete the flag called 'myflag'
```

Переименовывание флагов с помощью `fr` .

### Локальные флаги

Каждое имя флага должно быть уникальным по понятным причинам. Часто надо задавать флаги, например внутри функций, с простыми и вездесущими именами, такими как `loop` или `return`. Здесь можно использовать так называемые «локальные» флаги, которые привязаны к функции, в которой они находятся. Их можно добавить с помощью команды `f.`:

```
[0x00003a04]> pd 10
│      0x00003a04      48c705c9cc21.  mov qword [0x002206d8], 0xffffffffffffffff ;
[0x2206d8:8]=0
│      0x00003a0f      c60522cc2100.  mov byte [0x00220638], 0     ; [0x220638:1]=0
│      0x00003a16      83f802         cmp eax, 2
│  .─< 0x00003a19      0f84880d0000   je 0x47a7
│  │   0x00003a1f      83f803         cmp eax, 3
│ .──< 0x00003a22      740e           je 0x3a32
│ ││   0x00003a24      83e801         sub eax, 1
│.───< 0x00003a27      0f84ed080000   je 0x431a
││││   0x00003a2d      e8fef8ffff     call sym.imp.abort           ; void abort(void)
││││   ; CODE XREF from main (0x3a22)
││╰──> 0x00003a32      be07000000     mov esi, 7
[0x00003a04]> f. localflag @ 0x3a32
[0x00003a04]> f.
0x00003a32 localflag   [main + 210]
[0x00003a04]> pd 10
│      0x00003a04      48c705c9cc21.  mov qword [0x002206d8], 0xffffffffffffffff ;
[0x2206d8:8]=0
│      0x00003a0f      c60522cc2100.  mov byte [0x00220638], 0     ; [0x220638:1]=0
│      0x00003a16      83f802         cmp eax, 2
│  .─< 0x00003a19      0f84880d0000   je 0x47a7
│  │   0x00003a1f      83f803         cmp eax, 3
│ .──< 0x00003a22      740e           je 0x3a32                    ; main.localflag
│ ││   0x00003a24      83e801         sub eax, 1
│.───< 0x00003a27      0f84ed080000   je 0x431a
││││   0x00003a2d      e8fef8ffff     call sym.imp.abort           ; void abort(void)
││││   ; CODE XREF from main (0x3a22)
││`──>  .localflag:
││││   ; CODE XREF from main (0x3a22)
││`──> 0x00003a32      be07000000     mov esi, 7
[0x00003a04]>
```

### Флаги зон

Radare2 реализует механизм флагов зон, позволяющие помечать различные смещения на полосе прокрутки, чтобы упростить навигацию по большим двоичным файлам. Установка флага зоны по текущему смещению:

```
[0x00003a04]> fz flag-zone-name
```

Установите `scr.scrollbar=1` и перейдите в визуальный режим, зона флага появляется на полосе прокрутки с правой стороны окна.

Посмотрите инструкцию - `fz?`.