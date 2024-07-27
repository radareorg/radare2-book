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

But to allow using this radare commands without this prefix, it can be solved either by using shell alias or by adding `/snap/radare2/current/bin` to your `PATH` environment.
Also if `r2pm` gets used it can also be useful to add the user local prefix `~/.local/share/radare2/prefix/bin`.

So as an example could be somthing like this:
```sh
PATH="$HOME/.local/share/radare2/prefix/bin:/snap/radare2/current/bin:$PATH"
```
