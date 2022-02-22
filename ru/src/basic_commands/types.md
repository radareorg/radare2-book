## Работа с типами данных

Radare2 также может работать с типами данных. Можно использовать стандартные типы данных C или определять собственные с помощью C. В настоящее время существует поддержка структур, юнионов, сигнатур функций и перечислений.

```
[0x00000000]> t?
Usage: t   # cparse types commands
| t                          List all loaded types
| tj                         List all loaded types as json
| t <type>                   Show type in 'pf' syntax
| t*                         List types info in r2 commands
| t- <name>                  Delete types by its name
| t-*                        Remove all types
| tail [filename]            Output the last part of files
| tc [type.name]             List all/given types in C output format
| te[?]                      List all loaded enums
| td[?] <string>             Load types from string
| tf                         List all loaded functions signatures
| tk <sdb-query>             Perform sdb query
| tl[?]                      Show/Link type to an address
| tn[?] [-][addr]            manage noreturn function attributes and marks
| to -                       Open cfg.editor to load types
| to <path>                  Load types from C header file
| toe [type.name]            Open cfg.editor to edit types
| tos <path>                 Load types from parsed Sdb database
| tp  <type> [addr|varname]  cast data at <address> to <type> and print it (XXX: type can contain spaces)
| tpv <type> @ [value]       Show offset formatted for given type
| tpx <type> <hexpairs>      Show value for type with specified byte sequence (XXX: type can contain spaces)
| ts[?]                      Print loaded struct types
| tu[?]                      Print loaded union types
| tx[f?]                     Type xrefs
| tt[?]                      List all loaded typedefs
```

### Определение новых типов

Существует три различных метода определения новых типов:

1. Определение нового типа из оболочки r2 - используйте команду `td`, а всю строку возмите в двойные кавычками. Например:

`"td struct person {int age; char *name; char *address;};"`

2. Также можно использовать `to -`, чтобы открыть текстовый редактор для представления в нем требуемых типов. Этот подход удобен, когда у вас слишком много типов.

3. Radare2 также поддерживает загрузку заголовочных файлов с помощью команды `to`, путь к файлу заголовка - параметр команды.

Можно просматривать загруженные типы в r2, используя `ts` для структур, `tu` для юнионов, `tf` для сигнатур функций, `te` для перечислений.

Также можно преобразовывать (cast) указатели в типы данных и затем просматривать данные - команда `tp`. Пример:

```
[0x00400511]> tp person = 0x7fff170a46b0
       age : 0x7fff170a46b0 = 20
       name : (*0x4005b0) 0x7fff170a46b4 = My name
       address : (*0x4005b8) 0x7fff170a46bc = My age
[0x00400511]>
```

