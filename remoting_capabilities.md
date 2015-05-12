# Remoting Capabilities

Radare can work locally or remotelly without hard differences. The reason is that everything remains on the IO subsystem that abstracts the access to system(), cmd() and all basic IO operations thru the network.

    Here's the help of the command:
    
    [0x00405a04]> =?
    |Usage:  =[:!+-=hH] [...] # radare remote command execution protocol
    | 
    rap commands:     
    | =                  list all open connections
    | =<[fd] cmd         send output of local command to remote fd
    | =[fd] cmd          exec cmd at remote 'fd' (last open is default one)
    | =! cmd             run command via r_io_system
    | =+ [proto://]host  add host (default=rap://, tcp://, udp://)
    | =-[fd]             remove all hosts or host 'fd'
    | ==[fd]             open remote session with host 'fd', 'q' to quit
    | 
    rap server:       
    | =:port             listen on given port using rap protocol (o rap://9999)
    | =:host:port cmd    run 'cmd' command on remote server
    | 
    http server:      
    | =h port            listen for http connections (r2 -qc=H /bin/ls)
    | =h-                stop background webserver
    | =h*                restart current webserver
    | =h& port           start http server in background)
    | =H port            launch browser and listen for http
    | =H& port           launch browser and listen for http in background

You can also display some of the radare2 remoting capabilities by displaying the list of supported IO plugins `radare2 -L`.

lets introduce the command with a little example :) A typical remote session could be:

At remote host1:

    $ radare2 rap://:1234

At remote host2:

    $ radare2 rap://:1234
    
At localhost: 
    
    $ radare2 -

; Add hosts
   
    [0x004048c5]> =+ rap://<host1>:1234//bin/ls
    Connected to: <host1> at port 1234
    waiting... ok
    
    [0x004048c5]> =
    0 - rap://<host1>:1234//bin/ls

You can open remote files in debug mode (or using any io plugin) specifying the uri when adding hosts: 

    [0x004048c5]> =+ =+ rap://<host2>:1234/dbg:///bin/ls
    Connected to: <host2> at port 1234
    waiting... ok
    0 - rap://<host1>:1234//bin/ls
    1 - rap://<host2>:1234/dbg:///bin/ls

Exec commands in host1

    [0x004048c5]> =0 px
    [0x004048c5]> = s 0x666

Open a session with host2

    [0x004048c5]> ==1
    fd:6> pi 1
    ...
    fd:6> q

Remove hosts (and close connections)
    
    [0x004048c5]> =-

So, you can init tcp or udp servers, add them with '=+ tcp://' or '=+ udp://', and then redirect to them the radare output. For instance:

     [0x004048c5]> =+ tcp://<host>:<port>/
    Connected to: <host> at port <port>
    5 - tcp://<host>:<port>/
    [0x004048c5]> =<5 cmd...
    
The `=<' command will send the result of the execution of the command at the right to the remote connection number N (or the last one used if no id specified).
