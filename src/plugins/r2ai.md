## r2ai — AI integration for radare2

r2ai is a family of tools and plugins that bring large language models (LLMs) to radare2. It includes a native C plugin (r2ai-plugin), a javascript decompiler frontend (decai), helper tooling and optional Python/CLI clients. r2ai can run local models (llama/ollama/llamacpp) or talk to remote APIs (OpenAI, Anthropic, Mistral, Gemini, etc.).

This chapter explains how to install r2ai, basic usage from the r2 shell and examples, and how to use decai for decompilation and the auto mode. A short note about r2mcp/model control is included at the end.

### Key components

- r2ai-plugin (native C plugin, recommended) — adds the `r2ai` command to r2
- decai (r2js) — `decai` command focused on AI-assisted decompilation
- r2ai-python / r2ai-server — older Python-based client/server helpers (deprecated in favor of local ollama or r2ai-server)

### Installation

Recommended: use the radare2 package manager (r2pm).

- List available r2ai packages:

```
$ r2pm -s r2ai
r2ai-py             run a local language model integrated with radare2
r2ai-py-plugin      r2ai plugin for radare2
r2ai-plugin         r2ai plugin rewritten in plain C
r2ai-server         start a language model webserver in local
decai               r2ai r2js subproject with focus on LLM decompilation for radare2
```

- Install the recommended native plugin and decai:

```
$ r2pm -i r2ai-plugin
$ r2pm -i decai
```

From sources

- Clone the r2ai repo and build the component you need. The top-level Makefile guides you to subprojects:

```
$ make
Usage: Run 'make' in the following subdirectories instead
src/    - Modern C rewrite in form of a native r2 plugin
py/     - The old Python cli and r2 plugin
decai/  - r2js plugin with focus on decompiling
server/ - shellscript to easily run llamacpp and other
$ cd src
$ make
$ sudo make install      # if you want system-wide install
```

Notes about API keys

If you plan to use remote APIs (OpenAI, Anthropic, Mistral, Gemini, ...), put the key in the corresponding file in your home directory (the r2ai plugin will read these):

- OpenAI:  $HOME/.r2ai.openai-key
- Anthropic: $HOME/.r2ai.anthropic-key
- Mistral: $HOME/.r2ai.mistral-key
- Gemini: $HOME/.r2ai.gemini-key

Example:

```
$ echo "sk-..." > ~/.r2ai.openai-key
$ chmod 600 ~/.r2ai.openai-key
```

### Running r2ai from radare2

Once installed, r2ai exposes the `r2ai` command inside r2. Typical ways to launch it:

- From the r2 shell (native plugin):

```
$ r2 -qc r2ai-r /path/to/binary
```

- Using r2pm to run the packaged application:

```
$ r2pm -r r2ai
```

- If running from the repo or built artifacts, there may be a launcher script such as `r2ai.sh`:

```
$ ./r2ai.sh /path/to/binary
```

General configuration can be saved via your radare2 config file (for example `~/.radare2rc`) using `r2ai -e` or `r2ai -E` options. Example to set default API and model:

```
r2ai -e api=anthropic
r2ai -e model=claude-3-7-sonnet-20250219
r2ai -e max_tokens=64000
```

### Basic usage and examples

From inside radare2 (after opening a binary):

- Start r2 and invoke r2ai in interactive mode (native plugin):

```
[0x00400500]> r2ai
r2ai> help
```

- Ask general questions or embed r2 outputs: you can pipe the output of r2 commands to the assistant (r2ai supports embedding the result of r2 commands into the prompt to give context about functions, strings, graphs, etc.).

Example: ask what a function does (pseudo-commands shown):

```
[0x00400500]> s sym.main
[0x00400500]> pdf
[0x00400500]> r2ai -q "Explain what the current function does in two sentences and list the possible vulnerabilities"
```

- Use r2ai from the shell in repl or batch mode (depending on the installed component):

```
$ r2pm -r r2ai                   # launch r2ai UI/interactive client
$ r2ai -q "What does function at 0x401000 do?" --attach /path/to/binary
```

Scripting and automation

- r2ai is scriptable via r2pipe: you can write scripts that call r2 commands, gather output, and query the model programmatically.

Use cases

- Quick summary and explanation of unfamiliar functions
- Identify potential vulnerabilities (buffer overflows, format-string bugs, etc.)
- Suggest variable/argument names and produce human-readable documentation
- Assist writing small patches or exploit PoCs
- Help reverse engineers learn and speed up triage

### decai — AI-based decompilation

decai is a companion r2js plugin that focuses on decompilation powered by LLMs. It provides a `decai` command in the r2 shell and can use local models (ollama/llama) or remote APIs.

Install:

```
$ r2pm -i decai
```

Basic usage (inside radare2):

```
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

- Decompile the current function:

```
[0x00406cac]> decai -d
```

- Decompile recursively (function and callees):

```
[0x00406cac]> decai -dr
```

- Run a quick question about a symbol or behavior:

```
[0x00406cac]> decai -q "Explain what forkpty does in 2 lines"
```

Configuration and models

- Use `decai -e` to list and modify config values like `api`, `model`, `prompt`, `lang`, `hlang` (human language), `ctxfile`, and `cache`.

- Example to use Ollama and set a model:

```
[0x00002d30]> decai -e api=ollama
[0x00002d30]> decai -e model=codegeex4:latest
[0x00002d30]> decai -d main
```

decai supports an Auto mode that chains queries and uses function-calling style interactions with the model to refine outputs and solve higher-level tasks (for example: find vulnerabilities, produce patches, or generate documentation).

### Auto mode

Auto mode allows r2ai/decai to perform multi-step tasks automatically (for example: inspect a function, decompile it, search for issues, propose a patch). With auto mode you can give a high-level instruction and let the tooling attempt to solve it using the available tools and models.

Example (decai auto mode):

```
[0x00002d30]> decai -a "Find buffer overflows and propose a short patch for the current function"
```

Note: Auto mode depends heavily on the model and the chosen API. Always validate the results produced by the model — LLM output can be incorrect or hallucinated.

### r2mcp (model control / management) — short note

Some workflows mention a "model control" helper (often abbreviated r2mcp in some communities) or the r2ai-server which helps managing and serving local models (llamacpp, llamafile, ollama, etc.).

- r2ai provides server-side helpers (`r2ai-server`) and integration with tools such as Ollama or local llama-based runtimes. These tools let you list available local models and serve them via a local HTTP/OpenAPI endpoint consumed by r2ai/decai.
- If you need a model control protocol or management tool, check the r2ai repository's `server/` and `dist/docker/` folders and the project README for the most up-to-date instructions.

### Troubleshooting and tips

- If the model returns garbage or is too verbose, tune the `prompt`/`model`/`max_tokens` config via `r2ai -e` or `decai -e`.
- For offline/local usage prefer Ollama or a local llama binary to reduce latency and avoid sending sensitive binaries to third-party APIs.
- Always pin the model and prompt for reproducible results in analysis workflows.

### Further reading

- r2ai repository: https://github.com/radareorg/r2ai
- decai (inside the r2ai repo): https://github.com/radareorg/r2ai/tree/master/decai
- r2pm: the radare2 package manager (to install r2ai packages)

