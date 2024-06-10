/* radare - Copyright 2020 pancake */

#include <r_types.h>
#include <r_core.h>
#include <r_io.h>

static char *Glang = NULL;

static void box(int x0, int y0, int x1, int y1) {
	int y;
	int bw = x1 - x0;
	r_cons_gotoxy (x0 + 1, y0);
	r_cons_memset ('-', bw - 1);
	for (y = y0 + 1; y < y1; y++) {
		r_cons_gotoxy (x0 + 1, y);
		r_cons_memset (' ', bw - 1);
	}
	r_cons_gotoxy (x0 + 1, y1);
	r_cons_memset ('-', bw - 1);
}

static void translate(void) {
	// translate -l
	eprintf ("Select language to translate to: (ca, es, eo, de, fr, jp, cn)\n");
	const char *line = r_line_readline ();
	free (Glang);
	if (!strcmp (line, "cn")) {
		line = "zh-CN";
	} else if (!strcmp (line, "jp")) {
		line = "ja";
	}
	Glang = strdup (line);
}

static void showhelp(void) {
	int y, h, w = r_cons_get_size (&h);
	int x0 = w / 8;
	int x1 = w - x0;
	int y0 = h / 5;
	int y1 = h - y0;
	box (x0, y0, x1, y1);

	r_cons_printat (
		"Help message\n"
		"------------\n"
		"tab  - focus left or right panels\n"
		"!    - escape to the shell to commit changes\n"
		"_    - open file of the r2book\n"
		"/    - search string in all the r2book\n"
		"t    - language (sudo npm -g install translate)\n"
		"gG   - go begin/end of the page\n"
		"hlHL - move split\n"
		"jkJK - scroll the page\n"
		"e    - edit current page with vim\n"
		"q    - quit\n",
		x0 + 2, y0 + 2
	);
	for (y = y0 + 1; y < y1; y++) {
		r_cons_printat ("|", x0, y);
		r_cons_printat ("|", x1, y);
	}
	
	r_cons_flush ();
	r_cons_readchar ();
}

static char *indentcode(char *r, int *col) {
	char *or = NULL;
	*r++ = ' ';
	*r++ = ' ';
	*r++ = ' ';
	*r++ = ' ';
	strcpy (r, Color_YELLOW);
	int l = strlen (r);
	r += l;
	*col += l;
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
				r = indentcode (r, &col);
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
						col += strlen (Color_RESET);
						r += strlen (Color_RESET);
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
				r = indentcode (r, &col);
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
		if (Glang) {
			r_sys_cmdf ("translate -s en -t %s < %s > /tmp/.a.txt", Glang, file);
			free (file);
			file = strdup ("/tmp/.a.txt");
		}
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
		if (Glang) {
			r_sys_cmdf ("translate -s en -t %s < %s > /tmp/.a.txt", Glang, file);
			free (file);
			file = strdup ("/tmp/.a.txt");
		}
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
		char *si = r_str_ansi_crop (index, 0, scroll[0], split - 2, scroll[0] + h - 3);
		char *_sb = r_str_wrap (body, w - split - 4);
		char *sb = r_str_ansi_crop (_sb, 0, scroll[1], w, scroll[1] + h - 1);
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
		case '!':
			r_sys_cmdf ("echo;cd '%s/..'; PS1='$ ' sh", R2BOOK_HOME);
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
		case 't':
			translate ();
			goto reload;
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
		case '/':
			{
				char *files = r_sys_cmd_strf ("cd %s ; git grep -e '.' | grep md:", R2BOOK_HOME);
				char *p = r_cons_hud_string (files);
				// char *p = r_core_cmd_strf (core, "r2book~...");
				if (p && *p) {
				// 	free (path);	
					char *ch = strstr (p, ".md:");
					*ch = 0;
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
		case '?':
			showhelp ();
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
			char *out = r_sys_cmd_strf ("cd /usr/local/include/libr/; grep -R %s * | grep h: | grep R_API", arg);
			r_cons_printf ("%s%c", out, 10);
			free (out);
		}
		return true;
	} else if (!strncmp (input, "r2books", 7)) {
		char *out = r_sys_cmd_strf ("cd %s ; find * -iname '*.md' | sed -e 's,.md,,'", R2BOOK_HOME);
		r_cons_printf ("%s%c", out, 10);
		free (out);
	} else if (!strncmp (input, "r2book", 6)) {
		const char *arg = r_str_trim_head_ro (input + 6);
		if (R_STR_ISEMPTY (arg)) {
			arg = "intro";
		}
		r2book_view (core, arg);
		return true;
	}
	return false;
}

// PLUGIN Definition Info
RCorePlugin r_core_plugin_r2book = {
	.meta = {
		.name = "r2book",
		.desc = "Adds r2book, r2books and r2h commands",
		.license = "LGPL3",
	},
	.call = r_cmd_r2book,
};

#ifndef R2_PLUGIN_INCORE
R_API RLibStruct radare_plugin = {
	.type = R_LIB_TYPE_CORE,
	.data = &r_core_plugin_r2book,
	.version = R2_VERSION
};
#endif

