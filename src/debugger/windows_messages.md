# Windows Messages

On Windows, you can use `dbW` while debugging to set a breakpoint for the message handler of a specific window.

Get a list of the current process windows with  `dW` :

```
[0x7ffe885c1164]> dW
.----------------------------------------------------.
| Handle      | PID   | TID    | Class Name          |
)----------------------------------------------------(
| 0x0023038e  | 9432  | 22432  | MSCTFIME UI         |
| 0x0029049e  | 9432  | 22432  | IME                 |
| 0x002c048a  | 9432  | 22432  | Edit                |
| 0x000d0474  | 9432  | 22432  | msctls_statusbar32  |
| 0x00070bd6  | 9432  | 22432  | Notepad             |
`----------------------------------------------------'
```

Set the breakpoint with a message type, together with either the window class name or its handle:

```
[0x7ffe885c1164]> dbW WM_KEYDOWN Edit
Breakpoint set.
```

Or

``` 
[0x7ffe885c1164]> dbW WM_KEYDOWN 0x002c048a
Breakpoint set.
```

If you aren't sure which window you should put a breakpoint on, use `dWi` to identify it with your mouse:

```
[0x7ffe885c1164]> dWi
Move cursor to the window to be identified. Ready? y
Try to get the child? y
.--------------------------------------------.
| Handle      | PID   | TID    | Class Name  |
)--------------------------------------------(
| 0x002c048a  | 9432  | 22432  | Edit        |
`--------------------------------------------'
```
