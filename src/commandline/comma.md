# RTables and the comma operator (',')

The comma operator (`,`) exposes Radare2's RTables API in the shell. It provides a flexible way to load, query, filter and export tabular data produced by Radare2 commands (functions, symbols, flags, xrefs, sections, headers, etc.). The `,` command and its suffix operators let you build compact oneliners to manipulate large datasets interactively.

## Why use RTables?

* Unified table view for heterogeneous data (symbols, flags, functions, imports, etc.).
* Powerful filtering and projection using concise suffix expressions.
* Easy import/export to CSV, JSON, SQL and other formats for scripting and reporting.
* Compose commands and filters in a single shell pipeline for quick analysis.

## Basic usage

Run `,?` in the r2 shell to see the full help. Common forms:

* `,` — show the current table
* `, [query]` — filter and print the current table (non-destructive)
* `,. file.csv` — load table from CSV file
* `,,` — print current table in CSV
* `,-` — reset current table
* `,j` — print table in JSON
* `,h col1 col2` — define header column names and types
* `,r 1 2 foo` — add a row using the given format

Example: open the help inside r2

```console
[0x00000000]> ,?
Usage: ,[,.-/*jhr] [file]  # load table data
| ,                   display table
| , [table-query]     filter and print table. See ,? for more details
| ,. file.csv         load table from CSV file (comma dot)
| ,,                  print table in csv format (comma comma)
| ,-                  reset table
| ,/?                 query/filter current table (non-destructive)
| ,*>$foo             print table as r2 commands
| ,j                  print table in json format
| ,h xxd foo bar cow  define header column names and types
| ,r 1 2 foo          adds a row using the given format string
```

## Suffix operators and filters

Suffixes are used to query, sort, slice and format table output. They are concise and designed for one-liners. Examples of useful suffixes:

* `c/sort/inc` — sort rows by given column name (increasing)
* `c/sortlen/inc` — sort rows by string length
* `c/cols/c1/c2` — show only selected columns
* `c/gt/0x800` — rows where col > 0x800
* `c/lt/0x800` — rows where col < 0x800
* `c/eq/0x800` — rows where col == 0x800
* `c/ne/0x800` — rows where col != 0x800
* `*/uniq` — get the first row of each group where col0 is unique
* `*/head/10`, `*/tail/10` — take first/last N rows
* `*/skip/10` — skip the first N rows
* `*/page/1/10` — pagination: page 1 of size 10
* `c/str/warn` — grep rows matching substring "warn" in the column
* `c/nostr/warn` — inverse of `str`
* `c/strlen/3` — rows where strlen(col) == 3
* `c/minlen/3, c/maxlen/3` — length comparisons
* `c/sum` — sum of numeric column values

Output formatters:

* `:r2`, `:csv`, `:tsv`, `:json`, `:html`, `:sql`, `:fancy`, `:simple` — control how results are stringified and exported.
* `:header` / `:quiet` — show/hide header row

These operators can be combined. For example, sorting and taking the top results:

```console
# sort functions by number of xrefs (descending) and take the first 10
> afl,xref/sort/dec,any/head/10
```

## Practical examples

* List flags in JSON format

```console
[0x100000960]> f,:json
[{"size":8,"space":"registers","name":"x0"}, ...]
```

* Pretty symbols listing (fancy table output)

```console
[0x100000960]> is,:fancy
# (prints a nicely formatted table with columns like nth, paddr, vaddr, bind, type, size, name...)
```

* Export table as r2 commands (useful for scripting):

```console
,*>$foo   # prints the table as a series of r2 commands, storing into variable $foo
```

* Load a CSV file into a table and filter columns

```console
,. mydata.csv     # load CSV
, c/cols/name/addr  # show only name and addr columns
```

## Tips and recipes

* Use `,j` or `:json` when piping to external tools (jq, python scripts).
* Combine `c/cols` with `c/sort` and `*/head` to quickly find top items.
* Use `,h` to define column names/types when constructing tables manually.
* Reset the table with `,-` if you want to start a fresh context.
* When in doubt, run `,?` to discover all available suffixes and formatters.

## When to use comma vs other commands

Use the comma operator when you need to reason about sets of rows and perform ad-hoc queries: aggregations, sorting, filtering and export. For single-value queries or low-volume output you may prefer the individual commands (`afl`, `is`, `f`, etc.), but the power of `,` shows when dealing with larger lists and programmatic post-processing.

## Further reading

* See `,?` in the r2 shell for live help and the latest suffix names and semantics.
* Inspect RTables source or API if you need to extend or script complex export formats.
