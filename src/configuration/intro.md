# Configuration

The core reads `~/.config/radare2/radare2rc` while starting. You can add `e` commands to this file to tune the radare2 configuration to your taste.

To prevent radare2 from parsing this file at startup, pass it the `-N` option.

All the configuration of radare2 is done with the `eval` commands. A typical startup configuration file looks like this:
```sh
$ cat ~/.radare2rc
e scr.color = 1
e dbg.bep   = loader
```
The configuration can also be changed with `-e` <config=value> command-line option. This way you can adjust configuration from the command line, keeping the .radare2rc file intact. For example, to start with empty configuration and then adjust `scr.color` and `asm.syntax` the following line may be used:
```sh
$ radare2 -N -e scr.color=1 -e asm.syntax=intel -d /bin/ls
```
Internally, the configuration is stored in a hash table. The variables are grouped in namespaces: `cfg.`, `file.`, `dbg.`, `scr.` and so on.

To get a list of all configuration variables just type `e` in the command line
prompt. To limit the output to a selected namespace, pass it with an ending dot to `e`. For example, `e file.` will display all variables defined inside the "file" namespace.

To get help about `e` command type `e?`:

```
Usage: e [var[=value]]  Evaluable vars
| e?asm.bytes     show description
| e??             list config vars with description
| e a             get value of var 'a'
| e a=b           set var 'a' the 'b' value
| e var=?         print all valid values of var
| e var=??        print all valid values of var with description
| e.a=b           same as 'e a=b' but without using a space
| e,k=v,k=v,k=v   comma separated k[=v]
| e-              reset config vars
| e*              dump config vars in r commands
| e!a             invert the boolean value of 'a' var
| ec [k] [color]  set color for given key (prompt, offset, ...)
| eevar           open editor to change the value of var
| ed              open editor to change the ~/.radare2rc
| ej              list config vars in JSON
| env [k[=v]]     get/set environment variable
| er [key]        set config key as readonly. no way back
| es [space]      list all eval spaces [or keys]
| et [key]        show type of given config variable
| ev [key]        list config vars in verbose format
| evj [key]       list config vars in verbose format in JSON
```

A simpler alternative to the `e` command is accessible from the visual mode. Type `Ve` to enter it, use arrows (up, down, left, right) to navigate the configuration, and `q` to exit it. The start screen for the visual configuration edit looks like this:

```
[EvalSpace]

    >  anal
       asm
       scr
       asm
       bin
       cfg
       diff
       dir
       dbg
       cmd
       fs
       hex
       http
       graph
       hud
       scr
       search
       io
```

For configuration values that can take one of several values, you can use the `=?` operator to get a list
of valid values:

```
[0x00000000]> e scr.nkey = ?
scr.nkey = fun, hit, flag
```
