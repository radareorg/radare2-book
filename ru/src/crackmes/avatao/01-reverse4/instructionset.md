.instructionset

---------------

Теперь все инструкции виртуальной машины взломаны, получено полное представление об их работе. Приведем набор инструкций виртуальной машины:

| Инструкция | Первый аргумент | Второй аргумент | Функция исструкции |
| ----------- | ------- | ------- | ----------------
| "A" | "M" | arg2 | \*sym.current_memory_ptr += arg2 |
|             | "P" | arg2 | sym.current_memory_ptr += arg2 |
|             | "C" | arg2 | sym.written_by_instr_C += arg2 |
| "S" | "M" | arg2 | \*sym.current_memory_ptr -= arg2 |
|             | "P" | arg2 | sym.current_memory_ptr -= arg2 |
|             | "C" | arg2 | sym.written_by_instr_C -= arg2 |
| "I" | arg1 | отсутствует | instr_A(arg1, 1) |
| "D" | arg1 | отсутствует | instr_S(arg1, 1) |
| "P" | arg1 | отсутствует | \*sym.current_memory_ptr = arg1; instr_I("P") |
| "X" | arg1 | отсутствует | \*sym.current_memory_ptr ^= arg1 |
| "J" | arg1 | отсутствует | arg1_and_0x3f = arg1 & 0x3f;<br>if (arg1 & 0x40 != 0)<br>&nbsp;&nbsp;arg1_and_0x3f \*= -1<br>if (arg1 >= 0) return arg1_and_0x3f;<br>else if (\*sym.written_by_instr_C != 0) {<br>&nbsp;&nbsp;if (arg1_and_0x3f < 0)<br>&nbsp;&nbsp;&nbsp;&nbsp;++\*sym.good_if_ne_zero;<br>&nbsp;&nbsp;return arg1_and_0x3f;<br>} else return 2; |
| "C" | arg1 | отсутствует | \*sym.written_by_instr_C = arg1 |
| "R" | arg1 | отсутствует | return(arg1) |
