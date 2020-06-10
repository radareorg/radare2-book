## Exports

Rabin2 is able to find exports. For example:
```
$ rabin2 -E /usr/lib/libr_bin.so | head
[Exports]
210 0x000ae1f0 0x000ae1f0 GLOBAL   FUNC  200 r_bin_java_print_exceptions_attr_summary
211 0x000afc90 0x000afc90 GLOBAL   FUNC  135 r_bin_java_get_args
212 0x000b18e0 0x000b18e0 GLOBAL   FUNC   35 r_bin_java_get_item_desc_from_bin_cp_list
213 0x00022d90 0x00022d90 GLOBAL   FUNC  204 r_bin_class_add_method
214 0x000ae600 0x000ae600 GLOBAL   FUNC  175 r_bin_java_print_fieldref_cp_summary
215 0x000ad880 0x000ad880 GLOBAL   FUNC  144 r_bin_java_print_constant_value_attr_summary
216 0x000b7330 0x000b7330 GLOBAL   FUNC  679 r_bin_java_print_element_value_summary
217 0x000af170 0x000af170 GLOBAL   FUNC   65 r_bin_java_create_method_fq_str
218 0x00079b00 0x00079b00 GLOBAL   FUNC   15 LZ4_createStreamDecode
```
