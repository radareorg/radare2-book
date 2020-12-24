/* radare - Copyright 2020 pancake */

#include <r_types.h>
#include <r_core.h>
#include <r_io.h>

static char *indentcode (char *r) {
	*r++ = ' ';
	*r++ = ' ';
	*r++ = ' ';
	*r++ = ' ';
	strcpy (r, Color_YELLOW);
	r += strlen (r);
	return r;
}

static char *md2txt(const char *in) {
	int link = 0;
	char *res = calloc (4 * strlen (in) + 1, 1);
	char *r = res;
	int col = 0;
	bool iscode = false;
	bool spcode = false;
	while (*in) {
		if (col == 0 && spcode) {
			if (!strncmp (in, "    ", 4)) {
				iscode = spcode = false;
				strcpy (r, Color_RESET);
				r += strlen (r);
			}
		}
		switch (*in) {
		case ' ':
			if (col == 0 && in[1] == ' ') {
				const char *arg = r_str_trim_head_ro (in);
				r = indentcode (r);
				in = arg - 1;
				spcode = true;
			} else {
				*r++ = *in;
			}
			break;
		case '`':
			if (col == 0) {
				if (!strncmp (in, "```", 3)) {
					iscode = !iscode;
					if (!iscode) {
						strcpy (r, Color_RESET);
						r += strlen (r);
					}
					in += 2;
				} else {
					*r++ = *in;
				}
			} else {
				*r++ = *in;
			}
			break;
		case '[':
			link = 1;
			break;
		case ']':
			if (link == 1) {
				link = 2;
			}
			break;
		case '(':
			if (link == 2) {
				link = 3;
			}
			break;
		case ')':
			if (link == 3) {
				link = 0;
			}
			break;
		case '\t':
			if (col == 0) {
				strcpy (r, Color_YELLOW);
				r += strlen (r);
			} else {
				*r++ = '-';
				*r++ = '*';
			}
			break;
		case '\n':
			link = 0;
			*r++ = *in;
			col = -1;
			if (iscode) {
				r = indentcode (r);
			}
			break;
		default:
			if (link == 0 || link == 1) {
				*r++ = *in;
			}
			break;
		}
		in++;
		col++;
	}
	*r = 0;
	return res;
}

static char *r2book_index_str(RCore *core) {
	char *file = r_str_newf ("%s/SUMMARY.md", R2BOOK_HOME);
	if (r_file_exists (file)) {
		char *data = r_file_slurp (file, NULL);
		char *md = md2txt (data);
		free (data);
		return md;
	}
	return NULL;
}

static char *r2book_body_str(RCore *core, const char *path) {
	char *file = r_str_newf ("%s/%s.md", R2BOOK_HOME, path);
	if (r_file_exists (file)) {
		// char *data = r_file_slurp (file, NULL);
		// return data;
		char *data = r_file_slurp (file, NULL);
		char *md = md2txt (data);
		free (data);
		return md;
	}
	return strdup ("Not Found.");
}

static void edit_page(RCore *core, const char *path) {
	char *file = r_str_newf ("%s/%s.md", R2BOOK_HOME, path);
	r_sys_cmdf ("vim %s", file);
	free (file);
}

static void r2book_view(RCore *core, const char *path) {
	char buf[128];
	int split = 22;
	char *index = NULL;
	char *body = NULL;
	int col = 1;
	bool stop = false;
	int scroll[2] = {0, 0};
	reload:

	index = r2book_index_str (core);
	body = r2book_body_str (core, path);

	r_cons_enable_mouse (true);
	while (!stop) {
		int h, w = r_cons_get_size (&h);
		char *si = r_str_crop (index, 0, scroll[0], split - 2, scroll[0] + h - 3);
		char *_sb = r_str_wrap (body, w - split - 4);
		char *sb = r_str_crop (_sb, 0, scroll[1], w, scroll[1] + h - 1);
		free (_sb);
		if (si && sb) {
			r_cons_clear00 ();
			if (col) {
				r_cons_print ("--- SUMMARY.md -");
			} else {
				r_cons_print ("--[ SUMMARY.md ]");
			}
			int i;
			for (i = 16; i < split - 1; i++) {
				r_cons_print ("-");
			}
			r_cons_println (".");
			sb = r_str_append (sb, Color_RESET);
			r_cons_printat (sb, split + 2, 1);
			/// r_cons_print (Color_RESET);
			r_cons_printat (si, 0, 3);
			r_cons_line (split, 2, split + 1, h + 1, '|');
		}
		free (si);
		free (sb);
		r_cons_flush ();
		int ch = r_cons_readchar ();
		ch = r_cons_arrow_to_hjkl (ch);
		switch (ch) {
		case 9: // TAB
			col = !col;
			break;
		case 'H':
			split -= 4;
			if (split < 1) {
				split = 0;
			}
			break;
		case 'g':
			scroll[col] = 0;
			break;
		case 'G':
			{
				int lines = r_str_char_count (col?body:index, '\n');
				scroll[col] = lines - (h / 2);
			}
			break;
		case 'e':
			edit_page (core, path);
			free (index);
			free (body);
			goto reload;
			break;
		case 'h':
			split--;
			if (split < 1) {
				split = 0;
			}
			break;
		case 'l':
			split++;
			if (split >= w - 2) {
				split = w - 2;
			}
			break;
		case 'L':
			split += 4;
			if (split >= w - 2) {
				split = w - 2;
			}
			break;
		case 'j':
			scroll[col]++;
			break;
		case 'k':
			scroll[col]--;
			if (scroll[col] < 0) {
				scroll[col] = 0;
			}
			break;
		case 'J':
			scroll[col] += 8;
			break;
		case 'K':
			scroll[col] -= 8;
			if (scroll[col] < 0) {
				scroll[col] = 0;
			}
			break;
		case ':':
			r_cons_fgets (buf, sizeof (buf), 0, NULL);
			free (body);
			body = r2book_body_str (core, buf);
			break;
		case '_':
			{
				char *files = r_sys_cmd_strf ("cd %s ; find * -iname '*.md' | sed -e 's,.md,,'", R2BOOK_HOME);
				char *p = r_cons_hud_string (files);
				// char *p = r_core_cmd_strf (core, "r2book~...");
				if (p && *p) {
				// 	free (path);	
					path = p;
					free (index);
					free (body);
					free (files);
					goto reload;
				}
				free (p);
				free (files);
			}
			break;
		case 'q':
			stop = true;
			break;
		}
	}
	r_cons_enable_mouse (false);
	free (index);
	free (body);
}

static int r_cmd_r2book(void *user, const char *input) {
	RCore *core = (RCore *) user;
	if (!strncmp (input, "r2h", 3)) {
		const char *arg = r_str_trim_head_ro (input + 3);
		if (R_STR_ISEMPTY (arg)) {
			eprintf ("Usage: r2h [word]\n");
		} else {
			char *out = r_sys_cmd_strf ("cd /usr/local/include/libr/; grep -R %s *", arg);
			r_cons_printf ("%s%c", out, 10);
			free (out);
		}
		return true;
	} else if (!strncmp (input, "r2book", 6)) {
		const char *arg = r_str_trim_head_ro (input + 6);
		if (R_STR_ISEMPTY (arg)) {
			eprintf ("Usage: r2book topic");
			char *out = r_sys_cmd_strf ("cd %s ; find * -iname '*.md' | sed -e 's,.md,,'", R2BOOK_HOME);
			r_cons_printf ("%s%c", out, 10);
			free (out);
		} else {
			r2book_view (core, arg);
		}
		return true;
	}
	return false;
}

// PLUGIN Definition Info
RCorePlugin r_core_plugin_r2book = {
	.name = "r2book",
	.desc = "The r2book available inside r2",
	.license = "LGPL3",
	.call = r_cmd_r2book,
};

#ifndef R2_PLUGIN_INCORE
R_API RLibStruct radare_plugin = {
	.type = R_LIB_TYPE_CORE,
	.data = &r_core_plugin_r2book,
	.version = R2_VERSION
};
#endif

