## Imports

Rabin2 is able to find imported objects by an executable, as well as their offsets in its PLT. This information is useful, for example, to understand what external function is invoked by `call` instruction. Pass `-i` flag to rabin2 to get a list of imports. An example:

```
$ rabin2 -i /bin/ls
[Imports]
   1 0x000032e0  GLOBAL    FUNC __ctype_toupper_loc
   2 0x000032f0  GLOBAL    FUNC getenv
   3 0x00003300  GLOBAL    FUNC sigprocmask
   4 0x00003310  GLOBAL    FUNC __snprintf_chk
   5 0x00003320  GLOBAL    FUNC raise
   6 0x00000000  GLOBAL    FUNC free
   7 0x00003330  GLOBAL    FUNC abort
   8 0x00003340  GLOBAL    FUNC __errno_location
   9 0x00003350  GLOBAL    FUNC strncmp
  10 0x00000000    WEAK  NOTYPE _ITM_deregisterTMCloneTable
  11 0x00003360  GLOBAL    FUNC localtime_r
  12 0x00003370  GLOBAL    FUNC _exit
  13 0x00003380  GLOBAL    FUNC strcpy
  14 0x00003390  GLOBAL    FUNC __fpending
  15 0x000033a0  GLOBAL    FUNC isatty
  16 0x000033b0  GLOBAL    FUNC sigaction
  17 0x000033c0  GLOBAL    FUNC iswcntrl
  18 0x000033d0  GLOBAL    FUNC wcswidth
  19 0x000033e0  GLOBAL    FUNC localeconv
  20 0x000033f0  GLOBAL    FUNC mbstowcs
  21 0x00003400  GLOBAL    FUNC readlink
...

```

