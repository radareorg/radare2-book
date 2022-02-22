# Сигнатуры

Radare2 реализует собственный формат сигнатур, позволяющий как их загружать/применять, так и создавать на лету. Они доступны в пространстве имен команд `z`:

```
[0x00000000]> z?
Usage: z[*j-aof/cs] [args]   # Manage zignatures
| z            show zignatures
| z.           find matching zignatures in current offset
| zb[?][n=5]   search for best match
| z*           show zignatures in radare format
| zq           show zignatures in quiet mode
| zj           show zignatures in json format
| zk           show zignatures in sdb format
| z-zignature  delete zignature
| z-*          delete all zignatures
| za[?]        add zignature
| zg           generate zignatures (alias for zaF)
| zo[?]        manage zignature files
| zf[?]        manage FLIRT signatures
| z/[?]        search zignatures
| zc[?]        compare current zignspace zignatures with another one
| zs[?]        manage zignspaces
| zi           show zignatures matching information
```
Для загрузки созданного файла сигнатур необходимо загрузить его из SDB-файла с помощью команды `zo` или из сжатого SDB-файла - `zoz`.

Для создания сигнатуры нужно сначала определить функцию, затем из нее сигнатуру:
```
r2 /bin/ls
[0x000051c0]> aaa # this creates functions, including 'entry0'
[0x000051c0]> zaf entry0 entry
[0x000051c0]> z
entry:
  bytes: 31ed4989d15e4889e24883e4f050544c............48............48............ff..........f4
  graph: cc=1 nbbs=1 edges=0 ebbs=1
  offset: 0x000051c0
[0x000051c0]>
```
Как видите, сделана новая сигнатура с именем `entry` из функции `entry0`.
Также можете показать сигнатуру в формате JSON, полезный для сценариев:
```
[0x000051c0]> zj~{}
[
  {
    "name": "entry",
    "bytes": "31ed4989d15e4889e24883e4f050544c............48............48............ff..........f4",
    "graph": {
      "cc": "1",
      "nbbs": "1",
      "edges": "0",
      "ebbs": "1"
    },
    "offset": 20928,
    "refs": [
    ]
  }
]
[0x000051c0]>
```
Чтобы удалить его, просто запустите `z-entry`.

Если надо сохранить все созданные подписи, сохраняйте их в SDB-файл с помощью команды `zos myentry`.

Позже их можно применить. Давайте снова откроем файл:
```
r2 /bin/ls
 -- Войти. Взломать. Перейти куда угодно. Получить все.
[0x000051c0]> zo myentry
[0x000051c0]> z
entry:
  bytes: 31ed4989d15e4889e24883e4f050544c............48............48............ff..........f4
  graph: cc=1 nbbs=1 edges=0 ebbs=1
  offset: 0x000051c0
[0x000051c0]>
```
Это означает, что подписи успешно загружены из файла `myentry` и теперь можно искать совпадающие функции:
```
[0x000051c0]> z.
[+] searching 0x000051c0 - 0x000052c0
[+] searching function metrics
hits: 1
[0x000051c0]>
```
Обратите внимание, что команда `z.` просто проверяет сигнатуру по текущему адресу.
Чтобы искать сигнатуры по всему файлу, нужно сделать другую команду.
Есть важный момент, если просто запустить его «как есть», он ничего не найдет:
```
[0x000051c0]> z/
[+] searching 0x0021dfd0 - 0x002203e8
[+] searching function metrics
hits: 0
[0x000051c0]>
```
Обратите внимание на поисковый адрес - нам нужно сначала [настроить диапазон поиска](../search_bytes/configurating_the_search.md):
```
[0x000051c0]> e search.in=io.section
[0x000051c0]> z/
[+] searching 0x000038b0 - 0x00015898
[+] searching function metrics
hits: 1
[0x000051c0]>
```
Мы устанавливаем режим поиска в `io.section` (это был `файл` по умолчанию) на поиск в текущей секции (при условии, что мы в настоящее время находимся в разделе `.text</g>).
Теперь можно пjcvjnhtnm, что radare2 нашел для нас:
```
[0x000051c0]> pd 5
;-- entry0:
;-- sign.bytes.entry_0:
0x000051c0      31ed           xor ebp, ebp
0x000051c2      4989d1         mov r9, rdx
0x000051c5      5e             pop rsi
0x000051c6      4889e2         mov rdx, rsp
0x000051c9      4883e4f0       and rsp, 0xfffffffffffffff0
[0x000051c0]>
```
Здесь видно комментарий `entry0`, который взят из разбора ELF, а также
`sign.bytes.entry_0`, которая является именно результатом совпадения сигнатуры.

Конфигурация сигнатур хранится в переменных настройки среды в пространстве имен `zign.`:
```
[0x000051c0]> e? zign.
       zign.autoload: Автозагрузка всех зигнатур, расположенных в ~/.local/share/radare2/zigns
          zign.bytes: Использование шаблонов байтов для сопоставления
   zign.diff.bthresh: Пороговое значение для дифференциации zign-байтов [0, 1] (см. zc?)
   zign.diff.gthresh: Порог для дифференцированных графиков zign [0, 1] (см. zc?)
          zign.graph: Использование метрик графа для сопоставления
           zign.hash: Использование хэша для сопоставления
          zign.maxsz: Максимальная длина сигнатуры
          zign.mincc: Минимальная цикломатическая сложность для сопоставления
          zign.minsz: Минимальная длина сигнатуры для сопоставления
         zign.offset: Используйте исходное смещение для сопоставления
         zign.prefix: Префикс по умолчанию для совпадений сигнатур
           zign.refs: Использолать ссылки для сопоставления
      zign.threshold: Минимальное сходство, необходимое для включения в результаты zb
          zign.types: Использование типов для сопоставления
[0x000051c0]>
```

## Поиск лучших совпадений `zb`

Часть бывает, что сигнатура должна существовать где-то в двоичном файле, но `z/` и `z.` все еще терпят неудачу. Это часто связано с очень незначительными различиями между
сигнатурой и функцией. Может быть компилятор переключил две инструкции или сигнатура не для подходящей версии функции. В этих ситуациях команды `zb` по-прежнему могут помочь указать вам правильное направление, перечислив примерные совпадения.

```
[0x000040a0]> zb?
Usage: zb[r?] [args]  # search for closest matching signatures
| zb [n]           find n closest matching zignatures to function at current offset
| zbr zigname [n]  search for n most similar functions to zigname
```

Команда `zb` (zign best) покажет пять наиболее близких сигнатур к
функция. Каждый из них будет содержать оценку от 1,0 до 0,0.

```
[0x0041e390]> s sym.fclose
[0x0040fc10]> zb
0.96032  0.92400 B  0.99664 G   sym.fclose
0.65971  0.35600 B  0.96342 G   sym._nl_expand_alias
0.65770  0.37800 B  0.93740 G   sym.fdopen
0.65112  0.35000 B  0.95225 G   sym.__run_exit_handlers
0.62532  0.34800 B  0.90264 G   sym.__cxa_finalize
```

В приведенном выше примере `zb` правильно связал подпись `sym.fclose` с текущей функцией. Команды `z/` и `z.` не сработали бы здесь,
так как оценки `B`yte и `Gh`rap меньше 1,0. Разница 30% между результатами первого и второго места также является хорошим показателем правильного соответствия.

`Zbr` (zign best reverse) принимает имя сигнатуры и пытается найти
наиболее близкое соотеетствие. Используйте команду анализа, например `aa`, чтобы найти сначал функции.

```
[0x00401b20]> aa
[x] Analyze all flags starting with sym. and entry0 (aa)
[0x00401b20]> zo ./libc.sdb
[0x00401b20]> zbr sym.__libc_malloc 10
0.94873  0.89800 B  0.99946 G   sym.malloc
0.65245  0.40600 B  0.89891 G   sym._mid_memalign
0.59470  0.38600 B  0.80341 G   sym._IO_flush_all_lockp
0.59200  0.28200 B  0.90201 G   sym._IO_file_underflow
0.57802  0.30400 B  0.85204 G   sym.__libc_realloc
0.57094  0.35200 B  0.78988 G   sym.__calloc
0.56785  0.34000 B  0.79570 G   sym._IO_un_link.part.0
0.56358  0.36200 B  0.76516 G   sym._IO_cleanup
0.56064  0.26000 B  0.86127 G   sym.intel_check_word.constprop.0
0.55726  0.28400 B  0.83051 G   sym.linear_search_fdes
```
