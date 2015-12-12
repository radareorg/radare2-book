# Getting Started

## Small session in radare2 debugger

* `r2 -d /bin/ls`: Opens radare2 with file `/bin/ls` in debugger mode using radare2 native debugger, but does not run the program. You’ll see a prompt (radare2) - all examples are from this prompt.

* `db flag`: place breakpoint on flag

* `db`: show list of breakpoint

* `dc`: run the program

* `dr`: Show registers state

* `drr`: Show registers references (telescoping) (like peda)  

* `ds`: Step into instruction

* `dso`: Step over instruction

* `dbt`: Display backtrace

* `dm`: Show memory maps 

* `dk <signal>`: Send KILL signal to child
