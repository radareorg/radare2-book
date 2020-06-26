## Colors

Console access is wrapped in API that permits to show the output of any command as ANSI, W32 Console or HTML formats. This allows radare's core to run inside environments with limited displaying capabilities, like kernels or embedded devices. It is still possible to receive data from it in your favorite format.

To enable colors support by default, add a corresponding configuration option to the .radare2 configuration file:
```
$ echo 'e scr.color=1' >> ~/.radare2rc
```
Note that enabling colors is not a boolean option. Instead, it is a number because there are different color depth levels. This is:

* 0: black and white
* 1: 16 basic ANSI colors
* 2: 256 scale colors
* 3: 24bit true color

The reason for having such user-defined options is because there's no standard or portable way for the terminal programs to query the console to determine the best configuration, same goes for charset encodings, so r2 allows you to choose that by hand.

Usually, serial consoles may work with 0 or 1, while xterms may support up to 3. RCons will try to find the closest color scheme for your theme when you choose a different them with the `eco` command.

It is possible to configure the color of almost any element of disassembly output. For *NIX terminals, r2 accepts color specification in RGB format. To change the console color palette use `ec` command.

Type `ec` to get a list of all currently used colors. Type `ecs` to show a color palette to pick colors from:

![img](r2pal.png)

## Themes

You can create your own color theme, but radare2 have its own predefined ones. Use the `eco` command to list or select them.

After selecting one, you can compare between the color scheme of the shell and the current theme by pressing Ctrl-Shift and then right arrow key for the toggle.

In visual mode use the `R` key to randomize colors or choose the next theme in the list.
