## Файлы radare2

Используйте `r2 -H` для перечисления всех переменных среды, управляющих поиском файлов на диске. Путь поиска зависит от настроек вашей сборки r2 и операционной системы.

```
R2_PREFIX=/usr
MAGICPATH=/usr/share/radare2/2.8.0-git/magic
PREFIX=/usr
INCDIR=/usr/include/libr
LIBDIR=/usr/lib64
LIBEXT=so
RCONFIGHOME=/home/user/.config/radare2
RDATAHOME=/home/user/.local/share/radare2
RCACHEHOME=/home/user/.cache/radare2
LIBR_PLUGINS=/usr/lib/radare2/2.8.0-git
USER_PLUGINS=/home/user/.local/share/radare2/plugins
USER_ZIGNS=/home/user/.local/share/radare2/zigns
```

## Файлы RC

RC-файлы — это сценарии r2, которые загружаются во время запуска. Эти файлы должны находиться в трех разных местах:

* Системная, radare2 сначала попытается загрузить /usr/share/radare2/radare2rc,
* В домашнем директории.

Каждый пользователь в системе может настраивать свои собственные сценарии r2 для запуска, например, выбор цветовой схемы, а также другие параметры, задаваемые там командами r2.

* ~/.radare2rc,
* ~/.config/radare2/radare2rc,
* ~/.config/radare2/radare2rc.d/.

### Целевой файл

Если надо запускать сценарий каждый раз при открытии файла, просто создайте файл с тем же именем, что и бинарный файл, но добавив к нему `.r2`.
