# Signatures (Zignatures)

Radare2 provides a built-in function-signature system called "zignatures" (pronounced "zihg-natures"). Zignatures let you capture compact fingerprints of functions (bytes, graph metrics, hashes and other metadata) and use them to identify the same or similar functions across binaries. They are particularly useful when reversing stripped or statically-linked binaries, hunting for malware variants, or locating known library routines.

This page documents the common workflows, important commands, configuration knobs, practical tips and illustrative console examples for zignatures.

Concepts

- bytes: a masked byte-pattern derived from the function body. Useful for exact or near-exact byte matching.
- graph: function-level structural metrics (cyclomatic complexity, number of basic blocks, number of edges, etc.). Good for high-level matching when bytes differ.
- hash: a quick fingerprint of a function; fast but potentially brittle.
- refs: references (xrefs) and other context that help matching.
- types: function prototype/type hints that may be used when available.

Common commands (overview)

Use the `z` command namespace to manage zignatures. Running `z?` shows the full help; here are the commonly used subcommands:

- zaf <fn> <name>   — create/add a signature from the function at the current offset (or a named function)
- zg                — generate zignatures for all discovered functions (alias for `zaF`)
- zos <file.sdb>    — save current signatures into an SDB file
- zo <file.sdb>     — load zignatures from an SDB file
- z*                — show zignatures in radare2 command format (can be redirected to a .r2 file)
- . <file.r2>       — source a file of radare2 commands (useful to import zignatures created with `z*`)
- z/                — scan the current search range for matching signatures (creates sign.* comments and flags)
- z.                — test signatures against the function at the current offset
- zb [n]            — list the n best matches (similarity scores) for the current function (default n=5)
- zbr <zigname> [n] — find the n most similar functions to the named zignature
- zj                — show zignatures in JSON format (good for scripting)
- zk                — show zignatures in SDB format
- z-<name>          — delete a specific zignature
- z-*               — delete all zignatures
- zs                — manage "zignspaces" (collections of zignatures used when comparing)
- zc                — compare current zignspace with another (useful for diffing two sets of signatures)
- zi [text]         — show information about loaded zignatures matching text

Practical workflows

1) Create a single signature for a function

- Open a file in r2 and analyze functions: `r2 -A <file>` (or run `aa`/`aaa` inside r2).
- Seek to a function of interest (for example `s sym.main`).
- Create a zignature: `zaf sym.main myproj_main` (or `zaf main myproj_main`).
- Inspect it in JSON: `zj~{}` or `zj | jq .`.

2) Create signatures for many functions

- From inside r2 after analysis: `zg` — this will create zignatures for discovered functions.
- Note: `zg` can generate a lot of signatures for large binaries; it’s best for small-to-medium sized files or targeted runs.

3) Save and share signatures

- Save signatures to an SDB database: `zos my_sigs.sdb`.
- Merge into an existing SDB (give an absolute path to a file): `zos /path/to/library.sdb`.
- Load signatures later or on another host: `zo /path/to/my_sigs.sdb`.

4) Export/import using radare2 commands

- Export signatures as radare2 commands that recreate them: `z* > my_sigs.r2`.
- On another instance of r2, you can source the file that contains `za`/`zaf` commands to recreate the signatures: `. my_sigs.r2`.

  Note: `z*` prints zignatures in r2 format (a sequence of `za`/`zaf` commands and options). Redirecting that output to a file and sourcing it in a different r2 session is an easy way to share or import signatures when you don't want to use an SDB file.

5) Use signatures to find known functions in a binary

- Load your signatures: `zo my_sigs.sdb` (or source a .r2 file created with `z*`).
- Optionally adjust search range (important): by default `z/` may search the whole file or a configured range — set `e search.in=io.section` or `e search.in=range` depending on your needs.
- Scan for matches across the active search region: `z/` — this will create `sign.bytes.<name>` or similar comments and flags where matches are found.
- Check a specific function at the current offset: `z.`
- For near-matches or when exact matching fails, use `zb` to see the best matches and their scores.

---

Console examples and field explanations

Below are preserved console examples illustrating common zignature operations and the meaning of each field in the output. Keep these as a quick reference and copy-paste examples when experimenting.

1) z? (help)

```console
[0x100000960]> z?
Usage: z[*j-aof/cs] [args]   # Manage zignatures
| z ([addr])   show/list zignatures
| z.           find matching zignatures in current offset
| z,([:help])  list zignatures loaded in table format (see z,:help)
| zb[?][n=5]   search for best match
| zdzignature  diff current function and signature
| z* ([addr])  show zignatures in radare format
| zq ([addr])  show zignatures in quiet mode
| zj ([addr])  show zignatures in json format
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
| zi [text]    show zignatures matching information
[0x100000960]>
```

Explanation: this is the command help. Use it to find the right subcommand; many subcommands accept `?` for in-command help.


2) Creating a signature from a function (zaf) and inspecting it (zj)

```console
$ r2 /bin/ls
[0x000051c0]> aaa # analyze
[0x000051c0]> zaf entry0 entry
[0x000051c0]> z
entry:
  bytes: 31ed4989d15e4889e24883e4f050544c............48............48............ff..........f4
  graph: cc=1 nbbs=1 edges=0 ebbs=1
  offset: 0x000051c0
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

Field explanations (z / zj output):
- name: the zignature name you provided (here `entry`).
- bytes: a masked byte pattern (dots represent masked/variable bytes). Useful for byte-level matching.
- graph: a small object of graph metrics:
  - cc: cyclomatic complexity (higher means more branching/complexity).
  - nbbs: number of basic blocks in the function.
  - edges: number of edges between basic blocks.
  - ebbs: number of entry basic blocks (usually 1).
- offset: the original offset where the signature was created (file/r2 offset).
- refs: a list of references/xrefs captured for the signature (may be empty).

3) Searching the binary for signatures (z/)

```console
[0x000051c0]> e search.in=io.section
[0x000051c0]> z/
[+] searching 0x000038b0 - 0x00015898
[+] searching function metrics
hits: 1
[0x000051c0]>
```

Explanation:
- The `e search.in` option controls the search area. `io.section` is a common value to restrict search to the current section (e.g., .text).
- z/ prints the search range being scanned (start and end addresses).
- It reports which matching approaches it used (function metrics, bytes, graph, etc.).
- hits: N indicates how many matches were found within the search range.

When a match is found `z/` also annotates the disassembly with a `sign.*` comment. For example:

```console
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

- `sign.bytes.entry_0` is an annotation created by the matcher indicating the matched signature and match type (bytes).

4) Finding best matches (zb)

```console
[0x0041e390]> s sym.fclose
[0x0040fc10]> zb
0.96032  0.92400 B  0.99664 G   sym.fclose
0.65971  0.35600 B  0.96342 G   sym._nl_expand_alias
0.65770  0.37800 B  0.93740 G   sym.fdopen
0.65112  0.35000 B  0.95225 G   sym.__run_exit_handlers
0.62532  0.34800 B  0.90264 G   sym.__cxa_finalize
```

Field explanations (zb output columns):
- First column: overall similarity score (average/combined score between 0.0 and 1.0). Higher is better; 1.0 is a perfect match.
- Second column: bytes-only similarity score (how well the byte pattern matched).
- `B` marker: indicates the previous number is the bytes score.
- Third column after `B`: graph-only similarity score.
- `G` marker: indicates the previous number is the graph score.
- Last column: the zignature/function name matched.

Interpretation: in the example above the top result has an overall score of ~0.96, with a bytes score of ~0.924 and a graph score of ~0.997 — a strong match.

5) z* (export as r2 commands) — example of the radare2 format

`z*` prints zignatures in radare2 `za`/`zaf` command form. This can be saved to a .r2 file and sourced in another r2 session:

Example (excerpt):

```
za sym.unkown g cc=21 nbbs=36 edges=55 ebbs=1 bbsum=592
za sym.fclose g cc=21 nbbs=36 edges=55 ebbs=1 bbsum=583
```

Field explanation (in z* radare format):
- `za` / `zaf`: command to add a signature.
- Following tokens like `g cc=21 nbbs=36 edges=55 ebbs=1 bbsum=592` are the graph metrics for that signature (g = graph metrics):
  - cc: cyclomatic complexity
  - nbbs: number of basic blocks
  - edges: number of edges in the function graph
  - ebbs: number of entry basic blocks
  - bbsum: sum of basic block sizes (a heuristic)

Exporting/importing: redirect `z*` into a file and then use `. file.r2` to recreate the signatures (or load the SDB with `zo`).

---

Example script invocation (what we added in the examples directory)

We included a small script at `src/signatures/examples/zignature_example.sh` that automates a common workflow: generate a libc zignature database using `rasign2`, export an r2 command file, and run `z/` against a target binary. Typical usage:

```sh
chmod +x src/signatures/examples/zignature_example.sh
src/signatures/examples/zignature_example.sh /path/to/target_binary
```

What the script does (summary):
- Runs `rasign2 -o /tmp/libc_sigs.sdb /lib/x86_64-linux-gnu/libc.so.6` to generate zignatures for libc (adjust `LIBC_PATH` in the script if needed).
- Optionally exports the `z*` output to `/tmp/libc_sigs.r2`.
- Runs r2 non-interactively to `aa`, `zo /tmp/libc_sigs.sdb`, set `e search.in=io.section`, and `z/` the target binary.

Example expected output (trimmed)

```console
1) Generate zignatures for libc (may take a few seconds)
Wrote zignatures to: /tmp/libc_sigs.sdb
2) (Optional) Export r2 commands to: /tmp/libc_sigs.r2
3) Scan the TARGET binary using the generated SDB
[+] searching 0x00400000 - 0x0041f000
[+] searching function metrics
hits: 12
```

Then inspect specific functions interactively (example):

```sh
r2 -qc "aa; zo /tmp/libc_sigs.sdb; s sym.fclose; zb; q" /path/to/target_binary
```

This prints the `zb` result with similarity scores (see `zb` field explanations above).

Notes and further tips

- If `z/` finds nothing, double-check `e search.in` and the active section/offset. Searching only the `.text` section (`e search.in=io.section`) is usually the right choice.
- When automating, prefer `zj` (JSON) and r2pipe to programmatically parse matches and extract structured information.
- Consider generating signatures from multiple library versions (different libc versions) to improve recall when the exact library version is unknown.

Resources and further reading

- rasign2 tool: see src/tools/rasign2 in this repository for more examples and usage notes.
- Blog posts with practical workflows and real-world examples:
  - "Reverse Engineer Faster with Radare2 Signatures" — Hurricane Labs
  - "The Art and Science of macOS Malware Hunting with radare2" — SentinelOne

If you want, I can also:
- add an r2pipe example that runs `zj` and summarizes matches across a sample corpus;
- append sample `zj` JSON output and a small explanation of each JSON field in more detail; or
- create a short git-style diff snippet that shows the changes to this document for easier review.
