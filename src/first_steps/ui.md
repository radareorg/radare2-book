## User Interfaces

Radare2 has seen many different user interfaces being developed over the years.

Maintaining a GUI is far from the scope of developing the core machinery of a reverse engineering toolkit; It is preferred to have a separate project and community, allowing both projects to collaborate and improve alongside each other. This allows individual developers to focus entirely on implementing a CLI or GUI feature instead of trying to juggle both graphical implementation and the low-level logic of the core CLI.

In the past, there have been at least 5 different native user interfaces (ragui, r2gui, gradare, r2net, bokken) but none of them got enough maintenance power to take off and they all died.

### Iaito

The current main radare2 GUI is Iaito. It is written in C++ using Qt and was originally authored by Hugo Teso.

* Download: [https://github.com/radareorg/iaito](https://github.com/radareorg/iaito)

![Iaito screenshot](Iaito.png)

### The Built-in Webserver

In addition, r2 includes an embedded webserver with a basic HTML/JS interface. Add `-c=H` to your command-line flags to automatically start the webserver and open it in your browser.

```
$ r2 -c=H /bin/ls
```
