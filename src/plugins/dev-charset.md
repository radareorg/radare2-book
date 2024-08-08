## Charset Plugins

1. Create a file in `radare2/libr/util/d/yourfile.sdb.txt`. The extension .sdb.txt is important.
2. Edit the file `radare2/libr/util/charset.c`.
  -add `extern SdbGperf gperf_latin_1_ISO_8859_1_western_european;`.
  -then add your variable `&gperf_latin_1_ISO_8859_1_western_european,` in `static const SdbGperf *gperfs[]`
3. Update the Makefile: `radare2/libr/util/Makefile`:
  -Add `OBJS+=d/latin_1_ISO_8859_1_western_european.o`
4. Update the Makefile `radare2/libr/util/d/Makefile` to add your file name with not .sdb and not .txt in `FILES=latin_1_ISO_8859_1_western_european`
5. Update the unit tests of `radare2/test/db/cmd/charset`

Congratulation! You can now type the command:

```
e cfg.charset=latin_1_ISO_8859_1_western_european;
```

If you have any issue with this tutorial you can check out the example at <https://github.com/radareorg/radare2/pull/19627/files>.
