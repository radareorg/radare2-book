## ESIL Pins

ESIL pins are a powerful feature in radare2 that let you hook into the ESIL (Evaluable Strings Intermediate Language) emulation process. Instead of executing the standard ESIL expressions at specific addresses, you can run custom radare2 commands. This is particularly useful for implementing complex behaviors that ESIL alone can't handle, such as simulating system calls, logging execution, or faking function returns.

When the ESIL virtual machine reaches a pinned address, it executes the associated commands instead of the original ESIL code. Pins are essentially named hooks tied to memory addresses.

### Configuration

The `cmd.esil.pin` configuration variable defines the default command that runs whenever any pin is triggered. For example, setting it to `?e` will echo the pin name when hit. Individual pins can override this default with their own commands.

The commands executed can change the value of any register, memory state or even execute system programs.

To set a pin, use the `aep` command followed by the pin name and address:

```console
$ r2 -a arm -b64 -
[0x00000000]> e cmd.esil.pin=?e im a pin
[0x00000000]> aep mypin=?e my pin
[0x00000000]> aep mypin @ 0
[0x00000000]> aep fallback @ 4
[0x00000000]> aes
my pin
[0x00000000]> aes
im a pin
[0x00000000]>
```

Note that the first step over the address 0 prints "my pin" because the pin name have a command associated, the second step at address 4 prints 'im a pin' because ther'es no esil pin named "fallback" so it runs the command defined in the cmd.esil.pin variable.

## Soft and Hard Pins

ESIL pins can be categorized into soft and hard pins based on their naming prefix and execution context.

### Soft Pins

Soft pins are prefixed with `soft.` (e.g., `soft.ret0`). They are designed for use during disassembly and analysis, particularly in `asm.emu` mode, where they provide inline annotations and hints in the disassembly listing. Soft pins execute during disassembly loops to enhance readability without affecting the emulation state.

In the radare2 source code (`libr/anal/pin.c`), soft pins are explicitly skipped during ESIL emulation stepping to prevent interference with the simulation process.

### Hard Pins

Hard pins do not have the `soft.` prefix (e.g., `ret0`, `memcpy`). They are executed during full ESIL emulation, such as when using the `aes` command. Hard pins can modify registers, memory, or execution flow, making them suitable for simulating complex behaviors like system calls or function returns.

The distinction ensures that soft pins offer disassembly-time annotations without emulation interference, while hard pins enable dynamic simulation during stepping.

### Default ESIL Pins

radare2 comes with several built-in pins for common operations:

```console
[0x00000000]> aep
"aep ret0=dr R0=0;aexa ret"
"aep memcpy=wf `dr?A1` `dr?A2` @ `dr?A0`;aexa ret"
"aep strlen=dr R0=`pszl@r:A0`;aexa ret"
"aep puts=psz@r:A0; aexa ret"
[0x00000000]>
```

These provide basic implementations for return values, memory copying, string length calculation, and printing.


### Practical Examples

Let's explore some real-world scenarios where ESIL pins shine.

#### Simulating Function Returns

Suppose you're emulating a function that should return 0. Instead of letting ESIL handle the return logic, you can pin a command to set the return value and continue emulation:

```console
[0x00000000]> aep ret0=dr R0=0;aexa ret
```

This pin sets register R0 to 0 and continues execution after the return.

#### Memory Operations

For functions like `memcpy`, you might want to simulate the memory copy without executing the actual ESIL:

```console
[0x00000000]> aep memcpy=wf `dr?A1` `dr?A2` @ `dr?A0`;aexa ret
```

This writes data from one location to another and then returns.

#### String Operations

Pins can help with string functions. For `strlen`, calculate the string length and set the return value:

```console
[0x00000000]> aep strlen=dr R0=`pszl@r:A0`;aexa ret
```

For `puts`, print the string and return:

```console
[0x00000000]> aep puts=psz@r:A0; aexa ret
```

#### Logging Traces in Emulation

Use pins to log when certain addresses are reached:

```console
[0x00000000]> aep log=?e Reached interesting function at $$
```

This will print a message whenever the pin is hit, showing the current program counter.

#### Conditional Execution

You can create pins that only trigger under certain conditions by combining with ESIL expressions or r2 commands.

### Managing Pins

To list all pins:

```console
[0x00000000]> aep
```

To remove a specific pin:

```console
[0x00000000]> aep- 0x1000
```

Or remove all pins:

```console
[0x00000000]> aep-*
```

Check if there's a pin at the current address:

```console
[0x00000000]> aep.
```

Pins appear in disassembly as `[aep: name]`, making it easy to spot where they've been set.
