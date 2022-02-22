## Поиск в результатах ассемблера

Если надо найти определенные оп-коды ассемблера, можно использовать команды `/a`.

Команда `/ad/ jmp [esp]` ищет указанную категорию мнемоники ассемблера:
```
[0x00404888]> /ad/ jmp qword [rdx]
f hit_0 @ 0x0040e50d   # 2: jmp qword [rdx]
f hit_1 @ 0x00418dbb   # 2: jmp qword [rdx]
f hit_2 @ 0x00418fcb   # 3: jmp qword [rdx]
f hit_3 @ 0x004196ab   # 6: jmp qword [rdx]
f hit_4 @ 0x00419bf3   # 3: jmp qword [rdx]
f hit_5 @ 0x00419c1b   # 3: jmp qword [rdx]
f hit_6 @ 0x00419c43   # 3: jmp qword [rdx]
```

Команда `/a jmp eax` собирает строку для машинного кода, затем выполняет поиск байтов по-кода:
```
[0x00404888]> /a jmp eax
hits: 1
0x004048e7 hit3_0 ffe00f1f8000000000b8
```
