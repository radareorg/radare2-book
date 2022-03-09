## Реализация плагина отладчика

* Добавление профиля регистров отладчика в shlr/gdb/src/core.c,
* Добавление поддержки профиля и архитектуры регистров в libr/debug/p/debug_native.c и libr/debug/p/debug_gdb.c,
* Добавьте код для профилей в функцию `r_debug_gdb_attach(RDebug *dbg, int pid)`.

Если добавить поддержку gdb, тогда можно будет видеть профиль регистров в активном сеансе gdb, используя команду `maint print registers`.

## То ли еще будет!

* Статья по теме: http://radare.today/posts/extending-r2-with-new-plugins/

Некоторые коммиты, связанные с «Реализацией новых архитектур»

* Extensa: https://github.com/radareorg/radare2/commit/6f1655c49160fe9a287020537afe0fb8049085d7,
* Malbolge: https://github.com/radareorg/radare2/pull/579,
* 6502: https://github.com/radareorg/radare2/pull/656,
* h8300: https://github.com/radareorg/radare2/pull/664,
* GBA: https://github.com/radareorg/radare2/pull/702,
* CR16: https://github.com/radareorg/radare2/pull/721/ && 726,
* XCore: https://github.com/radareorg/radare2/commit/bb16d1737ca5a471142f16ccfa7d444d2713a54d,
* SharpLH5801: https://github.com/neuschaefer/radare2/commit/f4993cca634161ce6f82a64596fce45fe6b818e7,
* MSP430: https://github.com/radareorg/radare2/pull/1426,
* HP-PA-RISC: https://github.com/radareorg/radare2/commit/f8384feb6ba019b91229adb8fd6e0314b0656f7b,
* V810: https://github.com/radareorg/radare2/pull/2899,
* TMS320: https://github.com/radareorg/radare2/pull/596.

## Реализация новой псевдоархитектуры

Простой плагин для Z80, который можно использовать в качестве примера:

https://github.com/radareorg/radare2/commit/8ff6a92f65331cf8ad74cd0f44a60c258b137a06
