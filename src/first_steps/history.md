## History

In 2006, Sergi Àlvarez (aka pancake) while working as a forensic analyst he decided to write a small tool to recover some deleted files from an HFS+ disk by accident. As long as using the privative software from work it was a good toy project, following the concept of a block-based hexadecimal editor interface with a very simple repl to enter commands to search for byte patterns and dump the results to disk. And have the following characteristics:

* be extremely portable (unix friendly, command line, c, small)
* open disk devices, this is using 64bit offsets
* search for a string or hexpair
* review and dump the results to disk

After three years of intense development (in 2009) the project was too bloated to keep it as a monolitic tool and pancake decided to redesign the tool into a more pluggable and modular form. This way it was possible to create scripts, bindings and write plugins to add support for more architectures, debugging targets or improve the analysis capabilities by keeping the core intact.

Since then, the project has evolved to provide a complete framework for analyzing binaries, while making use of basic UNIX concepts. Those concepts include the famous "everything is a file", "small programs that interact using stdin/stdout", and "keep it simple" paradigms.

The need for scripting showed the fragility of the initial design: a monolithic tool made the API hard to use, and so a deep refactoring was needed. In 2009 radare2 (r2) was born as a fork of radare1. The refactor added flexibility and dynamic features. This enabled much better integration, paving the way to use r2 [from different programming languages](https://github.com/radareorg/radare2-bindings). Later on, the [r2pipe API](https://github.com/radareorg/radare2-r2pipe) allowed access to radare2 via pipes from any language, and the [r2papi](https://github.com/radareorg/radare2-r2papi) provided an idiomatic and high level interface to use r2 through r2pipe from a large list of programming languages.

What started as a one-man project, with some eventual contributions, gradually evolved into a big community-based project around 2014. The number of users was growing fast, changing roles and contribution rules to ease the maintenance as much as possible.

It's important to instruct users to report their issues, as well as help developers willing to contribute to understand the codebase. The whole development is managed in [radare2's GitHub](https://github.com/radareorg/radare2) and discussed in the Telegram and Discord channels.

There are several side projects that provide, among other things, a graphical user interface ([Iaito](https://github.com/radareorg/iaito)), a decompiler ([r2dec](https://github.com/wargio/r2dec-js), [r2ghidra](https://github.com/radareorg/r2ghidra)), Frida integration ([r2frida](https://github.com/nowsecure/r2frida)), Yara ([r2yara](https://github.com/radareorg/r2yara)), Unicorn, Keystone, and many other projects indexed in the [r2pm](https://github.com/radareorg/radare2-pm), the radare2 package manager.

Since 2016, the community gathers once a year in [r2con](https://www.radare.org/con/), a congress around radare2 that takes place in Barcelona.
