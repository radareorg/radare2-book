## Pattern search

The search command allows you to throw repeated pattern searchs against the IO backend to be able to identify repeated sequences of bytes without specifying them. The only property to perform this search is to manually define the minimum length of these patterns.

Here`s an example:

    [0x00000000]> /p 10
The output of the command will show the different patterns found and how many times they are repeated.
