# Системные вызовы

Radare2 позволяет вручную искать ассемблерный код, похожий на операцию системного вызова.
Например, на платформе ARM они обычно представлены инструкциями `svc`, на других архитектурах могут быть другие варианты, например `системный вызов` на x86 ПК.
```
[0x0001ece0]> /ad/ svc
...
0x000187c2   # 2: svc 0x76
0x000189ea   # 2: svc 0xa9
0x00018a0e   # 2: svc 0x82
...
```
Обнаружение системных вызовов управляется `asm.os`, `asm.bits`, а также `asm.arch`. Роверьте соответствие настроек параметров конфигурации. Можно использовать команду `asl` для проверки правильности настроек поддержки системных вызовов.
Команда перечисляет системные вызовы, поддерживаемые вашей платформой.
```
[0x0001ece0]> asl
...
sd_softdevice_enable = 0x80.16
sd_softdevice_disable = 0x80.17
sd_softdevice_is_enabled = 0x80.18
...
```

Если настроен стек ESIL с помощью `aei` или `aeim`, можно использовать команду `/as` для поиска адресов детектированных системных вызовов, и перечислить их.
```
[0x0001ece0]> aei
[0x0001ece0]> /as
0x000187c2 sd_ble_gap_disconnect
0x000189ea sd_ble_gatts_sys_attr_set
0x00018a0e sd_ble_gap_sec_info_reply
...
```
Для сокращения времени поиска можно [ограничить диапазон поиска](../search_bytes/configurating_the_search.md) на только сегменты кода или сегьутеы с `/as @e:search.in=io.maps.x`.

Используя [эмуляцию ESIL](emulation.md) в radare2 можно печатать аргументы системного вызова в дизассемблировании. Чтобы включить линейную (но очень грубую) эмуляцию, используйте
переменную конфигурации `asm.emu`:
```
[0x0001ece0]> e asm.emu=true
[0x0001ece0]> s 0x000187c2
[0x000187c2]> pdf~svc
   0x000187c2   svc 0x76  ; 118 = sd_ble_gap_disconnect
[0x000187c2]>
```

В случае выполнения команды `аае` (или `аааа`, который вызывает `аае`) radare2 будет помещать найденные системные вызовы в специальное пространство флагов `syscall.`, что полезо для автоматизации:
```
[0x000187c2]> fs
0    0 * imports
1    0 * symbols
2 1523 * functions
3  420 * strings
4  183 * syscalls
[0x000187c2]> f~syscall
...
0x000187c2 1 syscall.sd_ble_gap_disconnect.0
0x000189ea 1 syscall.sd_ble_gatts_sys_attr_set
0x00018a0e 1 syscall.sd_ble_gap_sec_info_reply
...
```

По нему также можно интерактивно перемещаться в режиме HUD (`V_`).
```
0> syscall.sd_ble_gap_disconnect
 - 0x000187b2  syscall.sd_ble_gap_disconnect
   0x000187c2  syscall.sd_ble_gap_disconnect.0
   0x00018a16  syscall.sd_ble_gap_disconnect.1
   0x00018b32  syscall.sd_ble_gap_disconnect.2
   0x0002ac36  syscall.sd_ble_gap_disconnect.3
```

При отладке в radare2 можно использовать `dcs` для продолжения выполнения до следующего системного вызова. Также можено запустить `dcs*` для отслеживания всех системных вызовов.
```
[0xf7fb9120]> dcs*
Running child until syscalls:-1
child stopped with signal 133
--> SN 0xf7fd3d5b syscall 45 brk (0xffffffda)
child stopped with signal 133
--> SN 0xf7fd28f3 syscall 384 arch_prctl (0xffffffda 0x3001)
child stopped with signal 133
--> SN 0xf7fc81b2 syscall 33 access (0xffffffda 0xf7fd8bf1)
child stopped with signal 133
```

В radare2 включена функция преобразования имени системного вызова в номер системного вызова. Можно получить имя системного вызова для заданного номера и наоборот.

```
[0x08048436]> asl 1
exit
[0x08048436]> asl write
4
[0x08048436]> ask write
0x80,4,3,iZi
```

Почитайте `as?` для получения дополнительной информации о функции.
