# Graph commands

Uunderstanding the structure and flow of a program is crucial. While linear disassembly and text-based analysis have their place, graphs provide a powerful visual representation that can significantly enhance your understanding of complex code.

Radare2's graph capabilities offer a multifaceted approach to visualizing various aspects of a program code structures:

* **Control Flow Graphs (CFG)**: Visualize the logical flow between basic blocks within a function, making it easier to identify loops, conditional branches, and execution paths.

* **Call Graphs**: Map out the relationships between functions, showing which functions call others and how data potentially flows between them.

* **String Reference Graphs**: Illustrate where and how strings are used throughout the program, often providing valuable insights into the program's functionality.

These graphical representations serve multiple purposes:

- Quickly identify complex structures and patterns that might be missed in text-based analysis.
- Facilitate easier navigation through large codebases.
- Aid in understanding the overall architecture and design of the program.
- Assist in locating potential vulnerabilities or points of interest for further investigation.

In the following sections, we'll explore the various graph commands available in radare2, demonstrating how to generate, navigate, and interpret these visual aids to supercharge your reverse engineering workflow.

Let's dive into the world of radare2 graphs and unlock new dimensions in your analysis!

## Commands

Radare2 supports various types of graph available through commands starting with `ag`:

```
[0x00005000]> ag?
Usage: ag<graphtype><format> [addr]
Graph commands:
| aga[format]             data references graph
| agA[format]             global data references graph
| agc[format]             function callgraph
| agC[format]             global callgraph
| agd[format] [fcn addr]  diff graph
| agf[format]             basic blocks function graph
| agi[format]             imports graph
| agr[format]             references graph
| agR[format]             global references graph
| agx[format]             cross references graph
| agg[format]             custom graph
| agt[format]             tree map graph
| ag-                     clear the custom graph
| agn[?] title body       add a node to the custom graph
| age[?] title1 title2    add an edge to the custom graph

Output formats:
| <blank>                 ascii art
| *                       r2 commands
| b                       braile art rendering (agfb)
| d                       graphviz dot
| g                       graph Modelling Language (gml)
| j                       json ('J' for formatted disassembly)
| k                       sdb key-value
| m                       mermaid
| t                       tiny ascii art
| v                       interactive ascii art
| w [path]                write to path or display graph image (see graph.gv.format)
```

## Graph Output Formats

The structure of the commands is as follows: `ag <graph type> <output format>`.

For example, `agid` displays the imports graph in dot format, while `aggj`
outputs the custom graph in JSON format.

Here's a short description for every output format available:

### Ascii Art 

Command: `agf`

Displays the graph directly to stdout using ASCII art to represent blocks and edges.

_Warning: displaying large graphs directly to stdout might prove to be computationally expensive and will make r2 not responsive for some time. In case of a doubt, prefer using the interactive view (explained below)._

### Interactive Ascii Art

Command: `agfv`

Displays the ASCII graph in an interactive view similar to `VV` which allows to move the screen, zoom in / zoom out, ...

### Tiny Ascii Art

Command: `agft`

Displays the ASCII graph directly to stdout in tiny mode (which is the same as reaching the maximum zoom out level in the interactive view).

### Graphviz dot

Command: `agfd`

Prints the dot source code representing the graph, which can be interpreted by programs such as [graphviz](https://graphviz.gitlab.io/download/) or online viewers like [this](http://www.webgraphviz.com/)

### JSON

Command: `agfj`

Prints a JSON string representing the graph.

- In case of the `f` format (basic blocks of function), it will have detailed information about the function and will also contain the disassembly of the function (use `J` format for the formatted disassembly.

- In all other cases, it will only have basic information about the nodes of the graph (id, title, body, and edges).

### Graph Modelling Language

Command: `agfg`

Prints the GML source code representing the graph, which can be interpreted by programs such as [yEd](https://www.yworks.com/products/yed/download)

### SDB key-value

**Command**: `agfk`

Prints key-value strings representing the graph that was stored by sdb (radare2's string database).

## Create your own graph

**Commands**: `agn` and `age` for nodes and edges, `agg` to render

Prints r2 commands that would recreate the desired graph. The commands to construct the graph are `agn [title] [body]` to add a node and `age [title1] [title2]` to add an edge.
The `[body]` field can be expressed in base64 to include special formatting (such as newlines).

To easily execute the printed commands, it is possible to prepend a dot to the command (`.agf*`).

This is a sample r2 script to create a graph using commands:

```
[0x00000000]> e scr.utf8=0
[0x00000000]> agn foo
[0x00000000]> agn bar
[0x00000000]> agn cow
[0x00000000]> age foo bar
[0x00000000]> age foo cow
[0x00000000]> agg
             .--------------------.
             |  foo               |
             `--------------------'
                   t f
                   | |
    .--------------' |
    |                '--------.
    |                         |
.--------------------.    .--------------------.
|  bar               |    |  cow               |
`--------------------'    `--------------------'
[0x00000000]>
```

### Web / image

Command: `agfw`

Radare2 will convert the graph to dot format, use the `dot` program to convert it to a `.gif` image and then try to find an already installed viewer on your system (`xdg-open`, `open`, ...) and display the graph there.

The extension of the output image can be set with the `graph.extension` config variable. Available extensions are `png, jpg, gif, pdf, ps`.

_Note: for particularly large graphs, the most recommended extension is `svg` as it will produce images of much smaller size_

If `graph.web` config variable is enabled, radare2 will try to display the graph using the browser (_this feature is experimental and unfinished, and
disabled by default._)
