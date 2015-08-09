## Visual Xrefs

radare2 implements many user-friendly features, helping to walk through target assembly code, in visual mode. One of them is accessible by pressing the `x` key. A popup menu appears with a list of cross-references (a.k.a xref), either data or code, defined for current seek position. After pressing a number key from this menu list, you will be transfered to a correspoding offset for that xref in the file.
Suppose you have moved your cursor to a position in code where the following list of xrefs is defined:

    |  ....--> ; CODE (CALL) XREF from 0x00402b98 (fcn.004028d0)
    |  ....--> ; CODE (CALL) XREF from 0x00402ba0 (fcn.004028d0)
    |  ....--> ; CODE (CALL) XREF from 0x00402ba9 (fcn.004028d0)
    |  ....--> ; CODE (CALL) XREF from 0x00402bd5 (fcn.004028d0)
    |  ....--> ; CODE (CALL) XREF from 0x00402beb (fcn.004028d0)
    |  ....--> ; CODE (CALL) XREF from 0x00402c25 (fcn.004028d0)
    |  ....--> ; CODE (CALL) XREF from 0x00402c31 (fcn.004028d0)
    |  ....--> ; CODE (CALL) XREF from 0x00402c40 (fcn.004028d0)
    |  ....--> ; CODE (CALL) XREF from 0x00402c51 (fcn.004028d0)
    
After pressing `x` you will see a menu listing the same xrefs along with numbers in brackets:

    [GOTO XREF]>
    [0] CODE (CALL) XREF 0x00402b98 (loc.00402b38)
    [1] CODE (CALL) XREF 0x00402ba0 (loc.00402b38)
    [2] CODE (CALL) XREF 0x00402ba9 (loc.00402b38)
    [3] CODE (CALL) XREF 0x00402bd5 (loc.00402b38)
    [4] CODE (CALL) XREF 0x00402beb (loc.00402b38)
    [5] CODE (CALL) XREF 0x00402c25 (loc.00402b38)
    [6] CODE (CALL) XREF 0x00402c31 (loc.00402b38)
    [7] CODE (CALL) XREF 0x00402c40 (loc.00402b38)
    [8] CODE (CALL) XREF 0x00402c51 (loc.00402b38)
    [9] CODE (CALL) XREF 0x00402c60 (loc.00402b38)

By pressing a number key from `0` to `9`, you can choose to move to a new position in the file assosiated with that corss-reference.

The history of seek positions is saved. You can always get back to a previous view in the file by pressing `u` key.
