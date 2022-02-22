## Кодировка

1. Создайте файл `radare2/libr/util/d/yourfile.sdb.txt`. Расширение .sdb.txt имеет важное значение.
2. Отредактируйте файл `radare2/libr/util/charset.c`:
  - добавьте `extern SdbGperf perf_latin_1_ISO_8859_1_western_european;`,
  - затем добавьте вашу переменную `&gperf_latin_1_ISO_8859_1_western_european,` в `static const SdbGperf *gperfs[]`.
3. Исправьте Makefile: `radare2/libr/util/Makefile`:
  - добавив `OBJS+=d/latin_1_ISO_8859_1_western_european.o`.
4. Исправьте Makefile `radare2/libr/util/d/Makefile`: надо добавить ваш файл с not .sdb и not .txt в `FILES=latin_1_ISO_8859_1_western_european`.
5. Обновите unit-тесты для `radare2/test/db/cmd/charset`.

Поздравляем! Теперь вводим команду:

```
e cfg.charset=latin_1_ISO_8859_1_western_european;
```


Если полученный опыт противоречит изложенному, изучайте примеры на https://github.com/radareorg/radare2/pull/19627/files.
