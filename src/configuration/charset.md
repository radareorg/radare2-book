## Charset

Sometimes, a binary can contain custom characters in another charsets than ascii. It is as example the case of the gameboy. The gameboy has his own custom charset for each game. To select as example a custom charset corresponding to the game pokemon red and blue of the gameboy, do:

```
e cfg.charset=pokered;
```

Sadly, as you can imagine, very often the charset will be missing. There can not be as many plugin as there are games for gameboy. If the charset does not exists yet, fell free to write it. Create a file `yourplugin.sdb.txt`, then write:
```
0x00=a
0x01=b
```
to write a plugin that translate 00 as a and 01 as b.

To load a charset from your computer do:
```
e dir.charset=.;
e cfg.charset=yourplugin;
```


You can now do commands such as `ps` and `w` with custom charset.



It works of course for each architecture. Not only the gameboy.




In the future we plan to allow people to use visual mode with the custom charset.
