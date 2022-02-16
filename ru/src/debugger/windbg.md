# Отладка в режиме ядра с WinDBG KD

Поддержка интерфейса WinDBG KD в r2 позволяет подключаться к работающей виртуальной машине Windows и отлаживать ее ядро через последовательный порт или сеть.

Также можно использовать удаленный интерфейса GDB для подключения и
отладки ядрер Windows вне зависимости от инструментов Windows.

Имейте в виду, что поддержка WinDBG KD все еще находится в стадии разработки, сейчас это базовая реализация, которая со временем станет лучше.

## Настройка KD в Windows

> Пошаговое руководство см. в [документации корпорации Майкрософт](https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/setting-up-kernel-mode-debugging-in-windbg--cdb--or-ntsd).

### Последовательный порт
Подключение KD через последовательный порт в Windows Vista и выше следующим образом:

```
bcdedit /debug on
bcdedit /dbgsettings serial debugport:1 baudrate:115200
```

Так это делвется для Windows XP:
Открыть boot.ini и добавить /debug /debugport=COM1 /baudrate=115200:
```
[boot loader]
timeout=30
default=multi(0)disk(0)rdisk(0)partition(1)\WINDOWS
[operating systems]
multi(0)disk(0)rdisk(0)partition(1)\WINDOWS="Debugging with Cable" /fastdetect /debug /debugport=COM1 /baudrate=57600
```
В случае VMWare
```
	Virtual Machine Settings -> Add -> Serial Port
	Device Status:
	[v] Connect at power on
	Connection:
	[v] Use socket (named pipe)
	[_/tmp/winkd.pipe________]
	From: Server To: Virtual Machine
```
Настройка машины VirtualBox:
```
    Preferences -> Serial Ports -> Port 1

    [v] Enable Serial Port
    Port Number: [_COM1_______[v]]
    Port Mode:   [_Host_Pipe__[v]]
                 [v] Create Pipe
    Port/File Path: [_/tmp/winkd.pipe____]
```
Создание виртуальной машины при помощи qemu:
```
$ qemu-system-x86_64 -chardev socket,id=serial0,\
     path=/tmp/winkd.pipe,nowait,server \
     -serial chardev:serial0 -hda Windows7-VM.vdi
```

### Сеть
Подключение KD по сети (KDNet) в Windows 7 или более поздней версии выглядит следующим образом:
```
bcdedit /debug on
bcdedit /dbgsettings net hostip:w.x.y.z port:n
```
Начиная с Windows 8, принудительно выполнить отладку
в каждой загрузке невозможно, но всегда можно входить в расширенные параметры загрузки и там включать отладку ядра:
```
bcedit /set {globalsettings} advancedoptions true
```

## Подключение к интерфейсу KD на r2

### Последовательный порт
Radare2 использует плагин ввода-вывода `winkd` для подключения к файлу сокета, создаваемому virtualbox-ом или qemu. Для плагина отладчика `winkd` нужно также указать x86-32. (поддерживаются 32- и 64-разрядная отладка)
```
$ r2 -a x86 -b 32 -D winkd winkd:///tmp/winkd.pipe
```

В Windows надо запустить следующую строку:
```
$ radare2 -D winkd winkd://\\.\pipe\com_1
```

### Сеть
```
$ r2 -a x86 -b 32 -d winkd://<hostip>:<port>:w.x.y.z
```

## Использование KD
При подключении к интерфейсу KD r2 отправит пакет разрыва для прерывания
выполнения целевого процесса:
```
[0x828997b8]> pd 20
	;-- eip:
	0x828997b8    cc           int3
	0x828997b9    c20400       ret 4
	0x828997bc    cc           int3
	0x828997bd    90           nop
	0x828997be    c3           ret
    0x828997bf    90           nop
```

Чтобы пропустить это прерывание, нужно изменить eip и запустить 'dc' дважды:
```
dr eip=eip+1
dc
dr eip=eip+1
dc
```
Теперь виртуальная машина Windows снова будет интерактивной. Нам нужно будет убить r2 и присоединить его еще раз, чтобы перейти к управлению ядром.

Команда `dp` используется для перечисления всех процессов, а `dpa` или `dp=` для присоединения к процессу. Будет отображены базовые адреса процессов в карте физической памяти.

# WinDBG бэкенд для Windows (DbgEng)

В Windows radare2 может использовать `DbgEng.dll` в качестве серверной части отладки, позволяя ему использовать возможности WinDBG, включая поддержку файлов дампа памяти, отладку режимов пользователя и ядра локально и удаленно.

Можно использовать библиотеки DLL отладки, включенные в Windows, или загрузить последние версии со [страницы загрузки](https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/debugger-download-tools) Майкрософт, что рекомендуется сделать.

> Использовать библиотеки DLL из папки приложения `WinDbg Preview` Microsoft Store напрямую **нельзя**, поскольку они не помечены как исполняемые для обычных пользователей.

> radare2 попытается сначала загрузить `dbgeng.dll` из директория, указанного в переменной среды `_NT_DEBUGGER_EXTENSION_PATH`, перед тем как использоваь путь поиска библиотек Windows по умолчанию.

## Использование плагина

Чтобы использовать плагин `windbg`, задавайте те же параметры командной строки, что и для `WinDBG` или `kd` (см. [документацию](https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/windbg-command-line-options) Microsoft), добавляя кавычки и экранируя "\"-ами при необходимости:

```
> r2 -d "windbg://-remote tcp:server=Server,port=Socket"
```
```
> r2 -d "windbg://MyProgram.exe \"my arg\""
```
```
> r2 -d "windbg://-k net:port=<n>,key=<MyKey>"
```
```
> r2 -d "windbg://-z MyDumpFile.dmp"
```

Затем можно выполнять отладку как обычно (см. команду `d?` ) или взаимодействовать с командной строкой на сервере непосредственно с помощью команды `=!`:

```
[0x7ffcac9fcea0]> dcu 0x0007ffc98f42190
Continue until 0x7ffc98f42190 using 1 bpsize
ModLoad: 00007ffc`ab6b0000 00007ffc`ab6e0000   C:\WINDOWS\System32\IMM32.DLL
Breakpoint 1 hit
hit breakpoint at: 0x7ffc98f42190

[0x7fffcf232190]> =!k4
Child-SP          RetAddr           Call Site
00000033`73b1f618 00007ff6`c67a861d r_main!r_main_radare2
00000033`73b1f620 00007ff6`c67d0019 radare2!main+0x8d
00000033`73b1f720 00007ff6`c67cfebe radare2!invoke_main+0x39
00000033`73b1f770 00007ff6`c67cfd7e radare2!__scrt_common_main_seh+0x12e
```
