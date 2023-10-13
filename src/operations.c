// C stdlib headers
#include <stdio.h>
#include <stdlib.h>
// Local headers
#include <operations.h>
#include <loader.h>
#include <index.h>
#include <errors.h>

#define IS_NULL(_x, _y) 		if((_x) == NULL) { _y; }
static char *errVal;

bool LOAD(int argc, char **argv) {
    if(argc != 1) {
        errVal = ARG_ERR;
        return true;
    }
	char *modPath = index_get_path(argv[0]);
	ModuleInterface *newMod = load_module(modPath, &errVal);
	free(modPath);
	IS_NULL(newMod, return true)
    if(index_store_mod(newMod, argv[0], &errVal)) {
        free(newMod);
        return true;
    }
	newMod->mod_init();
    return false;
}

bool UNLOAD(int argc, char **argv) {
    if(argc != 1) {
        errVal = ARG_ERR;
        return true;
    }
    if(unload_module(index_remove_module(argv[0]))) {
        errVal = NONE_ERR;
        return true;
    }
    return false;
}

char *get_op_err(void) {
    return errVal;
}