## Insert in Visual Mode

Remember that, to be able to actually edit files loaded in radare2, you have to start it with `-w` option. Otherwise a file is opened in read-only mode.

The cursor mode allows you to manipulate with data at nibble-level (4 bits chunks), like it is done in most of hexadecimal editors. Press `TAB` to switch between hexadecimal and ASCII columns of the hexadecimal dump view.
After you press `i` key, you are prompted for an hexpair string. If `a` is pressed, enter an assembler expression, which will be translated to machine code and then inseters at the chosen offset.
