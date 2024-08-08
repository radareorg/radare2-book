# Scripting

Scripting is a big part of using radare2. It's really important to get good at using radare2 commands. The better you know these commands, the more you can do with the tool. You'll be able to work faster and figure out tougher problems.

Spending time to learn the commands will pay off in the long run. But commands are just a portion of the capabilities of the shell, these can be modifier, combined or processed when used with some special characters similar to the posix shell, like pipes, filters, redirections, etc

r2pipe is the main way to use radare2 from other programming languages. It lets you control radare2 from languages like Python, Javascript or even Rust. This is great for making your own tools that work with radare2. You can write scripts to do things automatically, which saves a lot of time. r2pipe opens up a lot of possibilities for using radare2 in new ways.

radare2 also has some built-in ways to run scripts. There's r2js, which lets you run JavaScript right inside radare2. This is useful for quick scripts (and plugins) when you don't want to use external dependencies.

There's also rlang, which lets you use radare2's inner workings from different programming languages. These features help you customize radare2 and make it do exactly what you need. You can extend radare2's abilities and create your own add-ons.

## Shell

As mentioned before many commands can be executed in sequence by using `;` the semicolon operator.

```console
[0x00404800]> pd 1 ; ao 1
 0x00404800  b827e66100   mov eax, 0x61e627  ; "tab"
address: 0x404800
opcode: mov eax, 0x61e627
prefix: 0
bytes: b827e66100
ptr: 0x0061e627
refptr: 0
size: 5
type: mov
esil: 6415911,rax,=
stack: null
family: cpu
[0x00404800]>
```

It simply runs the second command after finishing the first one, like in a posix shell.

The second important way to sequence the commands is with a simple pipe `|`:

```console
ao | grep address
```

Note, the `|` pipe only can pipe output of r2 commands to external (shell) commands, like system programs or builtin shell commands.

There is a similar way to sequence r2 commands, using the backtick operator `` `command` ``. The quoted part will undergo command substitution and the output will be used as an argument of the command line.

For example, we want to see a few bytes of the memory at the address referred to by the 'mov eax, addr' instruction. We can do that without jumping to it, using a sequence of commands:

```console
[0x00404800]> pd 1
 0x00404800  b827e66100   mov eax, 0x61e627 ; "tab"
[0x00404800]> ao
address: 0x404800
opcode: mov eax, 0x61e627
prefix: 0
bytes: b827e66100
ptr: 0x0061e627
refptr: 0
size: 5
type: mov
esil: 6415911,rax,=
stack: null
family: cpu
[0x00404800]> ao~ptr[1]
0x0061e627
0
[0x00404800]> px 10 @ `ao~ptr[1]`
- offset -   0 1  2 3  4 5  6 7  8 9  A B  C D  E F  0123456789ABCDEF
0x0061e627  7461 6200 2e69 6e74 6572                 tab..inter
[0x00404800]>
```

And of course it's possible to redirect the output of an r2 command into a file, using the `>` and `>>`
commands

```console
[0x00404800]> px 10 @ `ao~ptr[1]` > example.txt
[0x00404800]> px 10 @ `ao~ptr[1]` >> example.txt
```

Radare2 also provides quite a few Unix type file processing commands like head, tail, cat, grep and many more. One such command is [Uniq](https://en.wikipedia.org/wiki/Uniq), which can be used to filter a file to display only non-duplicate content. So to make a new file with only unique strings, you can do:

```console
[0x00404800]> uniq file > uniq_file
```

Other than stdout, you can specify other file descriptors to be redirected like in the posix shell:

```console
[0x00404800]> aaa 2> /dev/null
```

The [head](https://en.wikipedia.org/wiki/Head_%28Unix%29) command can be used to see the first N number of lines in the file, similarly [tail](https://en.wikipedia.org/wiki/Tail_(Unix)) command allows the last N number of lines to be seen.

```console
[0x00404800]> head 3 foodtypes.txt
Proteins
Fats
Carbohydrates
[0x00404800]> tail 2 foodtypes.txt
Probiotics
Water
```

Similarly, sorting the content is also possible with the [sort](https://en.wikipedia.org/wiki/Sort_%28Unix%29) command. A typical example could be:

```console
[0x00404800]> cat foods.txt
Lentils
Avocado
Brown rice
Chia seeds
Spinach
Almonds
Blueberries
Broccoli
Sauerkraut
Cucumber
[0x00404800]> sort foods.txt
Almonds
Avocado
Blueberries
Broccoli
Brown rice
Chia seeds
Cucumber
Lentils
Sauerkraut
Spinach
```

The `?$?` command describes several helpful variables you can use to do similar actions even more
easily, like the `$v` "immediate value" variable, or the `$m` opcode memory reference variable.
