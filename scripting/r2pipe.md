# R2pipe

The r2pipe api was initially designed for NodeJS in order to support reusing the web's r2.js API from the commandline. The r2pipe module permits interacting with r2 instances in different methods:


* spawn pipes (r2 -0)
* http queries (cloud friendly)
* tcp socket (r2 -c)

```
         pipe spawn async http tcp rap json
nodejs    x     x     x    x    x   -   x
python    x     x     -    x    x   x   x
swift     x     x     x    x    -   -   x
dotnet    x     x     x    x    -   -   -
haskell   x     x     -    x    -   -   x
java      -     x     -    x    -   -   -
golang    x     x     -    -    -   -   x
ruby      x     x     -    -    -   -   x
rust      x     x     -    -    -   -   x
vala      -     x     x    -    -   -   - 
erlang    x     x     -    -    -   -   -
newlisp   x     -     -    -    -   -   -
dlang     x     -     -    -    -   -   x
perl      x     -     -    -    -   -   -
```

Examples
========

Python
------
```
$ pip install r2pipe
```
```python
import r2pipe

r2 = r2pipe.open("/bin/ls")
r2.cmd('aa')
print(r2.cmd("afl"))
print(r2.cmdj("aflj"))  # evaluates JSONs and returns an object
```

NodeJS
------
```
$ npm install r2pipe
```
```node
var r2jsapi = "./r2.js";
var r2node = require ("./");
var file = "/bin/ls";

function doSomeStuff(r) {
        var r2 = require (r2jsapi)(r);
        r2.analOp ("entry0", function(op) {
                console.log (op.size, op.opcode, op.esil);
        });
        r2.cmd ('af @ entry0', function (o) {
                r2.cmd ("pdf @ entry0", function (o) {
                        console.log (o);
                        r.quit ()
                });
        });
}

r2node.pipe (file, doSomeStuff);
r2node.launch (file, doSomeStuff);
```

**Full API doc:** https://github.com/radare/radare2-bindings/blob/master/r2pipe/nodejs/README.md

Go
```
$ r2pm -i r2pipe-go
```
--
[go-r2pipe repo](https://github.com/radare/r2pipe-go)

```go
package main

import (
	"fmt"
	"github.com/radare/r2pipe-go"
)

func main() {
	r2p, err := r2pipe.NewPipe("/bin/ls")
	if err != nil {
		panic(err)
	}
	defer r2p.Close()

	_, err = r2p.Cmd("aaaa")
	if err != nil {
		panic(err)
	}
	buf0, err := r2p.Cmd("S*")
	if err != nil {
		panic(err)
	}
	fmt.Println(buf0)

	buf1, err := r2p.Cmd("pi 10")
	if err != nil {
		panic(err)
	}
	fmt.Println(buf1)
	
        buf2, err := r2p.Cmd("px 64")
	if err != nil {
		panic(err)
	}
	fmt.Println(buf2)
}

```

Rust
--
```
$ cat Cargo.toml
...
[dependencies]
r2pipe = "*"
```
```rust
#[macro_use]
extern crate r2pipe;
use r2pipe::R2Pipe;
fn main() {
    // Initialize a new pipe
    let mut r2p = open_pipe!(Some("/bin/ls")).unwrap();


    // r2p.cmd() to send a command to r2
    println!("{:?}", r2p.cmd("?e Hello World"));

    // r2p.cmdj() to send a command and parse the returned JSON
    let json = r2p.cmdj("ij").unwrap();
    println!("{}", json.pretty());
    println!("ARCH {}", json.find_path(&["bin","arch"]).unwrap());

    // Closed and Done!
    r2p.close();
}
```

Ruby
----
```
$ gem install r2pipe
```
```ruby
require 'r2pipe'
puts 'r2pipe ruby api demo'
puts '===================='
r2p = R2Pipe.new '/bin/ls'
puts r2p.cmd 'pi 5'
puts r2p.cmd 'pij 1'
puts r2p.json(r2p.cmd 'pij 1')
puts r2p.cmd 'px 64'
r2p.quit
```

Perl
----
```
#!/usr/bin/perl

use R2::Pipe;
use strict;

my $r = R2::Pipe->new ("/bin/ls");
print $r->cmd ("pd 5")."\n";
print $r->cmd ("px 64")."\n";
$r->quit ();
```

Erlang
------
```erlang
#!/usr/bin/env escript
%% -*- erlang -*-
%%! -smp enable

%% -sname hr
-mode(compile).

-export([main/1]).

main(_Args) ->
    %% adding r2pipe to modulepath, set it to your r2pipe_erl location
    R2pipePATH = filename:dirname(escript:script_name()) ++ "/ebin",
    true = code:add_pathz(R2pipePATH),

    %% initializing the link with r2
    H = r2pipe:init(lpipe),

    %% all work goes here
    io:format("~s", [r2pipe:cmd(H, "i")]).
```

Haskell
-------
```haskell
import R2pipe
import qualified Data.ByteString.Lazy as L

showMainFunction ctx = do
    cmd ctx "s main"
    L.putStr =<< cmd ctx "pD `fl $$`"

main = do
    -- Run r2 locally
    open "/bin/ls" >>= showMainFunction
    -- Connect to r2 via HTTP (e.g. if "r2 -qc=h /bin/ls" is running)
    open "http://127.0.0.1:9090" >>= showMainFunction
```

Dotnet
------
```csharp
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using r2pipe;

namespace LocalExample
{
    class Program
    {
        static void Main(string[] args)
        {
#if __MonoCS__
            using(IR2Pipe pipe = new R2Pipe("/bin/ls"))
#else
            using (IR2Pipe pipe = new R2Pipe(@"C:\Windows\notepad.exe",
                @"C:\radare2\radare2.exe"))
#endif
            {
                Console.WriteLine("Hello r2! " + pipe.RunCommand("?V"));
                Task<string> async = pipe.RunCommandAsync("?V");
                Console.WriteLine("Hello async r2!" + async.Result);
                QueuedR2Pipe qr2 = new QueuedR2Pipe(pipe);

                qr2.Enqueue(new R2Command("x", (string result) => {
                       Console.WriteLine("Result of x:\n {0}", result); }));
                qr2.Enqueue(new R2Command("pi 10", (string result) => {
                       Console.WriteLine("Result of pi 10:\n {0}", result); }));

                qr2.ExecuteCommands();
            }
        }
    }
}
```

Java
----
```java
import org.radare.r2pipe.R2Pipe;

public class Test {
        public static void main (String[] args) {
                try {
                        R2Pipe r2p = new R2Pipe ("/bin/ls");
                        //R2Pipe r2p = new R2Pipe ("http://cloud.rada.re/cmd/", true);
                        System.out.println (r2p.cmd ("pd 10"));
                        System.out.println (r2p.cmd ("px 32"));
                        r2p.quit();
                } catch (Exception e) {
                        System.err.println (e);
                }
        }
}
```

Swift
-----
```swift
if let r2p = R2Pipe(url:nil) {
    r2p.cmd ("?V", closure:{
        (str:String?) in
        if let s = str {
            print ("Version: \(s)");
            exit (0);
        } else {
            debugPrint ("R2PIPE. Error");
            exit (1);
        }
    });
    NSRunLoop.currentRunLoop().run();
} else {
    print ("Needs to run from r2")
}
```

Vala

----

```vala
public static int main (string[] args) {
        MainLoop loop = new MainLoop ();
        var r2p = new R2Pipe ("/bin/ls");
        r2p.cmd ("pi 4", (x) => {
                stdout.printf ("DISASM((%s))\n", x);
                r2p.cmd ("ie", (x) => {
                        stdout.printf ("entry((%s))\n", x);
                        r2p.cmd ("q");
                });
        });
        ChildWatch.add (r2p.child_pid, (pid, status) => {
                // Triggered when the child indicated by child_pid exits
                Process.close_pid (pid);
                loop.quit ();
        });
        loop.run ();
        return 0;
}
```

NewLisp
-------
```lisp
(load "r2pipe.lsp")
(println "pd 3:\n" (r2pipe:cmd "pd 3"))
(exit)
```

Dlang
-----
```d
import std.stdio;
import r2pipe;

void main() {
       auto r2 = r2pipe.open ();
       writeln ("Hello "~ r2.cmd("?e World"));
       writeln ("Hello "~ r2.cmd("?e Works"));

       string uri = r2.cmdj("ij")["core"]["uri"].str;
       writeln ("Uri: ",uri);
}
```