# Comparison Watchers

Watchers are used to record memory at 2 different points in time, then report
if and how it changed.

```
[0x00000000]> cw?
Usage: cw [args]  Manage compare watchers; See if and how memory changes
| cw??            Show more info about watchers
| cw addr sz cmd  Add a compare watcher
| cw[*qj] [addr]   Show compare watchers (*=r2 commands, q=quiet)
| cwd [addr]      Delete watcher
| cwr [addr]      Revert watcher
| cwu [addr]      Update watcher
```

## Basic watcher usage

First, create one with `cw addr sz cmd`. This will record `sz` bytes at `addr`.
The command is stored and used to print the memory when shown.

```
# Create a watcher at 0x0 of size 4 using p8 as the command
[0x00000000]> cw 0 4 p8
```

To record the second state, use `cwu`. Now, when you run `cw`, the watcher will
report if the bytes changed and run the command given at creation with the size
and address. When an address is an optional argument, the command will apply
to all watchers if you don't pass one.

```
# Introduce a change to the block of data we're watching
[0x00000000]> wx 11223344
# Update all watchers
[0x00000000]> cwu
# Show changes
[0x00000000]> cw
0x00000000 modified
11223344
```

You may overwrite any watcher by creating another at the same address. This
will discard the existing watcher completely.

```
# Overwrite our existing watcher to display a bistream instead of
# hexpairs, and make the watched area larger
[0x00000000]> cw 0 8 pB
# Check that it's no longer "modified" as this is a new watcher
[0x00000000]> cw
0x00000000
0001000100100010001100110100010000000000000000000000000000000000
```

## Reverting watcher state

When you create a watcher, the data read from memory is marked as "new".
Updating the watcher with `cwu` will mark this data as "old", and then read the
"new" data.

`cwr` will mark the current "old" state as being "new", letting you reuse it as
your new base state when updating with `cwu`. Any existing "new" state from
running `cwu` previously is lost in this process. Showing a watcher without
updating will still run the command, but it will not report changes.

```
# Create a basic watcher
[0x00000000]> cw 0 4 p8
[0x00000000]> cw
0x00000000
00000000
# Modify the memory and update the watcher
[0x00000000]> wx 11223344
[0x00000000]> cwu
# Watcher reports modification
# The "new" state is 11223344, and the "old" state is 00000000
[0x00000000]> cw
0x00000000 modified
11223344
# Revert the watcher
[0x00000000]> cwr
# The "new" state is 00000000 again, and there is no "old" state
# The watcher reports no change since it is no longer up-to-date
[0x00000000]> cw
0x00000000
11223344
```

## Overlapping watcher areas

Watched memory areas may overlap with no ill effects, but may have unexpected
results if you update some but not others.

```
# Create a watcher that watches 512 bytes starting at 0
[0x00000000]> cw 0 0x200 p8
# Create a watcher that watches 16 bytes starting at 0x100
[0x00000000]> cw 0x100 0x10 p8
# Modify memory watched by both watchers
[0x00000000]> wx 11223344 @ 0x100
# Watchers aren't updated, so they don't report a change
[0x00000000]> cw*
cw 0x00000000 512 p8
cw 0x00000100 16 p8
# Update only the watcher at 0x100
[0x00000000]> cwu 0x100
# Since only one watcher was updated, the other can't
# report the change
[0x00000000]> cw*
cw 0x00000000 512 p8
cw 0x00000100 16 p8 # differs
```

## Watching for code modification

Here is an example of using a disassembly command to watch code being modified.

```
# Write an initial binary blob for the example
[0x00000000]> wx 5053595a
# Use pD since it counts by bytes
[0x00000000]> cw 0 4 pD
# Watcher prints disassembly
[0x00000000]> cw
0x00000000
            0x00000000      50             push rax
            0x00000001      53             push rbx
            0x00000002      59             pop rcx
            0x00000003      5a             pop rdx
# Modify the code
[0x00000000]> wx 585b5152
[0x00000000]> cwu
# Watcher prints different disassembly and reports a change
[0x00000000]> cw
0x00000000 modified
            0x00000000      58             pop rax
            0x00000001      5b             pop rbx
            0x00000002      51             push rcx
            0x00000003      52             push rdx

```
