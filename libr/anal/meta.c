/* radare - LGPL - Copyright 2008-2010 nibble<develsec.org> + pancake<nopcode.org> */

#include <r_meta.h>

R_API RMeta *r_meta_new() {
	RMeta *m = R_NEW (RMeta);
	if (m) {
		m->data = r_list_new ();
		m->data->free = r_meta_item_free;
		m->printf = (PrintfCallback) printf;
	}
	return m;
}

R_API void r_meta_free(RMeta *m) {
	r_list_free (m->data);
	/* TODO: memory leak */
	free (m);
}

R_API int r_meta_count(RMeta *m, int type, ut64 from, ut64 to, struct r_meta_count_t *c) {
	RMetaItem *d;
	RListIter *iter;
	int count = 0;

	r_list_foreach (m->data, iter, d) {
		if (d->type == type || type == R_META_ANY) {
			if (from >= d->from && d->to < to) {
				if (c) {
					/* */
				}
				count++;
			}
		}
	}
	return count;
}

R_API char *r_meta_get_string(RMeta *m, int type, ut64 addr) {
	char *str = NULL;
	RListIter *iter;
	RMetaItem *d;

	switch(type) {
	case R_META_COMMENT:
	case R_META_FOLDER:
	case R_META_ANY:
		break;
	case R_META_CODE:
	case R_META_DATA:
	case R_META_STRING:
	case R_META_STRUCT:
		/* we should remove overlapped types and so on.. */
		return "(Unsupported meta type)";
	default:
		eprintf ("Unhandled\n");
		return "(Unhandled meta type)";
	}
	r_list_foreach (m->data, iter, d) {
		if (d->type == type || type == R_META_ANY) {
			if (d->from == addr)
			switch (d->type) {
			case R_META_COMMENT:
				str = r_str_concatf (str, "; %s\n", d->str);
				break;
			case R_META_FOLDER:
				str = r_str_concatf (str, "; FOLDER %"PFMT64d" bytes\n", d->size);
				break;
			}
		}
	}
	return str;
}

R_API int r_meta_del(RMeta *m, int type, ut64 from, ut64 size, const char *str) {
	int ret = R_FALSE;
	RListIter *iter;
	RMetaItem *d;

	r_list_foreach (m->data, iter, d) {
		if (d->type == type || type == R_META_ANY) {
			if (str != NULL && !strstr(d->str, str))
				continue;
			if (from >= d->from && from <= d->to) {
				free (d->str);
				r_list_delete (m->data, iter);
				ret = R_TRUE;
			}
		}
	}
	return ret;
}

R_API int r_meta_cleanup(RMeta *m, ut64 from, ut64 to) {
	RMetaItem *d;
	RListIter *iter;
	int ret = R_FALSE;

	if (from == 0LL && to == UT64_MAX) {
		RMeta *m2 = r_meta_new ();
		r_list_free (m->data);
		m->data = m2->data;
		free (m2);
		return R_TRUE;
	}
	r_list_foreach (m->data, iter, d) {
		switch (d->type) {
		case R_META_CODE:
		case R_META_DATA:
		case R_META_STRING:
		case R_META_STRUCT:
#if 0
			   |__| |__|  |___|  |_|
			 |__|     |_|  |_|  |___|
			 ====== ===== ===== =====
#endif
			if (to>d->from && to<d->to) {
				d->from = to;
				ret= R_TRUE;
			} else
			if (from>d->from && from<d->to &&to>d->to) {
				d->to = from;
				ret= R_TRUE;
			} else
			if (from>d->from&&from<d->to&&to<d->to) {
				// XXX split!
				d->to = from;
				ret= R_TRUE;
			} else
			if (from>d->from&&to<d->to) {
				r_list_delete (m->data, iter);
				ret= R_TRUE;
			}
			break;
		}
	}
	return ret;
}

R_API void r_meta_item_free(void *_item) {
	RMetaItem *item = _item;
	free (item);
}

R_API RMetaItem *r_meta_item_new(int type) {
	RMetaItem *mi = R_NEW (RMetaItem);
	memset (mi, 0, sizeof (RMetaItem));
	mi->type = type;
	return mi;
}

R_API int r_meta_add(RMeta *m, int type, ut64 from, ut64 to, const char *str) {
	RMetaItem *mi = r_meta_item_new (type);
	switch (type) {
	case R_META_CODE:
	case R_META_DATA:
	case R_META_STRING:
	case R_META_STRUCT:
		/* we should remove overlapped types and so on.. */
		r_meta_cleanup (m, from, to);
	case R_META_COMMENT:
	case R_META_FOLDER:
		mi->size = R_ABS (to-from);//size;
		mi->type = type;
		mi->from = from;
		mi->to = to;
		if (str) mi->str = strdup (str);
		else mi->str = NULL;
		r_list_append (m->data, mi);
		break;
	default:
		eprintf ("Unsupported type '%c'\n", type);
		return R_FALSE;
	}
	return R_TRUE;
}

/* snippet from data.c */
R_API RMetaItem *r_meta_find(RMeta *m, ut64 off, int type, int where) {
	RMetaItem *d, *it = NULL;
	RListIter *iter;
	if (off)
	r_list_foreach (m->data, iter, d) {
		if (d->type == type || type == R_META_ANY) {
			switch (where) {
			case R_META_WHERE_PREV:
				if (d->from < off)
					it = d;
				break;
			case R_META_WHERE_HERE:
				if (off>=d->from && off <d->to)
					it = d;
				break;
			case R_META_WHERE_NEXT:
				if (d->from > off)
					it = d;
				break;
			}
		}
	}
	return it;
}

#if 0
	/* not necessary */
//int data_get_fun_for(ut64 addr, ut64 *from, ut64 *to)
int r_meta_get_bounds(RMeta *m, ut64 addr, int type, ut64 *from, ut64 *to)
{
	struct list_head *pos;
	int n_functions = 0;
	int n_xrefs = 0;
	int n_dxrefs = 0;
	struct r_meta_item_t *rd = NULL;
	ut64 lastfrom = 0LL;

	list_for_each(pos, &m->data) {
		struct r_meta_item_t *d = (struct r_meta_item_t *)
			list_entry(pos, struct r_meta_item_t, list);
		if (d->type == type) {
			if (d->from < addr && d->from > lastfrom)
				rd = d;
		}
	}
	if (rd) {
		*from = rd->from;
		*to = rd->to;
		return 1;
	}
	return 0;
}
#endif

R_API const char *r_meta_type_to_string(int type) {
	// XXX: use type as '%c'
	switch(type) {
	case R_META_CODE: return "Cc";
	case R_META_DATA: return "Cd";
	case R_META_STRING: return "Cs";
	case R_META_STRUCT: return "Cm";
	case R_META_COMMENT: return "CC";
	case R_META_FOLDER: return "CF";
	}
	return "(...)";
}

#if 0
#include <r_util.h>
struct r_range_t *r_meta_ranges(RMeta *m)
{
	struct r_range_t *r;
	struct list_head *pos;

	r = r_range_new();
	list_for_each(pos, &m->data) {
		struct r_meta_item_t *d = (struct r_meta_item_t *)
			list_entry(pos, struct r_meta_item_t, list);
		r_range_add(r, d->from, d->to, 1); //d->type);
	}
	return r;
}
#endif

static void printmetaitem(RMeta *m, RMetaItem *d) {
	char *str = r_str_unscape (d->str);
	m->printf ("%s %d \"%s\" @ 0x%08"PFMT64x"\n",
		r_meta_type_to_string (d->type),
		(int)(d->to-d->from), str, d->from);
	free (str);
}

// TODO: Deprecate
R_API int r_meta_list(RMeta *m, int type) {
	int count = 0;
	RListIter *iter;
	RMetaItem *d;

	r_list_foreach (m->data, iter, d) {
		if (d->type == type || type == R_META_ANY) {
			printmetaitem (m, d);
			count++;
		}
	}
	return count;
}