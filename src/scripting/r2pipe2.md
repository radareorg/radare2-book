## R2Pipe2

The original **r2pipe** protocol is very simple, this have some advantages, but also some inconvieniences and limitations.

The 2nd version aims to address these problems by extending the r2 shell with a new command: `{`.

Sounds funny? Probably yes, but it works and keeps the things simple and powerful.

The new `{` command (introduced in r2-5.9.x) permits to enter a JSON object right into the r2 shell, the output of the command will be another json containing not just the output of the command executed, but also some extra information that was missing in the previous version.

* Command output
* Return value
* Return code
* Error reason
* Log messages

As long as the JSON object can be easily extended in the future more

### R2pipe2 Example

The { object takes the mandatory "cmd" parameter, but can also handle two more fields:

* json: output of the command inlined as json in the resulting object
* trim: remove trailing spaces in the output of the command

Let's check the help:

```
[0x00000000]> {?
Usage: {"cmd":"...","json":false,"trim":true} # `cmd` is required
[0x00000000]>
```

For example:

```
[0x00000000]> '{"cmd":"?e hello"}
{"res":"hello\n","error":false,"value":256,"code":0,"code":0}
[0x00000000]>
```

### R2pipe2 APIs

As you can imagine, the new { command can be used directly from an `r2.callj` command. But r2pipe2 Python, TypeScript and R2JS implementations expose the `r2.cmd2` and `r2.cmd2j` functions to abstract this.
