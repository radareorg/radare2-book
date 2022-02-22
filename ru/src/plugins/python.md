# Плагины Python

Необходимым условием для реализации модулей расширения на языке Python для radare2 является установка плагина r2lang: `r2pm -i lang-python`. Обратим внимание, что ради читабельности кода в следующих примерах отсутствуют фактическая реализация функций!

Нужно сделать следующее:
1. `import r2lang` и `from r2lang import R` (модуль констант)
2. Далее для плагина RAsm реализуем функцию с двумя подфункциями - `assemble` и `disassemble`, она также должна возвращать структуру, представляющую плагин
```python
def mycpu(a):
    def assemble(s):
        return [1, 2, 3, 4]

    def disassemble(memview, addr):
        try:
            opcode = get_opcode(memview) # https://docs.python.org/3/library/stdtypes.html#memoryview
            opstr = optbl[opcode][1]
            return [4, opstr]
        except:
            return [4, "unknown"]
```
3. Данная конструкция должна содержать указатели на эти две функции – `assemble` и `disassemble`

```python
    return {
            "name" : "mycpu",
            "arch" : "mycpu",
            "bits" : 32,
            "endian" : R.R_SYS_ENDIAN_LITTLE,
            "license" : "GPL",
            "desc" : "MYCPU disasm",
            "assemble" : assemble,
            "disassemble" : disassemble,
    }
```
4. Для плагина RAnal создается функция с двумя подфункциями - `set_reg_profile` и `op`, возвращается структура

```python
def mycpu_anal(a):
       def set_reg_profile():
        profile = "=PC	pc\n" + \
		"=SP	sp\n" + \
		"gpr	r0	.32	0	0\n" + \
		"gpr	r1	.32	4	0\n" + \
		"gpr	r2	.32	8	0\n" + \
		"gpr	r3	.32	12	0\n" + \
		"gpr	r4	.32	16	0\n" + \
		"gpr	r5	.32	20	0\n" + \
		"gpr	sp	.32	24	0\n" + \
		"gpr	pc	.32	28	0\n"
        return profile

    def op(memview, pc):
		analop = {
            "type" : R.R_ANAL_OP_TYPE_NULL,
            "cycles" : 0,
            "stackop" : 0,
            "stackptr" : 0,
			"ptr" : -1,
            "jump" : -1,
            "addr" : 0,
            "eob" : False,
            "esil" : "",
        }
        try:
            opcode = get_opcode(memview) # https://docs.python.org/3/library/stdtypes.html#memoryview
            esilstr = optbl[opcode][2]
            if optbl[opcode][0] == "J": # it's jump
                analop["type"] = R.R_ANAL_OP_TYPE_JMP
                analop["jump"] = decode_jump(opcode, j_mask)
                esilstr = jump_esil(esilstr, opcode, j_mask)

        except:
            result = analop
		# Don't forget to return proper instruction size!
        return [4, result]

```
5. Эта структура должна содержать указатели на две функции `set_reg_profile` и `op`:

```python
    return {
            "name" : "mycpu",
            "arch" : "mycpu",
            "bits" : 32,
            "license" : "GPL",
            "desc" : "MYCPU anal",
            "esil" : 1,
            "set_reg_profile" : set_reg_profile,
            "op" : op,
    }
```
6. (Необязательный шаг) Чтобы добавить дополнительную информацию о размерах операций и выравнивании добавьте подфункцию `archinfo` и поместите ее указатель в структуре:

```python
def mycpu_anal(a):
    def set_reg_profile():
        [...]
    def archinfo(query):
        if query == R.R_ANAL_ARCHINFO_MIN_OP_SIZE:
            return 1
        if query == R.R_ANAL_ARCHINFO_MAX_OP_SIZE:
            return 8
        if query == R.R_ANAL_ARCHINFO_INV_OP_SIZE:  # invalid op size
            return 2
        return 0
    def analop(memview, pc):
        [...]

    return {
            "name" : "mycpu",
            "arch" : "mycpu",
            "bits" : 32,
            "license" : "GPL",
            "desc" : "MYCPU anal",
            "esil" : 1,
            "set_reg_profile" : set_reg_profile,
            "archinfo": archinfo,
            "op" : op,
    }
```
7. Зарегистрируйте оба плагина, используя `r2lang.plugin("asm")` и `r2lang.plugin("anal")`, соответственно.

```python
print("Registering MYCPU disasm plugin...")
print(r2lang.plugin("asm", mycpu))
print("Registering MYCPU analysis plugin...")
print(r2lang.plugin("anal", mycpu_anal))
```

Можно объединить все в один файл и загрузить его с помощью флага `-i` :
```
r2 -I mycpu.py some_file.bin
```
Можно загрузить плагин из оболочки r2: `#!python mycpu.py`

Смотрите также:

* [Python](https://github.com/radareorg/radare2-bindings/blob/master/libr/lang/p/test-py-asm.py),
* [Javascript](https://github.com/radareorg/radare2-bindings/blob/master/libr/lang/p/dukasm.js).

### Реализация плагина формата в Python

Обратим внимание, что ради читабельности кода в следующих примерах отсутствуют конкретные реализации функции!

Нужно выполнить:
1. `import r2lang`,
2. Создайте функцию с подфункциями:
   - `load`,
   - `load_bytes`,
   - `destroy`,
   - `check_bytes`,
   - `baddr`,
   - `entries`,
   - `sections`,
   - `imports`,
   - `relocs`,
   - `binsym`,
   - `info`,

   вернуть структуру плагина (для плагина RAsm):
```python
def le_format(a):
    def load(binf):
        return [0]

    def check_bytes(buf):
        try:
			if buf[0] == 77 and buf[1] == 90:
                lx_off, = struct.unpack("<I", buf[0x3c:0x40])
                if buf[lx_off] == 76 and buf[lx_off+1] == 88:
                    return [1]
            return [0]
        except:
            return [0]
```
Проверяйте параметры всех функций и форматы возвращаемых ими данных. Обратите внимание, что функции `entries`, `sections`, `imports`, `relocs` возвращают список словарей специального вида, каждый со своим типом. Другие функции возвращают только список числовых значений, даже если оно всего одно.
Есть специальная функция, которая возвращает информацию о файле - `info`:
```python
    def info(binf):
        return [{
                "type" : "le",
                "bclass" : "le",
                "rclass" : "le",
                "os" : "OS/2",
                "subsystem" : "CLI",
                "machine" : "IBM",
                "arch" : "x86",
                "has_va" : 0,
                "bits" : 32,
                "big_endian" : 0,
                "dbg_info" : 0,
                }]
```

3. Эта структура должна содержать указатели на наиболее важные функции, такие как `check_bytes`, `load`, `load_bytes`, `entries`, `relocs` и `imports`.

```python
    return {
            "name" : "le",
            "desc" : "OS/2 LE/LX format",
            "license" : "GPL",
            "load" : load,
            "load_bytes" : load_bytes,
            "destroy" : destroy,
            "check_bytes" : check_bytes,
            "baddr" : baddr,
            "entries" : entries,
            "sections" : sections,
            "imports" : imports,
            "symbols" : symbols,
            "relocs" : relocs,
            "binsym" : binsym,
            "info" : info,
    }
```
4. Затем нужно зарегистрировать новый плагин формата файла:

```python
print("Registering OS/2 LE/LX plugin...")
print(r2lang.plugin("bin", le_format))
```
