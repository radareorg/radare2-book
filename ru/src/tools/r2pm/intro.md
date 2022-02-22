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
 -I,info                     show information about a package
 -i,install <pkgname>        install package in your home (pkgname=all)
 -gi,global-install <pkg>    install package system-wide
 -gu,global-uninstall <pkg>  uninstall pkg from systemdir
 -u,uninstall <pkgname>      r2pm -u baleful (-uu to force)
 -l,list                     list installed pkgs
 -r,run [cmd ...args]        run shell command with R2PM_BINDIR in PATH
 -s,search [<keyword>]       search in database
 -t,test FX,XX,BR BID        check in Travis regressions
 -v,version                  show version
 -h,help                     show this message
 -H variable                 show value of given variable
 -c,clean ([git/dir])        clear source cache (GITDIR)
 -ci (pkgname)               clean install of given package
 -cp                         clean the user's home plugin directory
 -d,doc [pkgname]            show documentation for given package
 -w <pkgname>                what/where is installed
 init | update ..            инициализация/обновление базы данных
 cd [git/dir]                cd into given git (see 'r2pm ls')
 ls                          ls all cloned git repos in GITDIR
 suicide                     self remove all (home + system) installations of r2
 cache                       cache contents of r2 -H to make r2pm r2-independent
Environment:
 SUDO=sudo                    use this tool as sudo
 R2PM_PLUGDIR=~/.local/share/radare2/plugins   # default value, home-install for plugins
 R2PM_BINDIR=~/.local/share/radare2/prefix/bin # bindir in r2's PATH
 R2PM_PLUGDIR=/usr/lib/radare2/last/           # for system-wide plugin installs
 R2PM_DBDIR=~/.local/share/radare2/r2pm/db     # location of the db
 R2PM_GITDIR=~/.local/share/radare2/r2pm/git   # cloned git repositories
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

