# Signatures

Radare2 has its own format of the signatures, allowing to both load/apply and
create them on the fly. They are available under the `z` command namespace:

```
[0x00000000]> z?
Usage: z[*j-aof/cs] [args]   # Manage zignatures
| z            show zignatures
| z.           find matching zignatures in current offset
| zb[?][n=5]   search for best match
| z*           show zignatures in radare format
| zq           show zignatures in quiet mode
| zj           show zignatures in json format
| zk           show zignatures in sdb format
| z-zignature  delete zignature
| z-*          delete all zignatures
| za[?]        add zignature
| zg           generate zignatures (alias for zaF)
| zo[?]        manage zignature files
| zf[?]        manage FLIRT signatures
| z/[?]        search zignatures
| zc[?]        compare current zignspace zignatures with another one
| zs[?]        manage zignspaces
| zi           show zignatures matching information
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
To remove it just run `z-entry`.

If you want, instead, to save all created signatures, you need to save it into the SDB file using command `zos myentry`.

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
[0x000051c0]> z.
[+] searching 0x000051c0 - 0x000052c0
[+] searching function metrics
hits: 1
[0x000051c0]>
```
Note that `z.` command just checks the signatures against the current address.
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
[0x000051c0]> e? zign.
       zign.autoload: Autoload all zignatures located in ~/.local/share/radare2/zigns
          zign.bytes: Use bytes patterns for matching
   zign.diff.bthresh: Threshold for diffing zign bytes [0, 1] (see zc?)
   zign.diff.gthresh: Threshold for diffing zign graphs [0, 1] (see zc?)
          zign.graph: Use graph metrics for matching
           zign.hash: Use Hash for matching
          zign.maxsz: Maximum zignature length
          zign.mincc: Minimum cyclomatic complexity for matching
          zign.minsz: Minimum zignature length for matching
         zign.offset: Use original offset for matching
         zign.prefix: Default prefix for zignatures matches
           zign.refs: Use references for matching
      zign.threshold: Minimum similarity required for inclusion in zb output
          zign.types: Use types for matching
[0x000051c0]>
```

## Finding Best Matches `zb`

Often you know the signature should exist somewhere in a binary but `z/` and
`z.` still fail. This is often due to very minor differences between the
signature and the function. Maybe the compiler switched two instructions, or
your signature is not for the correct function version. In these situations the
`zb` commands can still help point you in the right direction by listing near
matches.

```
[0x000040a0]> zb?
Usage: zb[r?] [args]  # search for closest matching signatures
| zb [n]           find n closest matching zignatures to function at current offset
| zbr zigname [n]  search for n most similar functions to zigname
```

The `zb` (zign best) command will show the top 5 closest signatures to a
function. Each will contain a score between 1.0 and 0.0.

```
[0x0041e390]> s sym.fclose
[0x0040fc10]> zb
0.96032  0.92400 B  0.99664 G   sym.fclose
0.65971  0.35600 B  0.96342 G   sym._nl_expand_alias
0.65770  0.37800 B  0.93740 G   sym.fdopen
0.65112  0.35000 B  0.95225 G   sym.__run_exit_handlers
0.62532  0.34800 B  0.90264 G   sym.__cxa_finalize
```

In the above example, `zb` correctly associated the `sym.fclose` signature to
the current function. The `z/` and `z.` command would have failed to match here
since both the `B`yte and `G`raph scores are less then 1.0. A 30% separation
between the first and second place results is also a good indication of a
correct match.

The `zbr` (zign best reverse) accepts a zignature name and attempts to find the
closet matching functions. Use an analysis command, like `aa` to find functions
first.

```
[0x00401b20]> aa
[x] Analyze all flags starting with sym. and entry0 (aa)
[0x00401b20]> zo ./libc.sdb
[0x00401b20]> zbr sym.__libc_malloc 10
0.94873  0.89800 B  0.99946 G   sym.malloc
0.65245  0.40600 B  0.89891 G   sym._mid_memalign
0.59470  0.38600 B  0.80341 G   sym._IO_flush_all_lockp
0.59200  0.28200 B  0.90201 G   sym._IO_file_underflow
0.57802  0.30400 B  0.85204 G   sym.__libc_realloc
0.57094  0.35200 B  0.78988 G   sym.__calloc
0.56785  0.34000 B  0.79570 G   sym._IO_un_link.part.0
0.56358  0.36200 B  0.76516 G   sym._IO_cleanup
0.56064  0.26000 B  0.86127 G   sym.intel_check_word.constprop.0
0.55726  0.28400 B  0.83051 G   sym.linear_search_fdes
```
