/*
syncf.c - sunchronize files in source folder with same files on target folders.
Calling syntax: syncf [-l log-file] [-f shell-script-file] [-e] <source-Dir> <target-dir1>...<target-dirN> 
The program synchronize only files with that exists on both source and target folders.
The synchronization takes place only if the file on source-Dir is newer than the same
file on targer-DirX
-l - creates a log file with the name log-file
-f - set shell script file name. default is syncf.sh which is created on current folder
-e - execute the script-file
syncf will create a bash shell script that does the actual file updating.
Note! non-ASCI file names will be ignored 
*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <stdbool.h>
#include <errno.h>
#include "list.h"
#include "fld.h"

#define MAX_TARGETS 20

const char *cmd = "$syncf  [-l <log_file>] [-f <script_file>] [-e] <source> <target1> [<target2>..<targetN>]";
const char *help = "Default script file name: syncf.sh";
const char *def_script_fn = "syncf.sh";

static void print_files (l_node *fe) {
    f_entry *f;

    while ((f = g_next(&fe)) != NULL)
        printf("        %s\n", f->f_n);
}

static int cm_fe (f_entry *s, f_entry *t) {
    return strcmp(s->f_n, t->f_n);
}

static int print_error () {
    printf("wrong input\n");
    printf("%s\n", cmd);
    printf("%s\n", help);
    return -1;
}

static int print_error_close (char *error, char *name, char *fn, FILE *fp) {
    fprintf(stderr, "%s [%s]\n", error, name);
    fclose(fp);
    unlink(fn);
    return -1;   
}
  
int main(int argc, char *argv[])
{
    int  opt = 0;
    directory *sd = NULL;   // source folder
    directory *td;
    l_node *sfl;            // list of source files
    f_entry *sfe, *tfe;     // source and target file entries
    FILE *script_fp = NULL;
    char *script_fn = (char *)def_script_fn;
    char log_str[PATH_MAX+6] = {'\0'};
    char *sd_name = NULL;
    char *td_names[MAX_TARGETS];
    int td_ind = 0;
    bool execute = false;

    // get commad line input and initilalize internal data structures
	if (argc > 1) {
        while (optind < argc) {
            if (opt != -1) {
                if ((opt = getopt(argc, argv, "f:l:e")) != -1) {
                    switch (opt) {
                        case 'f':
                            script_fn = optarg;
                            break;
                        case 'l':
                            sprintf(log_str, "-v >> %s", optarg);
                            break;
                        case 'e':
                            execute = true;
                            break;
                        default:
                            return print_error();
                    }
                }
            }
            else {
                if (sd_name == NULL) {
                    sd_name = argv[optind];
                }
                else {
                    if (td_ind < MAX_TARGETS) {
                        if ((td_names[td_ind] = malloc(strlen(argv[optind]+1))) == NULL) {
                            perror("ERROR: memory allocation failed");
                            return -1;
                        }
                        strcpy(td_names[td_ind], argv[optind]);
                        td_ind++;
                    }
                }
                optind++;
            }
        }
	}
    else {
        return print_error();     
    }

    // complete parsing command input. Check if we have source folder and at least one target folder
    if (sd_name == NULL) {
        fprintf(stderr, "Source folder wasn't entered. Exit...\n");
        return -1;        
    }
    if (td_ind == 0) {
        fprintf(stderr, "No target folder was entered. Exit...\n");
        return -1;
    }

    // open script file
    if ((script_fp = fopen(script_fn, "w")) == NULL) {
        perror("Failed to open output script file");
        return -1;
    }

    // print command inputs to script file as comments:
    fprintf(script_fp, "#!/bin/bash\n");
    fprintf(script_fp, "# Command inputs:\n");
    fprintf(script_fp, "# Script file name: %s\n", script_fn);
    fprintf(script_fp, "# Logging?: %s\n", log_str[0] == '\0'? "No": (char *)&log_str[6]);
    fprintf(script_fp, "# Source folder: %s\n", sd_name);
    fprintf(script_fp, "# Target folders:\n");
    for (int i = 0; i < td_ind; i++)
        fprintf(script_fp, "#    %s\n", td_names[i]);
    fprintf(script_fp, "################################################\n");


    // construct source folder file list
    if ((sd = build_dlist(sd_name)) == NULL) {
        return print_error_close("Failed to create source folder", sd_name, script_fn, script_fp);
    }

    // scan all target folders and compare their files with source folder's files
    for (int i = 0; i < td_ind; i++) {
        td = NULL;
        if ((td = build_dlist(td_names[i])) == NULL) {
            return print_error_close("Failed to create target folder", td_names[i], script_fn, script_fp);
        }

        fprintf(script_fp, "\n# update files on: %s\n", td->d_name);

        // scan source folder and compare its files with those on the target
        sfl = sd->f_entries;
        while ((sfe = g_next(&sfl)) != NULL) {
            if ((tfe = find(td->f_entries, sfe, (int (*)(void *, void *))&cm_fe)) != NULL) {
                if (IS_NEWER(sfe, tfe)) {
#ifdef WINDOWS
                    fprintf(script_fp, "cp \"%s\\%s\" \"%s\\%s\" %s\n", sd->d_name, sfe->f_n, td->d_name, tfe->f_n, log_str);
#else
                    fprintf(script_fp, "cp %s/%s %s/%s %s\n", sd->d_name, sfe->f_n, td->d_name, tfe->f_n, log_str);
#endif //WINDOWS
                }
            }
        }
        rm_dir_list(td);
    }

    rm_dir_list(sd);
    for (int i = 0; i < td_ind; i++) {
        free(td_names[i]);
    }

    fclose(script_fp);
    if (execute) {
        char cmd[PATH_MAX+1];
        int s;
        
        sprintf(cmd, "bash %s", script_fn);
        s = system(cmd);

        if (s < 0)
            perror("Failed to execute script");
        else {
            printf("Executing script complete successful. Return status: %d\n", s);
            unlink(script_fn);
        }
    }

    return 0;
}