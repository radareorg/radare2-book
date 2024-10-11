## Handmade Projects

When you need full control and complete flexibility over your project metadata, you can choose to create a set of scripts that manage your binary analysis. This allows you to:

- Load the binary.
- Set the base address.
- Configure the memory layout.
- Run specific analysis commands.
- Have full control on the steps

Note that having full control on the commands you run is important in a variety os sutations (read the section on challenges for more details).

As long as handmade projects are organized in directories and files it is ideal to use it with git, allowing other people to jump into the same files and have a proper versioned.

These scripts are essentially the entry points for your analysis. However, this approach comes with a trade-off: comments, flags, or function names won’t be saved automatically when exiting the session. Instead, you’ll need to manually type them into the scripts every time they are required.

This workflow demands consistency and discipline from the user. It’s crucial to maintain an organized system to ensure that important metadata isn’t lost, as nothing is auto-saved unless explicitly written into the project script by the user.

### Advantages and Best Practices

Handmade projects allow you to fully customize your setup. Since they are typically organized into directories and files, they are ideal for use with Git, which provides version control and enables collaboration.

### Folder Organization

To begin, create a directory to store the binaries you want to work on. Then, you can create a **Makefile** or a shell script to automate running commands:

```console
$ mkdir project/bins
$ cd project

$ cp /bin/ls bins/ls
$ echo r2 -s main -i script.r2 bins/ls > main.sh
$ chmod +x main.sh
```

Next, you can edit the `script.r2` file to include the commands you need:

```bash
aaa
CC good boy @ sym.success
```

Running the script is simple. Just execute `./main.sh` to open the project. You can set up custom memory maps, load contents from RAM, enable cache, patch instructions, and much more.

### Radare2 Primitives

Much of Radare2’s environment is built around shell commands. Many commands include a `*` suffix or subcommand that lists data in Radare2 commands.

For example, to save flags:

```console
> f* > flags.r2
```

And to reload them:

```console
> . flags.r2
```

You can apply this same principle to comments (`CC*`), function names (`afn*@@F`), and more.

### Default Script Behavior

If you save an `.r2` script in the same directory as the binary, Radare2 will prompt you to run it upon opening the binary. This can save time by eliminating the need to use the `-i` flag:

```bash
$ echo '?e hello world' > ls.r2
$ cp /bin/ls .
$ r2 ls
Do you want to run the 'ls.r2' script? (y/N)
```

### Advanced Customization

You can learn more about custom handmade projects in the firmware reversing chapter of this book, where you’ll find examples of setting up memory layouts and project configurations.

For users who enjoy customization, Radare2 allows you to modify the environment by tweaking variables such as `scr.prompt` for a custom command prompt. For example:

```bash
$ r2 -p jeje
 -- The more 'a' you add after 'aa' the more analysis steps are executed.
[0x00000000]> e scr.prompt.prj=true
<jeje>[0x00000000]> e prj.name
jeje
<jeje>[0x00000000]>
```

### Autosaving Options

Radare2 also provides a mechanism for users to define actions that are performed when leaving the shell. The `cmd.exit` configuration variable can be set to run specific commands or scripts when the session ends. For example, you could create a `.r2.js` script that saves all comments and function names before closing the session.

However, it’s important to remember that it’s up to the user to manage and manually update project scripts with new flags, analysis commands, or comments. While this approach requires more effort, it offers unmatched flexibility.

### Conclusion

The handmade project approach in Radare2 provides full control over your workflow but requires meticulous organization and manual updates. Over time, these processes will improve, and user contributions are always welcome, whether through feedback or pull requests.
