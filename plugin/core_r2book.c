/* radare - Copyright 2020 pancake */

#include <r_types.h>
#include <r_core.h>
#include <r_io.h>

static char *md2txt(const char *in) {
	int link = 0;
	char *res = calloc (strlen (in) + 1, 1);
	char *r = res;
	while (*in) {
		switch (*in) {
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
		case '\n':
			link = 0;
			*r++ = *in;
			break;
		default:
			if (link == 0 || link == 1) {
				*r++ = *in;
			}
			break;
		}
		in++;
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

static void r2book_view(RCore *core, const char *path) {
	char buf[128];
	int split = 22;
	char *index = r2book_index_str (core);
	char *body = r2book_body_str (core, path);
	int col = 0;
	bool stop = false;
	int scroll[2] = {0, 0};
	while (!stop) {
		int h, w = r_cons_get_size (&h);
		char *si = r_str_crop (index, 0, scroll[0], split - 2, scroll[0] + h - 3);
		char *_sb = r_str_wrap (body, w - split - 4);
		char *sb = r_str_crop (_sb, 0, scroll[1], w, scroll[1] + h);
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
			r_cons_printat (sb, split + 2, 1);
			r_cons_printat (si, 0, 3);
			r_cons_line (split, 2, split + 1, h + 1, '|');
		}
		free (si);
		free (sb);
		r_cons_flush ();
		int ch = r_cons_readchar ();
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
			scroll[col] += 4;
			break;
		case 'K':
			scroll[col] -= 4;
			if (scroll[col] < 0) {
				scroll[col] = 0;
			}
			break;
		case ':':
			r_cons_fgets (buf, sizeof (buf), 0, NULL);
			free (body);
			body = r2book_body_str (core, buf);
			break;
		case 'q':
			stop = true;
			break;
		}
	}
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
			r_sys_cmdf ("cd /usr/local/include/libr/; grep -R %s *", arg);
		}
		return true;
	} else if (!strncmp (input, "r2book", 6)) {
		const char *arg = r_str_trim_head_ro (input + 6);
		if (R_STR_ISEMPTY (arg)) {
			eprintf ("Usage: r2book topic");
			r_sys_cmdf ("cd %s ; find * -iname '*.md' | sed -e 's,.md,,'", R2BOOK_HOME);
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

