## Signals

You can send signals to the target process, or change the behaviour of the the debugger and signal handler associated with the `dk` command.

```console
[0x00000000]> dk?
Usage: dk  Signal commands
| dk                         list all signal handlers of child process
| dk <signal>                send KILL signal to child
| dk <signal>=1              set signal handler for <signal> in child
| dk?<signal>                name/signum resolver
| dko[?] <signal>            reset skip or cont options for given signal
| dko <signal> [|skip|cont]  on signal SKIP handler or CONT into
| dkj                        list all signal handlers in JSON
[0x00000000]>
```

To change the behaviour of the r2 debugger when the target process receives a specific signal use the `dko` command. Note that radare2 handles signals in a portable way, so the Windows exceptions will be used instead of the signal unix syscalls.

These are the list of signals with their associated numbers:

```console
[0x00000000]> dk
32 SIGRTMIN 30 SIGPWR 14 SIGALRM 31 SIGSYS 15 SIGTERM 16 SIGSTKFLT
17 SIGCHLD 10 SIGUSR1 11 SIGSEGV 12 SIGUSR2 13 SIGPIPE 18 SIGCONT
19 SIGSTOP 27 SIGPROF 26 SIGVTALRM 25 SIGXFSZ 24 SIGXCPU 23 SIGURG
22 SIGTTOU 5 SIGTRAP 21 SIGTTIN 4 SIGILL 20 SIGTSTP 7 SIGBUS 6 SIGABRT
1 SIGHUP 3 SIGQUIT 2 SIGINT 29 SIGLOST 28 SIGWINCH 9 SIGKILL 8 SIGFPE
```
