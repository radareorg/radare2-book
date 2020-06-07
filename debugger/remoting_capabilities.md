# Remote Access Capabilities

Radare can be run locally, or it can be started as a server process which is controlled by a local
radare2 process. This is possible because everything uses radare's IO subsystem which abstracts access to system(), cmd() and all basic IO operations so to work over a network.

Help for commands useful for remote access to radare:

```
[0x00405a04]> =?
Usage:  =[:!+-=ghH] [...]   # connect with other instances of r2

remote commands:
| =                             list all open connections
| =<[fd] cmd                    send output of local command to remote fd
| =[fd] cmd                     exec cmd at remote 'fd' (last open is default one)
| =! cmd                        run command via r_io_system
| =+ [proto://]host:port        connect to remote host:port (*rap://, raps://, tcp://, udp://, http://)
| =-[fd]                        remove all hosts or host 'fd'
| ==[fd]                        open remote session with host 'fd', 'q' to quit
| =!=                           disable remote cmd mode
| !=!                           enable remote cmd mode

servers:
| .:9000                        start the tcp server (echo x|nc ::1 9090 or curl ::1:9090/cmd/x)
| =:port                        start the rap server (o rap://9999)
| =g[?]                         start the gdbserver
| =h[?]                         start the http webserver
| =H[?]                         start the http webserver (and launch the web browser)

other:
| =&:port                       start rap server in background (same as '&_=h')
| =:host:port cmd               run 'cmd' command on remote server

examples:
| =+tcp://localhost:9090/       connect to: r2 -c.:9090 ./bin
| =+rap://localhost:9090/       connect to: r2 rap://:9090
| =+http://localhost:9090/cmd/  connect to: r2 -c'=h 9090' bin
| o rap://:9090/                start the rap server on tcp port 9090
```

You can learn radare2 remote capabilities by displaying the list of supported IO plugins: `radare2 -L`.

A little example should make this clearer. A typical remote session might look like this:

At the remote host1:

```
$ radare2 rap://:1234
```

At the remote host2:

```
$ radare2 rap://:1234
```

At localhost:

```
$ radare2 -
```

Add hosts

```
[0x004048c5]> =+ rap://<host1>:1234//bin/ls
Connected to: <host1> at port 1234
waiting... ok

[0x004048c5]> =
0 - rap://<host1>:1234//bin/ls
```

You can open remote files in debug mode (or using any IO plugin) specifying URI when adding hosts:

```
[0x004048c5]> =+ =+ rap://<host2>:1234/dbg:///bin/ls
Connected to: <host2> at port 1234
waiting... ok
0 - rap://<host1>:1234//bin/ls
1 - rap://<host2>:1234/dbg:///bin/ls
```

To execute commands on host1:

```
[0x004048c5]> =0 px
[0x004048c5]> = s 0x666
```

To open a session with host2:

```
[0x004048c5]> ==1
fd:6> pi 1
...
fd:6> q
```

To remove hosts (and close connections):

```
[0x004048c5]> =-
```

You can also redirect radare output to a TCP or UDP server (such as `nc -l`). First, Add the server with '=+ tcp://' or '=+ udp://', then you can redirect the output of a command to be sent to the server:

```
[0x004048c5]> =+ tcp://<host>:<port>/
Connected to: <host> at port <port>
5 - tcp://<host>:<port>/
[0x004048c5]> =<5 cmd...
```

The `=<` command will send the output from the execution of `cmd` to the remote connection number N (or the last one used if no id specified).

