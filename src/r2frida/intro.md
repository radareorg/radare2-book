# r2frida

r2frida is a plugin that merges the capabilities of radare2 and Frida, allowing you to inspect and manipulate running processes. It is useful for dynamic analysis and debugging, leveraging radare2's reverse engineering tools and Frida's dynamic instrumentation.

With r2frida you can use short mnemonic r2 commands instead of having to type long javascript oneliners in the prompt, also those commands are executed inside the target process and are well integrated with radare2, allowing to import all the analysis information from dynamic instrumentation into your static analysis environment.

Some of the most relevant features include:

* Running Frida scripts with :. command
* Executing snippets in C, JavaScript, or TypeScript
* Attaching, spawning, or launching processes locally or remotely
* Listing sections, symbols, classes, methods
* Searching memory, creating hooks, and manipulating file descriptors
* Supporting Dalvik, Java, ObjC, Swift, and C interfaces

## Installation

Install r2frida via radare2 package manager:

```sh
$ r2pm -ci r2frida
```

Now you should be able to test if the system session works by running the following command:

```sh
$ r2 frida://0
```

If this is not working try with the `R2_DEBUG=1` environment variable set and see if there's any relevant error. Maybe the plugin is not loaded or it was not compiled for the specific version of radare2 that you are using.

The URI handler of r2frida can be quite complex as it allows you to specify different ways to start a process, attaching as well as the communication channel, permitting it to connect though usb, tcp or working with local programs.
