# Dietline

Radare2 comes with the lean [readline](https://en.wikipedia.org/wiki/GNU_Readline)-like input capability through the lean library to handle the command edition and history navigation. It allows users to perform cursor movements, search the
history, and implements autocompletion. Moreover, due to the radare2 portability, dietline provides
the uniform experience among all supported platforms. It is used in all radare2 subshells - main
prompt, SDB shell, visual prompt, and offsets prompt. It also implements the most common features
and keybindings compatible with the GNU Readline.

Dietline supports two major configuration modes : Emacs-mode and Vi-mode. 

It also supports the famous `Ctrl-R` reverse history search. Using `TAB` key it allows to scroll through the
autocompletion options.

# Autocompletion

In the every shell and radare2 command autocompletion is supported. There are multiple modes of it -
files, flags, and SDB keys/namespaces. To provide the easy way to select possible completion options
the scrollable popup widget is available. It can be enabled with `scr.prompt.popup`, just set it to
the `1`.

# Emacs (default) mode

By default dietline mode is compatible with readline Emacs-like mode key bindings. Thus active are:

## Moving 
- `Ctrl-a` - move to the beginning of the line
- `Ctrl-e` - move to the end of the line
- `Ctrl-b` - move one character backward
- `Ctrl-f` - move one character forward

## Deleting
- `Ctrl-w` - delete the previous word
- `Ctrl-u` - delete the whole line
- `Ctrl-h` - delete a character to the left
- `Ctrl-d` - delete a character to the right
- `Alt-d` - cuts the character after the cursor

## Killing and Yanking 
- `Ctrl-k` - kill the text from point to the end of the line.
- `Ctrl-x` - kill backward from the cursor to the beginning of the current line.
- `Ctrl-t` - kill from point to the end of the current word, or if between words, to the end of the next word. Word boundaries are the same as forward-word.
- `Ctrl-w` - kill the word behind point, using white space as a word boundary. The killed text is saved on the kill-ring.
- `Ctrl-y` - yank the top of the kill ring into the buffer at point.
- `Ctrl-]` - rotate the kill-ring, and yank the new top. You can only do this if the prior command is yank or yank-pop.

## History
- `Ctrl-r` - the reverse search in the command history

# Vi mode

Radare2 also comes with in vi mode that can be enabled by toggling `scr.prompt.vi`. The various keybindings available in this mode are:

## Entering command modes
- `ESC` - enter into the control mode
- `i` - enter into the insert mode

## Moving
- `j` - acts like up arrow key
- `k` - acts like down arrow key
- `a` - move cursor forward and enter into insert mode
- `I` - move to the beginning of the line and enter into insert mode
- `A` - move to the end of the line and enter into insert mode
- `^` - move to the beginning of the line
- `0` - move to the beginning of the line
- `$` - move to the end of the line
- `h` - move one character backward
- `l` - move one character forward

## Deleting and Yanking
- `x` - cuts the character
- `dw` - delete the current word
- `diw` - deletes the current word.
- `db` - delete the previous word
- `D` - delete the whole line
- `dh` - delete a character to the left
- `dl` - delete a character to the right
- `d$` - kill the text from point to the end of the line.
- `d^` - kill backward from the cursor to the beginning of the current line.
- `de` - kill from point to the end of the current word, or if between words, to the end of the next word. Word boundaries are the same as forward-word.
- `p` - yank the top of the kill ring into the buffer at point.
- `c` - acts similar to d based commands, but goes into insert mode in the end by prefixing the commands with numbers, the command is performed multiple times.

If you are finding it hard to keep track of which mode you are in, just set `scr.prompt.mode=true` to update the color of the prompt based on the vi-mode.
