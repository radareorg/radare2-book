## Visual xrefs

radare implements many user-friendly features for the visual interface to walk thru the assembly code. One of them is the `x` key that popups a menu for selecting the xref (data or code) against the current seek and then jump there. 
For example when pressing x when looking at those XREF:


    |  ....--> ; CODE (CALL) XREF from 0x00402b98 (fcn.004028d0)
    |  ....--> ; CODE (CALL) XREF from 0x00402ba0 (fcn.004028d0)                  
    |  ....--> ; CODE (CALL) XREF from 0x00402ba9 (fcn.004028d0)       
    |  ....--> ; CODE (CALL) XREF from 0x00402bd5 (fcn.004028d0)        
    |  ....--> ; CODE (CALL) XREF from 0x00402beb (fcn.004028d0)   
    |  ....--> ; CODE (CALL) XREF from 0x00402c25 (fcn.004028d0)     
    |  ....--> ; CODE (CALL) XREF from 0x00402c31 (fcn.004028d0)              
    |  ....--> ; CODE (CALL) XREF from 0x00402c40 (fcn.004028d0)            
    |  ....--> ; CODE (CALL) XREF from 0x00402c51 (fcn.004028d0)
    
After pressing `x`

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

All the calls and jumps are numbered (1, 2, 3...) these numbers are the keybindings for seeking there from the visual mode.
All the seek history is stored, by pressing `u` key you will go back in the seek history time :)
