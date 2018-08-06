## List Libraries

Rabin2 can list libraries used by a binary with the `-l` option:
```
$ rabin2 -l `which r2`
[Linked libraries]
libr_core.so
libr_parse.so
libr_search.so
libr_cons.so
libr_config.so
libr_bin.so
libr_debug.so
libr_anal.so
libr_reg.so
libr_bp.so
libr_io.so
libr_fs.so
libr_asm.so
libr_syscall.so
libr_hash.so
libr_magic.so
libr_flag.so
libr_egg.so
libr_crypto.so
libr_util.so
libpthread.so.0
libc.so.6

22 libraries
```
Lets check the output with `ldd` command:
```
$ ldd `which r2`
linux-vdso.so.1 (0x00007fffba38e000)
libr_core.so => /usr/lib64/libr_core.so (0x00007f94b4678000)
libr_parse.so => /usr/lib64/libr_parse.so (0x00007f94b4425000)
libr_search.so => /usr/lib64/libr_search.so (0x00007f94b421f000)
libr_cons.so => /usr/lib64/libr_cons.so (0x00007f94b4000000)
libr_config.so => /usr/lib64/libr_config.so (0x00007f94b3dfa000)
libr_bin.so => /usr/lib64/libr_bin.so (0x00007f94b3afd000)
libr_debug.so => /usr/lib64/libr_debug.so (0x00007f94b38d2000)
libr_anal.so => /usr/lib64/libr_anal.so (0x00007f94b2fbd000)
libr_reg.so => /usr/lib64/libr_reg.so (0x00007f94b2db4000)
libr_bp.so => /usr/lib64/libr_bp.so (0x00007f94b2baf000)
libr_io.so => /usr/lib64/libr_io.so (0x00007f94b2944000)
libr_fs.so => /usr/lib64/libr_fs.so (0x00007f94b270e000)
libr_asm.so => /usr/lib64/libr_asm.so (0x00007f94b1c69000)
libr_syscall.so => /usr/lib64/libr_syscall.so (0x00007f94b1a63000)
libr_hash.so => /usr/lib64/libr_hash.so (0x00007f94b185a000)
libr_magic.so => /usr/lib64/libr_magic.so (0x00007f94b164d000)
libr_flag.so => /usr/lib64/libr_flag.so (0x00007f94b1446000)
libr_egg.so => /usr/lib64/libr_egg.so (0x00007f94b1236000)
libr_crypto.so => /usr/lib64/libr_crypto.so (0x00007f94b1016000)
libr_util.so => /usr/lib64/libr_util.so (0x00007f94b0d35000)
libpthread.so.0 => /lib64/libpthread.so.0 (0x00007f94b0b15000)
libc.so.6 => /lib64/libc.so.6 (0x00007f94b074d000)
libr_lang.so => /usr/lib64/libr_lang.so (0x00007f94b0546000)
libr_socket.so => /usr/lib64/libr_socket.so (0x00007f94b0339000)
libm.so.6 => /lib64/libm.so.6 (0x00007f94affaf000)
libdl.so.2 => /lib64/libdl.so.2 (0x00007f94afdab000)
/lib64/ld-linux-x86-64.so.2 (0x00007f94b4c79000)
libssl.so.1.0.0 => /usr/lib64/libssl.so.1.0.0 (0x00007f94afb3c000)
libcrypto.so.1.0.0 => /usr/lib64/libcrypto.so.1.0.0 (0x00007f94af702000)
libutil.so.1 => /lib64/libutil.so.1 (0x00007f94af4ff000)
libz.so.1 => /lib64/libz.so.1 (0x00007f94af2e8000)
```

If you compare the outputs of `rabin2 -l` and `ldd`, you will notice that rabin2 lists fewer libraries than `ldd`. The reason is that rabin2 does not follow and does not show dependencies of libraries. Only direct binary dependencies are shown.
