/*
	fld.c

	Handle files and dir activites
*/

#include <dirent.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <limits.h>
#include <stdio.h>
#include "fld.h"

static void rm_fe (f_entry *fe) {
	if (fe != NULL) {
		free(fe->f_n);
		free(fe);
	}
}

static int cmp_fe (f_entry *e1, f_entry *e2) {
	return strcmp(e1->f_n, e2->f_n);
}

static directory *fail (directory *drh, char *op) {
	fprintf(stderr,"[build_dlist] %s, failed (%s): %s\n", op, drh->d_name, strerror(errno));
	rm_dir_list(drh);
	return NULL;
}

directory *build_dlist (char *path) {
	directory *drh = NULL;
    struct dirent *de;  // Pointer for directory entry
	f_entry *fe;
	DIR *dr;
	char fpn[PATH_MAX+1];
	int n;
	struct stat st;

	if (path == NULL) {
		fprintf(stderr,"[build_dlist] NULL path\n");
		return NULL;
	}
  
    // opendir() returns a pointer of DIR type. 
    dr = opendir(path);
  
    if (dr == NULL)  // opendir returns NULL if couldn't open directory
    {
        fprintf(stderr,"[build_dlist] Could not open directory (%s): %s\n", path, strerror(errno));
		return NULL;
    }
	
	if ((drh = malloc(sizeof (directory))) == NULL) {
		fprintf(stderr,"[build_dlist] failed on memory allocation (%s): %s\n", path, strerror(errno));
		return NULL;
	}

	drh->f_entries = NULL;
	// store directory information, but as a default set it to path
	drh->d_name = path;
	if ((drh->d_name = realpath(path, NULL)) == NULL)
		return fail(drh, "realpath");

    while ((de = readdir(dr)) != NULL) {
		if (!(strcmp(de->d_name, ".") == 0 || (strcmp(de->d_name, "..") == 0))) {
			if ((fe = malloc(sizeof(f_entry))) == NULL)
				return fail(drh, "allocating file entry");

			n = strlen(de->d_name);
			if ((fe->f_n = malloc(n+1)) == NULL)
				return fail(drh, "allocating file name");

			strcpy(fe->f_n, de->d_name);

#ifdef WINDOWS
			sprintf(fpn, "%s\\%s", drh->d_name, fe->f_n);
#else
			sprintf(fpn, "%s/%s", drh->d_name, fe->f_n);
#endif

			// if we can't get file attributes, then just skip the file
			if (stat(fpn, &st) < 0) {
				fprintf(stderr, "[build_dlist] failed to get file attributes (%s): %s\n", de->d_name, strerror(errno));
				free(fe->f_n);
				free(fe);
				continue;;
			}

			// check if file is a regular file. if it doesn't, drop it
			if (!S_ISREG(st.st_mode)) {
				free(fe->f_n);
				free(fe);
				continue;
			}

			fe->l_access = st.st_mtime;

			if ((drh->f_entries = insert(drh->f_entries, fe, (int (*)(void *, void *))&cmp_fe, (void (*)(void *))&rm_fe)) == NULL)
				return fail(drh, "insert new file");
		}
    }

    closedir(dr);  
	return drh;
}

void rm_dir_list (directory *d) {
	if (d != NULL) {
		rem_list(d->f_entries, (void (*)(void *))&rm_fe);
		free(d->d_name);
		free(d);
	}
}

void dir_list (directory *d) {
	l_node *l;
	f_entry *f;
	int i = 0;

	if (d == NULL)
		return;

	l = d->f_entries;
	printf("Directory: %s\n", d->d_name);
	printf("==========================================\n");

	while ((f = (f_entry *)g_next(&l)) != NULL) {
		i++;
		printf("[%d] %s, %s", i, f->f_n, ctime(&(f->l_access)));
	}
}


