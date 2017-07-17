## Exports

Rabin2 is able to find Exports. An example:

    $ rabin2 -E RtmPal.dll |head
    [Exports]
    vaddr=0x100955c0 paddr=0x000949c0 ord=000 fwd=NONE sz=0 bind=GLOBAL type=FUNC name=RtmPal.dll_??$check_version@$0BC@$0BG@@priv@spl_v18@@YAHXZ
    vaddr=0x100955c0 paddr=0x000949c0 ord=001 fwd=NONE sz=0 bind=GLOBAL type=FUNC name=RtmPal.dll_??$check_version@$0BC@$0BH@@priv@spl_v18@@YAHXZ
    vaddr=0x100955c0 paddr=0x000949c0 ord=002 fwd=NONE sz=0 bind=GLOBAL type=FUNC name=RtmPal.dll_??$check_version@$0BC@$0BI@@priv@spl_v18@@YAHXZ
    vaddr=0x100955c0 paddr=0x000949c0 ord=003 fwd=NONE sz=0 bind=GLOBAL type=FUNC name=RtmPal.dll_??$check_version@$0BC@$0BJ@@priv@spl_v18@@YAHXZ
    (...)
