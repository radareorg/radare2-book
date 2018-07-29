## Pattern Matching Search

The `/p` command allows you to apply repeated pattern searches on IO backend storage. It is possible to identify repeated byte sequences without explicitly specifying them. The only command's parameter sets minimum detectable pattern length.
Here is an example:
```
[0x00000000]> /p 10
```
This command output will show different patterns found and how many times each of them is encountered.
