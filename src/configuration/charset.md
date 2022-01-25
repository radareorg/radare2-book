## Charset

Sometimes, a binary can contain custom characters in another charsets than ascii. It is as example the case of the gameboy. The gameboy has his own custom charset for each game. To select as example a custom charset corresponding to the game pokemon red and blue of the gameboy, do:

```
e cfg.charset=pokered;
```

You can now do commands such as `ps` and `w` with custom charset.

Sadly, as you can imagine, very often the charset will be missing. There can not be as many plugin as there are games for gameboy. If the charset does not exists yet, fell free to write it. See the chapters `plugins` and then `charset``to read about charsets.

It works of course for each architecture. Not only the gameboy.

In the future we plan to allow people to use visual mode with the custom charset.
