# Signatures

Radare2 has its own format of the signatures, allowing to both load/apply and
create them on the fly. They are available under the `z` command namespace:

```
[0x000100b0]> z?
|Usage: z[*j-aof/cs] [args] # Manage zignatures
| z            show zignatures
| z*           show zignatures in radare format
| zj           show zignatures in json format
| z-zignature  delete zignature
| z-*          delete all zignatures
| za[?]        add zignature
| zo[?]        manage zignature files
| zf[?]        manage FLIRT signatures
| z/[?]        search zignatures
| zc           check zignatures at address
| zs[?]        manage zignspaces
```
To load the created signature file you need to load it from SDB file using `zo` command or
from the compressed SDB file using `zoz` command.

To create signature you need to make function first, then you can create it from the function:
```
r2 /bin/ls
[0x000051c0]> aaa # this creates functions, including 'entry0'
[0x000051c0]> zaf entry0 entry
[0x000051c0]> z
entry:
  bytes: 31ed4989d15e4889e24883e4f050544c............48............48............ff..........f4
  graph: cc=1 nbbs=1 edges=0 ebbs=1
  offset: 0x000051c0
[0x000051c0]>
```
As you can see it made a new signature with a name `entry` from a function `entry0`.
You can show it in JSON format too, which can be useful for scripting:
```
[0x000051c0]> zj~{}
[
  {
    "name": "entry",
    "bytes": "31ed4989d15e4889e24883e4f050544c............48............48............ff..........f4",
    "graph": {
      "cc": "1",
      "nbbs": "1",
      "edges": "0",
      "ebbs": "1"
    },
    "offset": 20928,
    "refs": [
    ]
  }
]
[0x000051c0]>
```
To remove it just run `z-entry`
But if you want to save all created signatures, you need to save it into the SDB file using command
`zos myentry`.
Then we can apply them. Lets open a file again:
```
r2 /bin/ls
 -- Log On. Hack In. Go Anywhere. Get Everything.
[0x000051c0]> zo myentry
[0x000051c0]> z
entry:
  bytes: 31ed4989d15e4889e24883e4f050544c............48............48............ff..........f4
  graph: cc=1 nbbs=1 edges=0 ebbs=1
  offset: 0x000051c0
[0x000051c0]>
```
This means that the signatures were successfully loaded from the file `myentry` and now we can
search matching functions:
```
[0x000051c0]> zc
[+] searching 0x000051c0 - 0x000052c0
[+] searching function metrics
hits: 1
[0x000051c0]>
```
Note that `zc` command just checks the signatures against the current address.
To search signatures across the all file we need to do a bit different thing.
There is an important moment though, if we just run it "as is" - it wont find anything:
```
[0x000051c0]> z/
[+] searching 0x0021dfd0 - 0x002203e8
[+] searching function metrics
hits: 0
[0x000051c0]>
```
Note the searching address - this is because we need to [adjust the searching](../search_bytes/configurating_the_search.md) range first:
```
[0x000051c0]> e search.in=io.section
[0x000051c0]> z/
[+] searching 0x000038b0 - 0x00015898
[+] searching function metrics
hits: 1
[0x000051c0]>
```
We are setting the search mode to `io.section` (it was `file` by default) to search in the current
section (assuming we are currently in the `.text` section of course).
Now we can check, what radare2 found for us:
```
[0x000051c0]> pd 5
;-- entry0:
;-- sign.bytes.entry_0:
0x000051c0      31ed           xor ebp, ebp
0x000051c2      4989d1         mov r9, rdx
0x000051c5      5e             pop rsi
0x000051c6      4889e2         mov rdx, rsp
0x000051c9      4883e4f0       and rsp, 0xfffffffffffffff0
[0x000051c0]>
```
Here we can see the comment of `entry0`, which is taken from the ELF parsing, but also the
`sign.bytes.entry_0`, which is exactly the result of matching signature.

Signatures configuration stored in the `zign.` config vars' namespace:
```
[0x000051c0]> e zign.
zign.bytes = true
zign.graph = true
zign.maxsz = 500
zign.mincc = 10
zign.minsz = 16
zign.offset = true
zign.prefix = sign
zign.refs = true
[0x000051c0]>
```

