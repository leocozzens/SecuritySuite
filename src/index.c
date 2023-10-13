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
#define INSERT_CMD(_cmd)		if(table_insert(modTable, STRINGIZE(_cmd), NULL, _cmd, get_op_err, errVal)) { \
								free(modDir.path); *errVal = OP_ERR; return true; }

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

bool index_init(char **errVal) {
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
	INSERT_CMD(LOAD); // TODO: Cleanup previous allocations if insert_cmd fails
	INSERT_CMD(UNLOAD);

	char *test = "test_module";
	Instance *loadInst = table_get(modTable, "load");
	loadInst->mainExec(1, &test);
	Instance *testInst = table_get(modTable, test);
	testInst->mainExec(0, NULL);

    return false;
}

char *index_get_path(const char *modName) {
	char *modPath = malloc(modDir.len + S_LEN(DEFAULT_MODULE_PATH) + strlen(modName) + S_LEN(EXTENSION) + 1);	
	local_stpcpy(local_stpcpy(local_stpcpy(local_stpcpy(modPath, modDir.path), DEFAULT_MODULE_PATH), modName), EXTENSION);
	return modPath;
}

ModuleInterface *index_remove_module(const char *modName) {
	return table_delete(modTable, modName);
}

bool index_store_mod(ModuleInterface *newMod, const char *modName, char **errVal) {
	return table_insert(modTable, modName, newMod, newMod->mod_exec, newMod->mod_get_err, errVal);
}

void index_cleanup(void) {
	table_kill(&modTable);
	free(modDir.path);
}