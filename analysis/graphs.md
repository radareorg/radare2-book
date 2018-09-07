# Graph commands

When analyzing data it is usually handy to have different ways to represent it in order to get new perspectives to allow the analyst to understand how different parts of the program interact.

Representing basic block edges, function calls, string references as graphs show a very clear view of this information.

Radare2 supports various types of graph available through commands starting with `ag`:

```
[0x00005000]> ag?
|Usage: ag<graphtype><format> [addr]
| Graph commands:
| agc[format] [fcn addr]  Function callgraph
| agf[format] [fcn addr]  Basic blocks function graph
| agx[format] [addr]      Cross references graph
| agr[format] [fcn addr]  References graph
| aga[format] [fcn addr]  Data references graph
| agd[format] [fcn addr]  Diff graph
| agi[format]             Imports graph
| agC[format]             Global callgraph
| agR[format]             Global references graph
| agA[format]             Global data references graph
| agg[format]             Custom graph
| ag-                     Clear the custom graph
| agn[?] title body       Add a node to the custom graph
| age[?] title1 title2    Add an edge to the custom graph
|
| Output formats:
| <blank>                 Ascii art
| v                       Interactive ASCII art
| t                       Tiny ASCII art
| d                       Graphviz dot
| j                       JSON ('J' for formatted disassembly)
| g                       Graph Modelling Language (GML)
| k                       SDB key-value
| *                       r2 commands
| w                       Web/image (see graph.extension and graph.web)
```

The structure of the commands is as follows: `ag <graph type> <output format>`.

For example, `agid` displays the imports graph in dot format, while `aggj`
outputs the custom graph in JSON format.

Here's a short description for every output format available:

### Ascii Art ** (e.g. `agf`)

Displays the graph directly to stdout using ASCII art to represent blocks and edges.

_Warning: displaying large graphs directly to stdout might prove to be computationally expensive and will make r2 not responsive for some time. In case of a doubt, prefer using the interactive view (explained below)._

### Interactive Ascii Art (e.g. `agfv`)

Displays the ASCII graph in an interactive view similar to `VV` which allows to move the screen, zoom in / zoom out, ...

### Tiny Ascii Art (e.g. `agft`)

Displays the ASCII graph directly to stdout in tiny mode (which is the same as reaching the maximum zoom out level in the interactive view).

### Graphviz dot	(e.g. `agfd`)

Prints the dot source code representing the graph, which can be interpreted by programs such as [graphviz](https://graphviz.gitlab.io/download/) or online viewers like [this](http://www.webgraphviz.com/)

### JSON	(e.g. `agfj`)

Prints a JSON string representing the graph.

- In case of the `f` format (basic blocks of function), it will have detailed information about the function and will also contain the disassembly of the function (use `J` format for the formatted disassembly.

- In all other cases, it will only have basic information about the nodes of the graph (id, title, body, and edges).

### Graph Modelling Language (e.g. `agfg`)

Prints the GML source code representing the graph, which can be interpreted by programs such as [yEd](https://www.yworks.com/products/yed/download)

### SDB key-value (e.g. `agfk`)

Prints key-value strings representing the graph that was stored by sdb (radare2's string database).

### R2 custom graph commands (e.g. `agf*`)

Prints r2 commands that would recreate the desired graph. The commands to construct the graph are `agn [title] [body]` to add a node and `age [title1] [title2]` to add an edge.
The `[body]` field can be expressed in base64 to include special formatting (such as newlines).

To easily execute the printed commands, it is possible to prepend a dot to the command (`.agf*`).

### Web / image	(e.g. `agfw`)

Radare2 will convert the graph to dot format, use the `dot` program to convert it to a `.gif` image and then try to find an already installed viewer on your system (`xdg-open`, `open`, ...) and display the graph there.

The extension of the output image can be set with the `graph.extension` config variable. Available extensions are `png, jpg, gif, pdf, ps`.

_Note: for particularly large graphs, the most recommended extension is `svg` as it will produce images of much smaller size_

If `graph.web` config variable is enabled, radare2 will try to display the graph using the browser (_this feature is experimental and unfinished, and
disabled by default._)

