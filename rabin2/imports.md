##11.3 Imports

Rabin2 is able to get all the imported objects, as well as their offset at the PLT, this information is quite useful, for example, to recognize wich function is called by a call instruction.

    $ rabin2 -i /bin/ls |head
    [Imports]
    ordinal=001 plt=0x000021b0 bind=GLOBAL type=FUNC name=__ctype_toupper_loc
    ordinal=002 plt=0x000021c0 bind=GLOBAL type=FUNC name=__uflow
    ordinal=003 plt=0x000021d0 bind=GLOBAL type=FUNC name=getenv
    ordinal=004 plt=0x000021e0 bind=GLOBAL type=FUNC name=sigprocmask
    ordinal=005 plt=0x000021f0 bind=GLOBAL type=FUNC name=raise
    ordinal=006 plt=0x00002210 bind=GLOBAL type=FUNC name=localtime
    ordinal=007 plt=0x00002220 bind=GLOBAL type=FUNC name=__mempcpy_chk
    ordinal=008 plt=0x00002230 bind=GLOBAL type=FUNC name=abort
    ordinal=009 plt=0x00002240 bind=GLOBAL type=FUNC name=__errno_location
    (...)