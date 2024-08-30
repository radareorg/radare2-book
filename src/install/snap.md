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
alias r2p='radare2.r2p'
alias r2pm='radare2.r2pm'
alias r2r='radare2.r2r'
alias rabin2='radare2.rabin2'
alias radiff2='radare2.radiff2'
alias rafind2='radare2.rafind2'
alias ragg2='radare2.ragg2'
alias rahash2='radare2.rahash2'
alias rarun2='radare2.rarun2'
alias rasign2='radare2.rasign2'
alias rasm2='radare2.rasm2'
alias ravc2='radare2.ravc2'
alias rax2='radare2.rax2'
alias sleighc='radare2.sleighc'
alias yara='radare2.yara'
alias yarac='radare2.yarac'
```
