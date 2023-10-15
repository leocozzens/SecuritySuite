// C stdlib headers
#include <stdio.h>
#include <stdlib.h>
// Local headers
#include <operations.h>
#include <loader.h>
#include <index.h>
#include <errors.h>

#define EXPAND_STR(_arg)        #_arg
#define STRINGIZE(_arg)         EXPAND_STR(_arg)
#define LOAD_USAGE              " - USAGE: " STRINGIZE(LOAD) " [MODULE_NAME]"
#define UNLOAD_USAGE            " - USAGE: " STRINGIZE(UNLOAD) " [MODULE_NAME]"
#define CLEAR_USAGE             " - USAGE: " STRINGIZE(CLEAR)
#define DONE_USAGE              " - USAGE: " STRINGIZE(DONE)

#define CLEAR_CODE              "\e[1;1H\e[2J"
#define RUNNING_CLEANUP         "Running cleanup...\n"

#define IS_NULL(_x, _y) 		if((_x) == NULL) { _y; }
#define RET_ERR(_count, _usage) if(argc != (_count)) { errVal = (argc < 1) ? FEW_ARG_ERR _usage : MANY_ARG_ERR _usage; return true; }

static char *errVal = NULL;
static char *lastOp = NULL;
static bool *running = NULL;

BASE_OP(LOAD) {
    lastOp = STRINGIZE(LOAD);
    RET_ERR(1, LOAD_USAGE)
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

BASE_OP(UNLOAD) {
    lastOp = STRINGIZE(UNLOAD);
    RET_ERR(1, UNLOAD_USAGE)
    ModuleInterface *targetInterface;
    if(index_remove_module(argv[0], &targetInterface)) {
        errVal = OBJ_ERR;
        return true;
    }
    if(targetInterface != NULL) {
        unload_module(targetInterface);
    }
    return false;
}

BASE_OP(CLEAR) {
    lastOp = STRINGIZE(CLEAR);
    RET_ERR(0, CLEAR_USAGE)
    printf(CLEAR_CODE);
    return false;
}

BASE_OP(DONE) {
    lastOp = STRINGIZE(DONE);
    RET_ERR(0, DONE_USAGE)
    *running = false;
    printf(RUNNING_CLEANUP);
    return false;
}

char *get_op_err(void) {
    return errVal;
}

char *get_last_op(void) {
    return lastOp;
}

void set_run_var(bool *runVar) {
    running = runVar;
}