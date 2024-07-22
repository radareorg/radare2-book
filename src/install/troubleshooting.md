## Troubleshooting

Sometimes, old builds or conflicting versions can cause problems, such as tools using the wrong version of libraries, plugins not loading, or crashes with segmentation faults at startup. Here are some steps to help you resolve these issues.

We will assume you have tried to build radare2 with meson and make:

* your builddir doesnt contain spaces
* the repository is a clean clone
* you read the error messages carefully
* you have at least make, gcc, git, patch working

First, if you encounter issues during startup, you can use the `R2_DEBUG=1` environment variable to see detailed debugging information. This can help you understand what is going wrong during the initialization of radare2.

```sh
export R2_DEBUG=1
radare2 -
```

To remove old builds of radare2 from your system, you can run the make purge command. This command will remove previous installations of radare2 from various common prefix paths.

```sh
./configure --prefix=/old/r2/prefix/installation
make purge
```

Additionally, you may need to remove the plugin directory from your home directory to ensure no old or incompatible plugins are causing issues. You can do this by deleting the local/share/radare2/plugins directory.

```sh
rm -rf ~/.local/share/radare2/plugins
```

If you use r2pm (radare2 package manager), it is also a good idea to clear the r2pm cache to free up some disk space and remove potentially problematic cached files. You can do this by deleting the ~/.local/share/radare2/r2pm directory.

```sh
rm -rf ~/.local/share/radare2/r2pm
```

By following these steps, you can clean up old installations and resolve common issues that might arise during the installation and usage of radare2.
