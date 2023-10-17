// C stdlib headers
#include <stdio.h>
#include <stdlib.h>
// Local headers
#include <operations.h>
#include <loader.h>
#include <index.h>
#include <errors.h>

#define EXPAND_STR(_arg)            #_arg
#define STRINGIZE(_arg)             EXPAND_STR(_arg)

#define USAGE(_str)                 "USAGE: " _str
#define LOAD_USAGE                  USAGE(STRINGIZE(LOAD) " [MODULE_NAMES...]")
#define UNLOAD_USAGE                USAGE(STRINGIZE(UNLOAD) " [MODULE_NAMES...]")
#define HELP_USAGE                  USAGE(STRINGIZE(HELP) " [MODULE NAMES...]")
#define CLEAR_USAGE                 USAGE(STRINGIZE(CLEAR))
#define DONE_USAGE                  USAGE(STRINGIZE(DONE))

#define DESCRIPTION(_str)           "DESCRIPTION: " _str
#define LOAD_DESC                   DESCRIPTION("Loads target module definitions into memory.\nTo use specify list of valid target module names contained within the environment addons directory.")
#define UNLOAD_DESC                 DESCRIPTION("Unloads target module definitions from memory.\nTo use specify list of valid target module names previously loaded into memory.")
#define CLEAR_DESC                  DESCRIPTION("Clears the standard output stream by printing the unix terminal code for clear.\nTakes no arguments.")
#define HELP_DESC                   DESCRIPTION("Looks in memory for help data related to target module.\nTo use specify a valid list of target modules previous loaded into memory.")
#define DONE_DESC                   DESCRIPTION("Clears memory and cleanly gracefully program.\nTakes no arguments")

#define CLEAR_CODE                  "\e[1;1H\e[2J"
#define RUNNING_CLEANUP             "Running cleanup...\n"

#define IS_NULL(_x, _y) 		    if((_x) == NULL) { _y; }
#define RET_ERR(_count, _trigger)   if(argc _trigger (_count)) { errVal = (argc < _count) ? FEW_ARG_ERR : MANY_ARG_ERR; return true; }
#define SET_LAST_OP(_operation)     lastOp = (STRINGIZE(_operation))
#define DIVIDE_LITERALS(_x, _y)     _x "\n" _y

#define ERR_BUFF_SIZE 256

static char *errVal = NULL;
static char errBuff[ERR_BUFF_SIZE]; // TODO: More in depth diagnostic information for help, unload and load fallback
static char *lastOp = NULL;
static bool *running = NULL;
// static FILE *outStream;

BASE_OP(LOAD) {
    SET_LAST_OP(LOAD);
    RET_ERR(2, <)
        for(uint64_t i = 1; i < argc; i++) {
        char *modPath = index_get_path(argv[i]);
        ModuleInterface *newMod = load_module(modPath, &errVal);
        free(modPath);
        IS_NULL(newMod, return true)
        if(index_store_mod(newMod, argv[i], &errVal)) {
            free(newMod);
            return true;
        }
        newMod->mod_init();
        fprintf(stdout, "Sucessfully loaded module - %s\n", argv[i]);
    }
    return false;
}

OP_HELP(LOAD_HELP) {
    return DIVIDE_LITERALS(LOAD_USAGE, LOAD_DESC);
}

BASE_OP(UNLOAD) {
    SET_LAST_OP(UNLOAD);
    RET_ERR(2, <)
    for(uint64_t i = 1; i < argc; i++) {
        ModuleInterface *targetInterface = NULL;
        if(index_remove_module(argv[i], &targetInterface)) {
            errVal = OBJ_ERR;
            return true;
        }
        if(targetInterface != NULL) {
            unload_module(targetInterface);
        }
        fprintf(stdout, "Sucessfully unloaded module - %s\n", argv[i]);
    }
    return false;
}

OP_HELP(UNLOAD_HELP) {
    return DIVIDE_LITERALS(UNLOAD_USAGE, UNLOAD_DESC);
}

BASE_OP(CLEAR) {
    SET_LAST_OP(CLEAR);
    RET_ERR(1, !=)
    printf(CLEAR_CODE);
    return false;
}

OP_HELP(CLEAR_HELP) {
    return DIVIDE_LITERALS(CLEAR_USAGE, CLEAR_DESC);
}

BASE_OP(HELP) { // TODO: 'help all' functionality
    SET_LAST_OP(HELP);
    RET_ERR(2, <)
    char *helpData;
    for(uint64_t i = 1; i < argc; i++) {
        helpData = index_get_helper(argv[i], &errVal);
        IS_NULL(helpData, return true)
        fprintf(stdout, "%s\n\n", helpData);
    }
    return false;
}

OP_HELP(HELP_HELP) {
    return DIVIDE_LITERALS(HELP_USAGE, HELP_DESC);
}

BASE_OP(DONE) {
    SET_LAST_OP(DONE);
    RET_ERR(1, !=)
    *running = false;
    printf(RUNNING_CLEANUP);
    return false;
}

OP_HELP(DONE_HELP) {
    return DIVIDE_LITERALS(DONE_USAGE, DONE_DESC);
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