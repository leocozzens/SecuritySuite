// C stdlib headers
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
// Local headers
#include <loader.h>
#include <stpcpy.h>
#include <dirfinder.h>
#include <modtable.h>
#include <operations.h>
#include <errors.h>

#define DEFAULT_MODULE_PATH		"addons/"
#define ERR_LEN					256
#define TABLE_SIZE 				1<<15
#define IS_NULL(_x, _y) 		if((_x) == NULL) { _y; }
#define S_LEN(_string)			(sizeof(_string) - 1)	// Compile-time string length evaluation for known literals
#define STRINGIZE(_x)			#_x
#define REMOVE_FUNCTION(_func)	table_delete(modTable, STRINGIZE(_func), NULL)
#define REMOVE_FUNCTIONS		REMOVE_FUNCTION(LOAD); REMOVE_FUNCTION(UNLOAD); REMOVE_FUNCTION(HELP); REMOVE_FUNCTION(CLEAR); REMOVE_FUNCTION(DONE)
#define INSERT_CMD(_cmd, _help) if(table_insert(modTable, STRINGIZE(_cmd), NULL, _cmd, get_op_err, _help, errVal)) { \
								free(modDir.path); *errVal = OP_ERR; REMOVE_FUNCTIONS; return true; }

#ifdef _WIN32
#define EXTENSION				".dll"
#else
#define EXTENSION				".so"
#endif

typedef struct {
	char *path;
	uint64_t len;
} ModDir;

static ModDir modDir;
static HashTable *modTable;

bool index_init(char **errVal, bool *running) {
	set_run_var(running);
	static char errStr[ERR_LEN];
	const char *dirErr;
	modDir.path = dir_get_exec(&dirErr);
	IS_NULL(modDir.path, sprintf(errStr, "%s - %s", DIR_ERR, dirErr); *errVal = errStr; return true)
	modDir.len = strlen(modDir.path);

	if(table_init(TABLE_SIZE, &modTable)) {
		free(modDir.path);
		*errVal = MEM_ERR;
		return true;
	}
	INSERT_CMD(LOAD, LOAD_HELP);
	INSERT_CMD(UNLOAD, UNLOAD_HELP);
	INSERT_CMD(HELP, HELP_HELP);
	INSERT_CMD(CLEAR, CLEAR_HELP);
	INSERT_CMD(DONE, DONE_HELP);

    return false;
}

char *index_get_path(const char *modName) {
	char *modPath = malloc(modDir.len + S_LEN(DEFAULT_MODULE_PATH) + strlen(modName) + S_LEN(EXTENSION) + 1);	
	local_stpcpy(local_stpcpy(local_stpcpy(local_stpcpy(modPath, modDir.path), DEFAULT_MODULE_PATH), modName), EXTENSION);
	return modPath;
}

bool index_call_module(int argc, char **argv, char **currMod, char **errVal) {
	Instance *targetInst = table_get(modTable, argv[0]);
	IS_NULL(targetInst, *currMod = CALL_MODULE_ERR; *errVal = NONE_ERR; return true)
	if(targetInst->mainExec(argc, argv)) {
		char *lastOp = (targetInst->interface == NULL) ? get_last_op() : targetInst->interface->mod_get_ver();
		*currMod = (lastOp == NULL) ? argv[0] : lastOp;

		char *diagnostic = targetInst->diagnostic();
		*errVal = (diagnostic == NULL) ? UNLISTED_ERR : diagnostic;
		return true;
	}
	return false;
}

char *index_get_helper(char *modName, char **errVal) {
	Instance *targetInst = table_get(modTable, modName);
	IS_NULL(targetInst, *errVal = NONE_ERR; return NULL)
	IS_NULL(targetInst->helpData, *errVal = HELP_ERR; return NULL)
	char *helpData = targetInst->helpData();
	return (helpData == NULL) ? *errVal = HELP_NONE, NULL : helpData;
}

bool index_remove_module(const char *modName, ModuleInterface **retInterface) {
	return table_delete(modTable, modName, retInterface);
}

bool index_store_mod(ModuleInterface *newMod, const char *modName, char **errVal) {
	return table_insert(modTable, modName, newMod, newMod->mod_exec, newMod->mod_get_err, newMod->mod_get_help, errVal);
}

void index_cleanup(void) {
	table_kill(&modTable, unload_module);
	free(modDir.path);
}