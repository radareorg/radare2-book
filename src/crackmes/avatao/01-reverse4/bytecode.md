.bytecode
---------

Well, we did the reverse engineering part, now we have to write a program for
the VM with the instruction set described in the previous paragraph. Here is
the program's functional specification:

- the program must return "*"
- *sym.memory* has to contain the string "Such VM! MuCH reV3rse!" after
  execution
- all 9 instructions have to be used at least once
- *sym.good_if_ne_zero* should not be zero
- instr_P is not allowed to be used more than 9 times

Since this document is about reversing, I'll leave the programming part to the
fellow reader :) But I'm not going to leave you empty-handed, I'll give you one
advice: Except for "J", all of the instructions are simple, easy to use, and it
should not be a problem to construct the "Such VM! MuCH reV3rse!" using them.
"J" however is a bit complicated compared to the others. One should realize that
its sole purpose is to make *sym.good_if_ne_zero* bigger than zero, which is a
requirement to access the flag. In order to increment *sym.good_if_ne_zero*,
three conditions should be met:

- *arg1* should be a negative number, otherwise we would return early
- *sym.written_by_instr_C* should not be 0 when "J" is called. This means that
  "C", "AC", or "SC" instructions should be used before calling "J".
- *arg1_and_0x3f* should be negative when checked. Since 0x3f's sign bit is
  zero, no matter what *arg1* is, the result of *arg1* & 0x3f will always be
  non-negative. But remember that "J" negates *arg1_and_0x3f* if *arg1* & 0x40
  is not zero. This basically means that *arg1*'s 6th bit should be 1
  (0x40 = 01000000b). Also, because *arg1_and_0x3f* can't be 0 either, at least
  one of *arg1*'s 0th, 1st, 2nd, 3rd, 4th or 5th bits should be 1 (0x3f =
  00111111b).

I think this is enough information, you can go now and write that program. Or,
you could just reverse engineer the quick'n'dirty one I've used during the CTF:

```
\x90\x00PSAMuAP\x01AMcAP\x01AMhAP\x01AM AP\x01AMVAP\x01AMMAP\x01AM!AP\x01AM AP\x01AMMAP\x01AMuAP\x01AMCAP\x01AMHAP\x01AM AP\x01AMrAP\x01AMeAP\x01AMVAP\x01AM3AP\x01AMrAP\x01AMsAP\x01AMeIPAM!X\x00CAJ\xc1SC\x00DCR*
```

Keep in mind though, that it was written on-the-fly, parallel to the reversing
phase - for example there are parts that was written without the knowledge of
all possible instructions. This means that the code is ugly and inefficient.
