/*
	fld.c
	Handles directory listings
*/

#ifndef FLD_H_
#define FLD_H_

#include <stdio.h>
#include <sys/types.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>
#include "list.h"

typedef struct _f_entry {
	char *f_n;
	time_t l_access;
} f_entry;

typedef struct _directory {
	l_node *f_entries;
	char *d_name;
} directory;

#define IS_NEWER(fe1, fe2) (difftime(fe1->l_access, fe2->l_access) > 0.0)

#ifdef WINDOWS
#define realpath(path, res) _fullpath(res, path, PATH_MAX+1) 
#endif

directory *build_dlist (char *path);
void rm_dir_list (directory *d);
void dir_list (directory *d);

#endif /* FLD_H_ */