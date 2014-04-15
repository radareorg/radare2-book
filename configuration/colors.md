## Colors

The console access is wrapped by an API that permits to show the output of any command as ANSI, w32 console or HTML (more to come ncurses, pango, ...) this allows the core to be flexible enought to run on limited environments like kernels or embedded devices allowing us to get the feedback from the application in our favourite format.

To start, we'll enable the colors by default in our rc file:

    $ echo 'e scr.color=true' >> ~/.radare2rc
You can configure the colors to be used in almost every element in your disassembly.
r2 supports rgb colors in unix terminals and allows to change the console color paletes using the `ec` command.

Type `ec` to get a list of all the palete elements.
Type `ecs` to show a color palete to pick colors from:

![img](http://lolcathost.org/b/r2pal.png)

#### xvilka theme


    ec fname rgb:0cf
    ec label rgb:0f3
    ec math rgb:660
    ec bin rgb:f90
    ec call rgb:f00
    ec jmp rgb:03f
    ec cjmp rgb:33c
    ec offset rgb:366
    ec comment rgb:0cf
    ec push rgb:0c0
    ec pop rgb:0c0
    ec cmp rgb:060
    ec nop rgb:000
    ec b0x00 rgb:444
    ec b0x7f rgb:555
    ec b0xff rgb:666
    ec btext rgb:777
    ec other rgb:bbb
    ec num rgb:f03
    ec reg rgb:6f0
    ec fline rgb:fc0
    ec flow rgb:0f0

![img](http://xvilka.me/r2-rainbow.png)
