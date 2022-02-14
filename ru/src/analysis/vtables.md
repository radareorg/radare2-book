# Virtual Tables

There is a basic support of virtual tables parsing (RTTI and others).
The most important thing before you start to perform such kind of analysis
is to check if the `anal.cpp.abi` option is set correctly, and change if needed.

All commands to work with virtual tables are located in the `av` namespace.
Currently, the support is very basic, allowing you only to inspect
parsed tables.

```
|Usage: av[?jr*] C++ vtables and RTTI
| av           search for vtables in data sections and show results
| avj          like av, but as json
| av*          like av, but as r2 commands
| avr[j@addr]  try to parse RTTI at vtable addr (see anal.cpp.abi)
| avra[j]      search for vtables and try to parse RTTI at each of them
```

The main commands here are `av` and `avr`. `av` lists all virtual tables
found when r2 opened the file. If you are not happy with the result
you may want to try to parse virtual table at a particular address with
`avr` command. `avra` performs the search and parsing of all virtual
tables in the binary, like r2 does during the file opening.
