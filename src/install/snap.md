## Snap releases

The Snap package system is a cross-platform solution for packaging and distributing software on Linux. Snaps bundle all necessary dependencies, ensuring they work on any Linux distribution. This makes Snap an ideal choice for distributing radare2.

Radare2 is distributed via Snap by building it from the continuous integration (CI) system for each release. This means every new official release is packaged and made available to Snap users automatically. While the builds are automated, they are only done for official releases, not for every git commit. This ensures that Snap users have a stable and tested version of radare2.

Snap packages can run in a sandboxed environment, isolating them from the rest of the system. This enhances security and prevents conflicts with other software. Sandboxed snaps can request permissions for specific resources, but they remain contained within their sandbox, ensuring stability and security.

To use radare2 Snap builds, note that they have different program names to avoid conflicts with other installations. Snap versions of radare2 programs are prefixed with `radare2.`. For example, to run the stable build, use:

```sh
sudo snap install radare2 --classic
radare2.radare2 /bin/ls
radare2.rabin2 -z /bin/sleep
```

This way, you can maintain a stable version alongside a development version without conflicts.

To allow using radare commands without this prefix, it can be solved by using shell aliases. So as an example could be something like this:

```sh
alias r2='radare2.r2'
alias r2agent='radare2.r2agent'
alias r2frida-compile='radare2.r2frida-compile'
alias r2mcp='radare2.r2mcp'
alias r2p='radare2.r2p'
alias r2pm='radare2.r2pm'
alias r2r='radare2.r2r'
alias r2sdb='radare2.r2sdb'
alias rabin2='radare2.rabin2'
alias radiff2='radare2.radiff2'
alias rafind2='radare2.rafind2'
alias rafs2='radare2.rafs2'
alias ragg2='radare2.ragg2'
alias rahash2='radare2.rahash2'
alias rapatch2='radare2.rapatch2'
alias rarun2='radare2.rarun2'
alias rasign2='radare2.rasign2'
alias rasm2='radare2.rasm2'
alias ravc2='radare2.ravc2'
alias rax2='radare2.rax2'
alias sleighc='radare2.sleighc'
alias yara='radare2.yara'
alias yarac='radare2.yarac'
alias clang-format-radare2='radare2.clang-format-radare2'
```

The snap build comes bundled with several useful plugins: [r2dec](https://github.com/wargio/r2dec-js), [r2ghidra](https://github.com/radareorg/r2ghidra), [r2frida](https://github.com/nowsecure/r2frida), [r2yara](https://github.com/radareorg/r2yara), [r2ai](https://github.com/radareorg/r2ai), and [r2mcp](https://github.com/radareorg/radare2-mcp). These plugins enhance the functionality of radare2, providing additional capabilities for decompilation, integration with Ghidra, dynamic analysis with Frida, and YARA rule matching.

### Iaito Snap

Also **iaito**, as a radare GUI, can be installed using this packaging system.

To install it is required to run the following command:

```sh
sudo snap install iaito --classic
```

In the same way radare2 snap include a set of commands, iaito snap also include radare2 commands prefixed with `iaito.` (for example `iaito.r2`, `iaito.rax2`,...).
This commands can be used from CLI, and allow to use the same environment as iaito GUI.
But to use radare2 from CLI it is prefered to install and use the main `radare2` snap package.

### Configure MCP

#### Claude Desktop Integration

In the Claude Desktop app, press `CMD + ,` to open the Developer settings. Edit the configuration file and restart the client after editing the JSON file as explained below:

1. Locate your Claude Desktop configuration file:

   * macOS: `~/Library/Application Support/Claude/claude_desktop_config.json`
   * Windows: `%APPDATA%\Claude\claude_desktop_config.json`

2. Add the following to your configuration file:

```json
{
  "mcpServers": {
    "radare2": {
      "command": "radare2.r2mcp"
    }
  }
}
```

#### VS Code Integration

To use r2mcp with GitHub Copilot Chat in Visual Studio Code by [adding it to your user configuration](https://code.visualstudio.com/docs/copilot/chat/mcp-servers#_add-an-mcp-server-to-your-user-configuration) (see also [add an mcp server to vscode](https://code.visualstudio.com/docs/copilot/chat/mcp-servers#_add-an-mcp-server)):

1. Open the Command Palette with `CMD + Shift + P` (macOS) or `Ctrl + Shift + P` (Windows/Linux).
2. Search for and select `Copilot: Open User Configuration` (typically found in `~/Library/Application Support/Code/User/mcp.json` in macOS).
3. Add the following to your configuration file:

```json
{
  "servers": {
    "radare2": {
      "type": "stdio",
      "command": "radare2.r2mcp"
    }
  },
  "inputs": []
}
```

#### Zed Integration

You can use r2mcp with Zed as well by [adding it to your configuration](https://zed.dev/docs/ai/mcp):

1. Open the command palette: `CMD + Shift + P` (macOS) or `Ctrl + Shift + P` (Windows/Linux).1º
2. Search of `agent: open configuration` or search of `settings`.
3. Add your server as such:

```json
  "context_servers": {
    "r2-mcp-server": {
      "source": "custom",
      "command": "radare2.r2mcp"
    }
  }
```

Note: you will need another LLM agent, such as Claude, Gemini or else to be able to use it.
