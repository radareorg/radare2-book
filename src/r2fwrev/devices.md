## Peripherals

Buttons, screens, leds, serial ports and sensors are usually accessible through mapping their registers as part of the memory. Usually this memory can't be read or written directly because every single bit can cause a wait lock, infinite loops, undesired hardware changes, etc

The Intel X86 architecture used IO ports accessible through two instructions (IN/OUT) to communicate with hardware. But all the RISC machines end up using the memory unit to make them accessible by just using peek and poke operations.

This is we can find the clock timer in a dword around and we will have a different value each time we read from there. But writing in this dword won't do anything.

Use flags to name each of these addresses, and you may get xrefs to them when analysing the code.

### Loading SVD files

SVD stands for System View Description. It is a standardized XML-based file format that provides detailed descriptions of the registers, memory, and peripheral mappings of a microcontroller or a system-on-chip (SoC).

These descriptions are crucial for reverse engineering efforts, as they allow engineers to understand how firmware interacts with the hardware at a low level, facilitating the identification of key functionality, debugging, and modification of firmware.

By parsing SVD files, reverse engineers can automate the process of mapping firmware code to hardware registers, streamlining the reverse engineering process and improving accuracy.

The r2svd program takes one of these XML files and creates an r2 script that sets all these flags in the right addresses.

```console
$ head tc1767.pspec.r2 
f pin.SBCU_ID=0xF0000108
f pin.SBCU_CON=0xF0000110
f pin.SBCU_ECON=0xF0000120
f pin.SBCU_EADD=0xF0000124
f pin.SBCU_EDAT=0xF0000128
f pin.SBCU_DBCNTL=0xF0000130
f pin.SBCU_DBGRNT=0xF0000134
f pin.SBCU_DBADR1=0xF0000138
```

Note that we can extract the ranges of memory where the peripherals are located and we should create new maps to cover them.
