## Менеджер пакетов Radare2

Radare2 имеет собственный менеджер пакетов для управления внешними плагинами (в основном от [radare2-extra](https://github.com/radareorg/radare2-extras)). Как и большинство менеджеров пакетов, он позволяет устанавливать, удалять или обновлять пакеты (плагины). Однако есть одно ограничение - в настоящее время он поддерживает только компиляцию из исходных кодов, поэтому предоставление инструментария, инструментов сборки и зависимостей для каждого плагина является ответственностью пользователя. Это может измениться в будущем.

```
$ r2pm -ч
$R2PM_DBDIR: Нет такого файла или каталога.
Запустите 'r2pm init' для инициализации репозитория пакетов
$ r2pm init
git clone https://github.com/radareorg/radare2-pm
Cloning into 'radare2-pm'...
remote: Counting objects: 147, done.
remote: Compressing objects: 100% (139/139), done.
remote: Total 147 (delta 26), reused 57 (delta 7), pack-reused 0
Receiving objects: 100% (147/147), 42.68 KiB | 48.00 KiB/s, done.
Resolving deltas: 100% (26/26), done.
/home/user/.local/share/radare2/r2pm/git/radare2-pm
r2pm database initialized. Используйте 'r2pm update' для обновления
```

Как вы заметили, база данных пакетов расположена по адресу [radare2-pm](https://github.com/radareorg/radare2-pm). В любой момент времени мы можем обновить базу данных с помощью `r2pm update`:

```
r2pm update
HEAD is now at 7522928 Fix syntax
Updating 7522928..1c139e0
Fast-forward
 db/ldid2 | 18 ++++++++++++++++++
 1 file changed, 18 insertions(+)
 create mode 100644 db/ldid2
Updating /home/user/.local/share/radare2/r2pm/db ...
Already up to date.
```

Теперь доступно множество команд:
```
r2pm -h
Usage: r2pm [init|update|cmd] [...]
Commands:
 -I,info                     показать информацию о пакете
 -i,install <pkgname>        установить пакет в домашний каталог (pkgname=all)
 -gi,global-install <pkg>    установить пакет в систему
 -gu,global-uninstall <pkg>  удалить пакет из системной папки
 -u,uninstall <pkgname>      r2pm -u baleful (-uu to force)
 -l,list                     перечислить установленные пакеты
 -r,run [cmd ...args]        запустить команды оболочки при помощи R2PM_BINDIR в PATH
 -s,search [<keyword>]       искать в базе данных
 -t,test FX,XX,BR BID        загрузить регрессии Travis-а
 -v,version                  показать версию
 -h,help                     показать это сообщение
 -H variable                 показать значение заданной переменной
 -c,clean ([git/dir])        очистить исходный кэш (GITDIR)
 -ci (pkgname)               очистить данные инсталляции заданного пакета
 -cp                         очистить папку плагинов пользователя
 -d,doc [pkgname]            показать документацию для заданного пакета
 -w <pkgname>                что и где установлено
 init | update ..            инициализация/обновление базы данных
 cd [git/dir]                перейти в заданный git (смотри 'r2pm ls')
 ls                          перечислить все склонированные git-репозитории в GITDIR
 suicide                     удалить всю установку r2, включая себя, домашний директорий, систему
 cache                       содержимое кэша r2, -H чтобы сделать r2pm независимым от r2
Environment:
 SUDO=sudo                   использовать эту программу как sudo
 R2PM_PLUGDIR=~/.local/share/radare2/plugins   # значение по умолчанию, установка в домашний директорий для плагинов
 R2PM_BINDIR=~/.local/share/radare2/prefix/bin # папка с бинариками r2, PATH
 R2PM_PLUGDIR=/usr/lib/radare2/last/           # для системной установки устаноленные плагины
 R2PM_DBDIR=~/.local/share/radare2/r2pm/db     # месторасположение базы данных
 R2PM_GITDIR=~/.local/share/radare2/r2pm/git   # репозитории клонированных git
```

Обычный сценарий его использования заключается в установке новых плагинов, например `lang-python3` (который используется для создания плагинов r2 на Python):

```
$ r2pm -i lang-python3
...
mkdir -p ~/.config/radare2/plugins
cp -f lang_python3.so ~/.config/radare2/plugins
...
```

Обратите внимание, что если использовали флаг `-i`, он устанавливает плагин в каталог `$HOME`, в случае `-gi` он устанавливается плагин по всей системе (`требуется sudo`).

После этого видим плагин в списке установленных:

```
$ r2pm -l
lang-python3
```

Чтобы удалить плагин, просто запустите

```
$ r2pm -u lang-python3
```

