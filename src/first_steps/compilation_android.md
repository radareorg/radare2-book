## Android

Программа Radare2 также может быть кросс-компилирована для других архитектур и операционных систем, например в Android.

### Необходимые инструменты

* Python 3,
* Meson,
* Ninja,
* Git,
* Android NDK.

### Последовательность шагов

#### Загрузка и распаковка Android NDK

Загрузите Android NDK с [официального сайта](https://developer.android.com/ndk) и распакуйте его где-нибудь у вас в системе, например в директории `/tmp/android-ndk`.

#### Использование Make

##### Указание месторасположение NDK

```
$ echo NDK=/tmp/android-ndk  > ~/.r2androidrc
```

##### Компилирование и создание архива tar.gz, загрузка в подключенное устройство android

```
./sys/android-build.sh arm64-static
```

Можно собрать пакет для какой-либо другой архитектуры, изменив аргумент на
`./sys/android-build.sh`. Запустить скрипт без аргументов, он покажет использованные значения.

#### Использование Meson

##### Создание кросс-файл для мезона

Meson-у нужен конфигурационный файл, описывающий среду кросс-компиляции, например, `meson-android.ini`.
Его можно донастроить при необходимости. Следующий пример - хорошая отправная точка:
```
[binaries]
c       = '/tmp/android-ndk/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android28-clang'
cpp     = '/tmp/android-ndk/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android28-clang++'
ar      = '/tmp/android-ndk/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android-ar'
as      = '/tmp/android-ndk/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android-as'
ranlib  = '/tmp/android-ndk/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android-ranlib'
ld      = '/tmp/android-ndk/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android-ld'
strip   = '/tmp/android-ndk/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android-strip'
pkgconfig = 'false'

[properties]
sys_root = '/tmp/android-ndk/sysroot'

[host_machine]
system = 'android'
cpu_family = 'arm'
cpu = 'aarch64'
endian = 'little'
```

##### Компилирование при помощи meson + ninja

Настроить каталог сборки с meson-ом, как обычно:
```
$ CFLAGS="-static" LDFLAGS="-static" meson --default-library static --prefix=/tmp/android-dir -Dblob=true build --cross-file ./meson-android.ini
```

Поясним значения всех перечисленных выше переменных:
* `CFLAGS="-static"`, `LDFLAGS="-static"`, `--default-library static` обеспечивает
   компилирование библиотек и запускаемых файлов в статическом режиме, теперь нет необходимости указывать значения переменных LD_* в вашей среде Android-а. Переменные указывают, где находятся подходящие библиотеки. В статическом режиме они не нужны. У двоичных (запускаемых) файлов есть все, что им нужно внутри.
* `-Dblob=true` указывает meson-у компилировать только один единый запускаемый файл со всем требуемым кодом для всех программ `radare2`, `rabin2`, `rasm2`, и т.д. В результате для каждой программы будет создана символьная ссылка на этот единый файл. Это позволяет избежать создания множества больших статически скомпилированных двоичных файлов, а
    просто создается один единый, включающий все функции. Мы получим наши `rabin2`,
   `rasm2`, `rax2`, и др., но они будут только ссылками на `radare2`.
* `--cross-file ./meson-android.ini` описывает процесс компилирования radare2 для ОС Android

Затем, надо скомпилировать проект, и установить его на устройство:
```
$ ninja -C build
$ ninja -C build install
```

##### Копирование файлов на свое устройство Android, запуск приложения

Последний этап - копирование сгенерированных файлов в /tmp/android-dir на ваше Android-устройство, запуск на нем radare2:

```
$ cd /tmp && tar -cvf radare2-android.tar.gz android-dir
$ adb push radare2-android.tar.gz /data/local/tmp
$ adb shell
DEVICE:/ $ cd /data/local/tmp
DEVICE:/data/local/tmp $ tar xvf radare2-android.tar.gz
DEVICE:/data/local/tmp $ ./android-dir/bin/radare2
Usage: r2 [-ACdfLMnNqStuvwzX] [-P patch] [-p prj] [-a arch] [-b bits] [-i file]
          [-s addr] [-B baddr] [-m maddr] [-c cmd] [-e k=v] file|pid|-|--|=
```
