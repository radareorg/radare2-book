## Flatpak releases

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
flatpak install flathub org.radare.iaito
```

During the installation process, you may be prompted to confirm the installation and to enter your user password. Flatpak will handle all necessary dependencies for iaito, ensuring a smooth installation process. Once the installation is complete, you can launch iaito using the following command:

```sh
flatpak run org.radare.iaito
```

### CLI Configuration

To allow usage of CLI radare applications you need to define the following aliases:

```sh
alias r2='flatpak run --command=r2 org.radare.iaito'
alias r2agent='flatpak run --command=r2agent org.radare.iaito'
alias r2p='flatpak run --command=r2p org.radare.iaito'
alias r2pm='flatpak run --command=r2pm --share=network --devel org.radare.iaito'
alias r2r='flatpak run --command=r2r org.radare.iaito'
alias rabin2='flatpak run --command=rabin2 org.radare.iaito'
alias radare2='flatpak run --command=radare2 org.radare.iaito'
alias radiff2='flatpak run --command=radiff2 org.radare.iaito'
alias rafind2='flatpak run --command=rafind2 org.radare.iaito'
alias ragg2='flatpak run --command=ragg2 org.radare.iaito'
alias rahash2='flatpak run --command=rahash2 org.radare.iaito'
alias rarun2='flatpak run --command=rarun2 org.radare.iaito'
alias rasign2='flatpak run --command=rasign2 org.radare.iaito'
alias rasm2='flatpak run --command=rasm2 org.radare.iaito'
alias ravc2='flatpak run --command=ravc2 org.radare.iaito'
alias rax2='flatpak run --command=rax2 org.radare.iaito'
```

With this commands, by default no local files will be accesible.
To allow acces to a folder please use the special permissions procedure explained below or use `zenity` inside the flatpak sandbox to open a dialog using XDG portals.
Example:

```console
$ alias r2='flatpak run --command=r2 org.radare.iaito'
$ r2 -
[0x00000000]> o `!zenity --file-selection`
```

### Sandbox Configuration

To manage the sandbox permissions for iaito, you can use the [Flatseal](https://flathub.org/apps/details/com.github.tchx84.Flatseal) utility. Flatseal allows you to configure Flatpak application permissions easily.

Install Flatseal with the following command:

```sh
flatpak install flathub com.github.tchx84.Flatseal
```

Then, run Flatseal to adjust iaito's permissions:

```sh
flatpak run com.github.tchx84.Flatseal
```

Alternatively this can be configured via CLI using `flatpak override`, here there are some examples on how to configure it:

* To allow radare plugins to connect to your network or Internet:

```sh
flatpak override --user --share=network org.radare.iaito
```

* To allow some plugins to attach to a usb device:

```sh
flatpak override --user --device=all org.radare.iaito
```

* To allow some plugins to access to an specific folder not selected by the GUI (with optional `:ro` to only allow read only):

```sh
flatpak override --user --filesystem=/mnt/hdd:ro org.radare.iaito
```

* To reset back to default required permissions this command can be used:

```sh
flatpak override --user --reset org.radare.iaito
```

The Flatpak version of iaito comes bundled with several useful plugins: [r2ghidra](https://github.com/radareorg/r2ghidra), [r2frida](https://github.com/nowsecure/r2frida), [r2yara](https://github.com/radareorg/r2yara), and [decai](https://github.com/radareorg/r2ai). These plugins enhance the functionality of radare2, providing additional capabilities for decompilation, integration with Ghidra, dynamic analysis with Frida, and YARA rule matching.
