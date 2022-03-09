# Реализация пакета r2pm для плагина

Напомним, что в radare2 включен собственный [менеджер пакетов](../tools/r2pm/intro.md),  можно легко добавлять новые плагины, чтобы они были доступны всем.

Все пакеты расположены в репозитории [radare2-pm](https://github.com/radareorg/radare2-pm). Для описания структуры плагина используется очень простой текстовый формат.

```
R2PM_BEGIN

R2PM_GIT "https://github.com/user/mycpu"
R2PM_DESC "[r2-arch] дизассемблер и анализатор для MYCPU, плагин"

R2PM_INSTALL() {
	${MAKE} clean
	${MAKE} all || exit 1
	${MAKE} install R2PM_PLUGDIR="${R2PM_PLUGDIR}"
}

R2PM_UNINSTALL() {
	rm -f "${R2PM_PLUGDIR}/asm_mycpu."*
	rm -f "${R2PM_PLUGDIR}/anal_mycpu."*
}

R2PM_END
```

Затем добавьте его в каталог `/db` репозитория radare2-pm и отправьте pull-запрос.

