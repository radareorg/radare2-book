# r2wars

r2wars is a game where two programs, called bots or warriors, run in a shared memory space. The goal is to make the opponent's program crash or survive until it crashes on its own. It's similar to the classic game CoreWars, but implemented using radare2 technologies.

The game was originally implemented by pancake as a toy r2pipe.py script in 2016. It served as a way to identify bugs and improve the support for emulation and assembling instructions in radare2.

Years later the game was reimplemented in C# by SkUaTeR and used for the r2wars competitions during r2con. Check the links below:

* [r2wars for n00bs (captain banana)](https://www.youtube.com/watch?v=PB0AFBqFwGQ)
* [Competing at r2con](https://www.youtube.com/watch?v=i61v8Uvxhqk)
* [r2wars github repository](https://github.com/radareorg/r2wars)
* [Writeup from 2019](https://anisse.astier.eu/r2wars-2019.html)

## How It Works

The game takes place in a virtual memory space of 1024 bytes (from address 0 to 1023). Bots are written in assembly language for various architectures. They take turns executing one instruction at a time. A bot loses if it tries to access an invalid memory address (outside the 0-1023 range) or executes an invalid instruction. Battles also have a 4000 cycle limit to prevent endless rounds.

In order to bypass `rep` tricks and make the game more competitive when mixing different CPU architectures bot turns switch depending on cycles, instead of steps. This is, memory operations are slower than register ones, branches also have a cost, etc. You must have all those restrictions in mind when developing your bots in order to win.

r2wars supports multiple architectures thanks to ESIL (Evaluated Strings Intermediate Language) which translates opcodes from supported architectures a generic form that can be executed in a portable way. Supported architectures by r2wars include x86, ARM and MIPS, but technically, any other architecture supported by r2 is valid.

## Common Techniques

Successful r2wars bots often use these strategies:

* Self-location: Bots need to know where they are in memory to avoid overwriting themselves.
* Scanning: Search for the opponent's code in memory.
* Bombing: Write invalid opcodes or jump instructions to disrupt the opponent.
* Replication: Copy the bot's code to other memory locations.
* Decoys: Write fake code to mislead opponents.
* Register storage: Store the bot's payload in registers, then push to memory and execute.

## Writing your first bot

Here's a simple x86-32 bot that writes invalid opcodes across the memory space starting from the initial program counter address.

```asm
call tag
tag:
pop esp
loop:
add esp, 16
push 0xffffffff
jmp loop
```

This bot uses the call/pop trick to find its own location in memory, then moves through memory in 16-byte increments, writes 0xffffffff (likely invalid opcodes) as it loops until it crashes by going out of bounds, hopefully
