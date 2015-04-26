## Visual cursor

Pressing lowercase `c` makes the cursor appear or disappear:

![Cursor at 0x00404896](cursor.png)


The cursor is used to select a range of bytes or just point to a byte to flag it (press `f` to create a new flag where the cursor points to)

If you select a range of bytes press `i` and then a byte array to overwrite the selected bytes with the ones you choose in a circular copy way. For example:

    <select 10 bytes in visual mode using upper hjkl>
    <press 'i' and then '12 34'>
    
The 10 bytes selected will become: 12 34 12 34 12 34 12 34 12 34
The byte range selection can be used together with the `d` key to change the data type of the selected bytes into a string, code or a byte array.

That's useful to enhace the disassembly, add metadata or just align the code if there are bytes mixed with code.

In cursor mode you can set the block size by simply moving it to the position you want and pressing `_`. Then change block size.
