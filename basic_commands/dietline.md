# Dietline

Radare2 comes with the lean [readline](https://en.wikipedia.org/wiki/GNU_Readline)-like input capability through the lean library to handle the
command edition and history navigation. It allows users to perform cursor movements, search the
history, and implements autocompletion. Moreover, due to the radare2 portability, dietline provides
the uniform experience among all supported platforms. It is used in all radare2 subshells - main
prompt, SDB shell, visual prompt, and offsets prompt. It also implements the most common features
and keybindings compatible with the GNU Readline.

Dietline supports two major configuration modes - Emacs-mode, and Vi-mode. It also supports the
famous `Ctrl-R` reverse history search. Using `TAB` key it allows to scroll through the
autocompletion options.

# Autocompletion

In the every shell and radare2 command autocompletion is supported. There are multiple modes of it -
files, flags, and SDB keys/namespaces.

# Emacs (default) mode

By default dietline mode is compatible with readline Emacs-like mode key bindings. Thus active are:
- `Ctrl-a` - move to the beginning of the line
- `Ctrl-e` - move to the end of the line
- `Alt-b` - move one word backward
- `Alt-f` - move one word forward
- `Alt-d` - cuts the word after the cursor
- `Ctrl-w` - delete the last word
- `Ctrl-b` - move one character backward
- `Ctrl-f` - move one character forward
- `Ctrl-u` - delete the whole line
- `Ctrl-r` - the reverse search in the command history
- `Ctrl-h` - delete a character to the left
- `Ctrl-d` - delete a character to the right



