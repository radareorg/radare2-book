## Environment

Radare2 uses several environment variables to determine where to look for important files and directories. You can view these variables and their current values by running the command "r2 -H" in your terminal.

This will display information such as:

* The version of radare2 you're using
* Installation prefix path
* Locations of various resource directories (e.g. for plugins, scripts, configuration files)
* File paths for things like the radare2 history file and cache
* Extensions used for shared libraries on your system

The exact values will depend on your operating system and how radare2 was built and installed. Understanding these variables can be helpful for troubleshooting or customizing your radare2 setup.

Some key variables to note include those pointing to plugin directories, configuration paths, and build flags. By examining the output of "r2 -H", you can see exactly where radare2 is looking for various resources on your system.

This information can be particularly useful if you are writing installation recipes for **Makefile**s.

```sh
$ r2 -H
R2_VERSION=5.8.9
R2_PREFIX=/usr/local
R2_MAGICPATH=/usr/local/share/radare2/5.8.9/magic
R2_INCDIR=/usr/local/include/libr
R2_BINDIR=/usr/local/bin
R2_LIBDIR=/usr/local/lib
R2_LIBEXT=dylib
R2_RCFILE=/Users/pancake/.radare2rc
R2_RDATAHOME=/Users/pancake/.local/share/radare2
R2_HISTORY=/Users/pancake/.cache/radare2/history
R2_CONFIG_HOME=/Users/pancake/.config//radare2
R2_CACHE_HOME=/Users/pancake/.cache/radare2
R2_LIBR_PLUGINS=/usr/local/lib/radare2/5.8.9
R2_USER_PLUGINS=/Users/pancake/.local/share/radare2/plugins
R2_ZIGNS_HOME=/Users/pancake/.local/share//radare2/zigns
```

## RC Files

RC files in radare2 are configuration scripts that are automatically loaded when the tool starts up. They allow users to customize the default behavior and settings of radare2 without having to manually enter commands each time.

These files typically contain a series of radare2 commands that are executed sequentially on startup. This can include things like setting color schemes, defining custom commands, adjusting display options, or loading plugins.

RC files for radare2 are usually placed in specific locations where the tool looks for them by default. While I can't specify exact locations, users generally have options to place RC files in system-wide locations, user home directories, or project-specific folders.

Those files must be in 3 different places:

### System Wide

When initializing, radare2 first checks for a system-wide configuration file. By default, it looks for a file named `radare2rc` in the `/usr/share/radare2/` directory. This allows administrators to set up default configurations that will apply for all users on the system.

This system-wide script is loaded before any user-specific configurations, providing a baseline setup that can then be customized further by individual users if needed.

radare2 will first try to load `/usr/share/radare2/radare2rc`

### Home Directories

Radare2 allows users to customize their experience through configuration files. These files contain r2 commands that are executed on startup, allowing users to set preferences like color schemes and other options.

The main configuration files are typically located in the user's home directory:

* `~/.radare2rc`
* `~/.config/radare2/radare2rc
* `~/.config/radare2/radare2rc.d/

An important feature is the `R2_RCFILE` environment variable. This variable allows users to specify a custom path to their radare2 configuration file. By setting this variable, users can override the default locations and use a configuration file from any location on their system.

This flexibility in configuration allows users to tailor radare2 to their specific needs and preferences, enhancing their workflow and user experience.

If scripting with r2 commands doesn't fit your needs you can also write it in **r2js**, the embedded javascript interpreter inside radare2, or in any other language like Python or C, you can just run the `. foo.r2.js` command to evaluate the specified file.

See the scripting chapter for more details.

### File

Radare2 offers a convenient way to automatically execute scripts when opening specific files. This feature is particularly useful for customizing your analysis environment for different types of files or projects. To take advantage of this functionality, you can create a script file with the same name as the target file, but with an additional `.r2` extension.

For example, if you frequently work with a file named "example.bin", you can create a corresponding script file called "example.bin.r2". Whenever you open "example.bin" in radare2, it will automatically detect and execute the commands contained in "example.bin.r2". This allows you to set up predefined configurations, run specific analysis commands, or perform any other desired operations tailored to that particular file.

This automatic script execution feature provides a seamless way to streamline your workflow and ensure consistent analysis setups for specific files or file types. By leveraging this capability, you can save time and effort by automating repetitive tasks and applying custom configurations without manual intervention each time you open a file in radare2.
