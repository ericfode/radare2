/* radare - LGPL - Copyright 2010 */
/*   nibble<.ds@gmail.com> */
/*   pancake<nopcode.org> */

#include <r_anal.h>
#include <r_util.h>
#include <r_list.h>

R_API RAnalysisRef *r_anal_ref_new() {
	return r_anal_ref_init (MALLOC_STRUCT (RAnalysisRef));
}

R_API RList *r_anal_ref_list_new() {
	RList *list = r_list_new ();
	list->free = &r_anal_ref_free;
	return list;
}

R_API void r_anal_ref_free(void *ref) {
	free (ref);
}

R_API RAnalysisRef *r_anal_ref_init(RAnalysisRef *ref) {
	if (ref)
		*ref = -1;
	return ref;
}