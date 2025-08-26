## r2ai — AI integration for radare2

r2ai is a family of tools and plugins that bring large language models (LLMs) to radare2. It includes a native C plugin (r2ai-plugin), a javascript decompiler frontend (decai), helper tooling and optional Python/CLI clients. r2ai can run local models (llama/ollama/llamacpp) or talk to remote APIs (OpenAI, Anthropic, Mistral, Gemini, etc.).

This chapter explains how to install r2ai, basic usage from the r2 shell and examples, and how to use decai for decompilation and the auto mode. A short note about r2mcp/model control is included at the end.

### Key components

* r2ai-plugin (native C plugin, recommended) — adds the `r2ai` command to r2
* decai (r2js) — `decai` command focused on AI-assisted decompilation

### Installation

Recommended: use the radare2 package manager (r2pm).

* List available r2ai packages:

```bash
$ r2pm -s r2ai
r2ai      r2ai plugin written in plain C
r2mcp     radare2 Model Context Protocol server
decai     LLM-based decompiler written in r2js
```

* Install the recommended native plugin and decai:

```bash
$ r2pm -Uci r2ai
$ r2pm -Uci decai
```

Notes about API keys

If you plan to use remote APIs (OpenAI, Anthropic, Mistral, Gemini, ...), put the key in the corresponding file in your home directory (the r2ai plugin will read these):

* OpenAI:  $HOME/.r2ai.openai-key
* Anthropic: $HOME/.r2ai.anthropic-key
* Mistral: $HOME/.r2ai.mistral-key
* Gemini: $HOME/.r2ai.gemini-key

Example:

```
$ echo "sk-..." > ~/.r2ai.openai-key
$ chmod 600 ~/.r2ai.openai-key
```

### Running r2ai from radare2

Once installed, the `r2ai` command will be available inside the radare2 shell.

```console
$ r2 -qc "r2ai -a solve this crackme" /path/to/binary
```

General configuration can be saved via your radare2 config file (for example `~/.radare2rc`) using `r2ai -e` options. Example to set default API and model:

```
r2ai -e api=anthropic
r2ai -e model=claude-3-7-sonnet-20250219
r2ai -e max_tokens=64000
```

### Basic usage and examples

From inside radare2 (after opening a binary):

* Ask general questions or embed r2 outputs: you can pipe the output of r2 commands to the assistant (r2ai supports embedding the result of r2 commands into the prompt to give context about functions, strings, graphs, etc.).

**Example**: ask what a function does (pseudo-commands shown):

```console
[0x00400500]> s sym.main ; af
[0x00400500]> r2ai -d Explain what the current function does in two sentences and list the possible vulnerabilities
```

As long as r2pipe is the main scripting interface for radare2, automating actions that reuse r2ai is as easy as running the `r2ai` command via r2js or your language of choice, harvest the output and work on that.

Use cases

* Solve crackmes with no interactions
* Improve decompilation output
* Better function signatures and Type propagation
* Quick summary and explanation of unfamiliar functions
* Identify potential vulnerabilities (buffer overflows, format-string bugs, etc.)
* Suggest variable/argument names and produce human-readable documentation
* Assist writing small patches or exploit PoCs
* Help reverse engineers learn and speed up triage

### decai — AI-based decompilation

decai is a companion r2js plugin that focuses on decompilation powered by LLMs. It provides a `decai` command in the r2 shell and can use local models (ollama/llama) or remote APIs.

Install:

```
$ r2pm -i decai
```

Basic usage (inside radare2):

```console
[0x00406cac]> decai -h
Usage: decai (-h) ...
 decai -H         - help setting up r2ai
 decai -d [f1 ..] - decompile given functions
 decai -dr        - decompile function and its called ones (recursive)
 decai -dd [..]   - same as above, but ignoring cache
 decai -dD [query]- decompile current function with given extra query
 decai -q [text]  - query language model with given text
 decai -a [query] - solve query with auto mode
 ...
```

Examples

* Decompile the current function:

```console
[0x00406cac]> decai -d
```

* Decompile recursively (function and callees):

```console
[0x00406cac]> decai -dr
```

* Run a quick question about a symbol or behavior:

```console
[0x00406cac]> decai -q "Explain what forkpty does in 2 lines"
```

Configuration and models

* Use `decai -e` to list and modify config values like `api`, `model`, `prompt`, `lang`, `hlang` (human language), `ctxfile`, and `cache`.

* Example to use Ollama and set a model:

```console
[0x00002d30]> decai -e api=ollama
[0x00002d30]> decai -e model=gpt-oss
[0x00002d30]> decai -d
```

decai also supports an Auto mode that chains queries and uses function-calling style interactions with the model to refine outputs and solve higher-level tasks (for example: find vulnerabilities, produce patches, or generate documentation).

### Auto mode

Auto mode allows r2ai/decai to perform multi-step tasks automatically (for example: inspect a function, decompile it, search for issues, propose a patch). With auto mode you can give a high-level instruction and let the tooling attempt to solve it using the available tools and models.

Example (decai auto mode):

```console
[0x00002d30]> decai -a Find buffer overflows and propose a short patch for the current function
```

**Note**: Auto mode depends heavily on the model and the chosen API. Always validate the results produced by the model — LLM output can be incorrect or hallucinated.

```console
[0x00002d30]> decai -a solve this crackme
```

### r2mcp - model context protocol

r2mcp is the model context protocol server that permits the use of radare2 with AI agents, the source can be pulled from [https://github.com/radareorg/radare2-mcp](github) but it is usually installed via r2pm.

An MCP server acts like a modernized inetd, utilizing JSON-RPC via stdio to present tools with descriptions, enabling AI models to understand and execute these tools automatically in workflows to address user queries effectively.

In Mai, Cursor, Visual Studio, Claude, Kiro, Zed, .. just press **CMD+,** to get into settings and add the r2mcp server in the config file (`claude_desktop_config.json` for example)

```json
{
    "mcpServers": {
        "radare2": {
            "command": "r2pm",
            "args": ["-r", "r2mcp"]
        }
    }
}
```

**NOTE** to reduce the amount of tools and therefor use less context for the MCP tooling use the `-m` flag.

```shell
$ r2pm -r r2mcp -h
Usage: r2mcp [-flags]
 -c [cmd]   run those commands before entering the mcp loop
 -d [pdc]   select a different decompiler (pdc by default)
 -h         show this help
 -m         expose minimum amount of tools
 -n         do not load any plugin or radare2rc
 -v         show version
$
```

### Troubleshooting and tips

* If the r2ai native plugin is not loaded set the `R2_DEBUG=1` environment and check the logs
* If the model returns garbage or is too verbose, tune the `prompt`/`model`/`max_tokens` config via `r2ai -e` or `decai -e`.
* For offline/local usage prefer Ollama or a local llama binary to reduce latency and avoid sending sensitive binaries to third-party APIs.
* Always pin the model and prompt for reproducible results in analysis workflows.
* The `deterministic` variable lowers the temperature and `top_*` settings to always get the same output
