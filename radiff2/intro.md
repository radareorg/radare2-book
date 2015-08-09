# Radiff2

Radiff2 is a tool designed to compare binary files, similar to how regular `diff` compares text files.

    $ radiff2 -h
    Usage: radiff2 [-abcCdrspOv] [-g sym] [-t %] [file] [file]
      -a [arch]  specify architecture plugin to use (x86, arm, ..)
      -b [bits]  specify register size for arch (16 (thumb), 32, 64, ..)
      -c         count of changes
      -C         graphdiff code (columns: off-A, match-ratio, off-B)
      -CC        same as above but run `aac` to find more functions
      -d         use delta diffing
      -g [sym|off1,off2]   graph diff of given symbol, or between two offsets
      -n         print bare addresses only (diff.bare=1)
      -O         code diffing with opcode bytes only
      -p         use physical addressing (io.va=0)
      -r         output in radare commands
      -s         compute text distance
      -t [0-100] set threshold for code diff (default is 70%)
      -v         show version information

