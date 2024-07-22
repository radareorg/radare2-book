## FlatHub

The easiest way to get **iaito** and **radare2** installed in your Linux distro is by using Flatpak.

This method ensures a well-tested, sandboxed environment that doesn't interfere with your system dependencies. In this section, we will guide you through the steps to install and configure iaito using Flatpak.

First, ensure that Flatpak is installed and configured on your system. If Flatpak is not already installed, you can add it through your distribution's package manager. For example, on Debian-based systems, you can install Flatpak with the following command:

```sh
sudo apt install flatpak
```

Next, you need to add the Flathub repository, which hosts iaito and many other applications. Run the following command to add Flathub:

```sh
flatpak remote-add --if-not-exists flathub https://flathub.org/repo/flathub.flatpakrepo
```

With Flatpak and the Flathub repository set up, you can install iaito by executing the following command in your terminal:

```sh
flatpak install flathub io.github.radare.iaito
```

During the installation process, you may be prompted to confirm the installation and to enter your user password. Flatpak will handle all necessary dependencies for iaito, ensuring a smooth installation process. Once the installation is complete, you can launch iaito using the following command:

```sh
flatpak run io.github.radare.iaito
```

### Sandbox Configuration

To manage the sandbox permissions for iaito, you can use the Flatseal utility. Flatseal allows you to configure Flatpak application permissions easily. Install Flatseal with the following command:

```sh
flatpak install flathub com.github.tchx84.Flatseal
```

Then, run Flatseal to adjust iaito's permissions:

```sh
flatpak run com.github.tchx84.Flatseal
```

The Flatpak version of iaito comes bundled with several useful plugins: r2dec, r2ghidra, r2frida, and r2yara. These plugins enhance the functionality of radare2, providing additional capabilities for decompilation, integration with Ghidra, dynamic analysis with Frida, and YARA rule matching.

### Snap

The Snap package system is a cross-platform solution for packaging and distributing software on Linux. Snaps bundle all necessary dependencies, ensuring they work on any Linux distribution. This makes Snap an ideal choice for distributing radare2.

Radare2 is distributed via Snap by building it from the continuous integration (CI) system for each release. This means every new official release is packaged and made available to Snap users automatically. While the builds are automated, they are only done for official releases, not for every git commit. This ensures that Snap users have a stable and tested version of radare2.

Snap packages run in a sandboxed environment, isolating them from the rest of the system. This enhances security and prevents conflicts with other software. Snaps can request permissions for specific resources, but they remain contained within their sandbox, ensuring stability and security.

To use radare2 Snap builds, note that they have different program names to avoid conflicts with other installations. Snap versions of radare2 programs are prefixed with radare2.. For example, to run the stable build, use:

```sh
snap install radare2 --channel=stable
radare2.radare2 /bin/ls
radare2.rabin2 -z /bin/sleep
```

This way, you can maintain a stable version alongside a development version without conflicts.
