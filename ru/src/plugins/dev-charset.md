## Кодировка

1-Создайте файл `radare2/libr/util/d/yourfile.sdb.txt`. Расширение .sdb.txt имеет важное значение.
2-Отредактируйте файл `radare2/libr/util/charset.c`.
-добавьте `extern SdbGperf gperf_latin_1_ISO_8859_1_western_european;`.
-then add your variable `&gperf_latin_1_ISO_8859_1_western_european,` in `static const SdbGperf *gperfs[]`
3-Update the Makefile: `radare2/libr/util/Makefile`:
-Add `OBJS+=d/latin_1_ISO_8859_1_western_european.o`
4-Update the Makefile `radare2/libr/util/d/Makefile` to add your file name with not .sdb and not .txt in `FILES=latin_1_ISO_8859_1_western_european`
5-Update the unit tests of `radare2/test/db/cmd/charset`

Поздравляем! Теперь вводим команду:

```
e cfg.charset=latin_1_ISO_8859_1_western_european;
```


Если полученный опыт противоречит изложенному, изучайте примеры на https://github.com/radareorg/radare2/pull/19627/files.
