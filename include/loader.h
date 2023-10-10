#ifndef LOADER_H
#define LOADER_H

#include <stdbool.h>

#define MOD_INIT            "mod_init"
#define MOD_EXEC            "mod_exec"
#define MOD_STATUS          "mod_status"
#define MOD_GET_ERR         "mod_get_err"
#define MOD_GET_VER         "mod_get_ver"
#define MOD_CLEANUP         "mod_cleanup"
#define MOD_SET_OUTPUT      "mod_set_output"

typedef struct _IO_FILE     FILE;

typedef struct {
    char *objectTitle;
    void *objectHandle;
    bool (*mod_init)        (void);
    bool (*mod_exec)        (int argc, char **argv);
    bool (*mod_status)      (void);
    char *(*mod_get_err)    (void);
    char *(*mod_get_ver)    (void);
    void (*mod_cleanup)     (void);
    void (*mod_set_output)  (FILE *newStream);
} ModuleInterface;

ModuleInterface *load_module(const char *modName, const char **errVal);

#endif