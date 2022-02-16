## Компиляция и переносимость

В настоящее время ядро radare2 можно скомпилировать для многих операционных систем и архитектур, но основная разработка ведется на GNU/Linux с помощью GCC, на MacOS X с помощью clang. Radare также компилируется на других системах и архитектурах, включая TCC и SunStudio.

Разработчики часто используют radare в качестве отладчика при обратном проектировании (реверс-инжениринге). Сейчас подсистему отладчика можно использовать в Windows, GNU/Linux (Intel x86, x86_64, MIPS и ARM), OS X, FreeBSD, NetBSD и OpenBSD (Intel x86 и x86_64).

По сравнению с ядром функция отладчика является ограниченно-переносимой. Если отладчик еще не портирован на вашу любимую платформу, можно отключить модуль отладки при помощи параметра --without-debugger скрипта `configure` при компиляции Radare2.

Напомним, что в программной системе есть плагины ввода-вывода, которые используют функции GDB, WinDbg или Wine, они полагаются на наличие соответствующих инструментов (в случае удаленной отладки — только на целевой машине).

Чтобы собрать программную систему, используя `acr` и `GNU Make`, например, в системах *BSD:
```
$ ./configure --prefix=/usr
$ gmake
$ sudo gmake install
```
Существует и простой скрипт, собирающий проект автоматически:
```
$ sys/install.sh
```
### Статическая сборка

Можно собрать radare2 статически вместе со всеми инструментами с помощью команды:
```
$ sys/static.sh
```
### Сборка при помощи Meson

Для сборки можно использовать meson + ninja:
```
$ sys/meson.py --prefix=/usr --shared --install
```
Для локальной сборки:
```
$ sys/meson.py --prefix=/home/$USER/r2meson --local --shared --install
```
### Установка в Docker

В репозитории исходного кода radare2 есть [Dockerfile](https://github.com/radareorg/radare2/blob/master/Dockerfile), используемый с системой Docker.

Этот dockerfile также используется в дистрибутиве Remnux из SANS, а также на сайте hub.docker.com, [registryhub](https://registry.hub.docker.com/u/remnux/radare2/).

## Деинсталляция старых установок Radare2
```
./configure --prefix=/old/r2/prefix/installation
make purge
```