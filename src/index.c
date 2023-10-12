// C stdlib headers
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
// Local headers
#include <index.h>
#include <loader.h>
#include <stpcpy.h>
#include <dirfinder.h>

#define MEM_ERR            		"Unable to allocate additional memory"
#define DIR_ERR					"Failed to locate executable path"
#define DEFAULT_MODULE_PATH		"addons/"
#define ERR_LEN					256
#define IS_NULL(_x, _y) 		if((_x) == NULL) { _y; }
#define S_LEN(_string)			(sizeof(_string) - 1)

#ifdef _WIN32
#define EXTENSION				".dll"
#else
#define EXTENSION				".so"
#endif

typedef struct {
	ModuleInterface **list;
	uint32_t listLen;
	uint32_t listCapacity;
    uint32_t growthFactor;
} ModVals;

typedef struct {
	char *path;
	uint64_t len;
} ModDir;

static ModVals modData;
static ModDir modDir;

bool index_init(uint32_t listInitSize, uint32_t newGrowthFactor, const char **errVal) {
	static char errStr[ERR_LEN];
	const char *dirErr;
	modDir.path = dir_get_exec(&dirErr);
	IS_NULL(modDir.path, sprintf(errStr, "%s - %s", DIR_ERR, dirErr); *errVal = errStr; return true)
	modDir.len = strlen(modDir.path);

	modData.listCapacity = listInitSize;
    modData.growthFactor = newGrowthFactor;
	modData.list = malloc(sizeof(ModuleInterface*) * listInitSize);
    IS_NULL(modData.list, *errVal = MEM_ERR; return true)
    return false;
}

bool index_attempt_load(const char *modName, const char **errVal) {	
	char *modPath = malloc(modDir.len + S_LEN(DEFAULT_MODULE_PATH) + strlen(modName) + S_LEN(EXTENSION) + 1);	
	local_stpcpy(local_stpcpy(local_stpcpy(local_stpcpy(modPath, modDir.path), DEFAULT_MODULE_PATH), modName), EXTENSION);

	ModuleInterface *newMod = load_module(modPath, errVal);
	free(modPath);
	IS_NULL(newMod, return true)
	if((modData.listLen + 1) > modData.listCapacity) {
		ModuleInterface **tmp = realloc(modData.list, sizeof(ModuleInterface*) * (modData.listCapacity * modData.growthFactor));
		IS_NULL(tmp, *errVal = MEM_ERR; return true)
		else {
			modData.list = tmp;
			modData.listCapacity *= modData.growthFactor;
			modData.listLen++;
		}
	}
	newMod->mod_init();
    return false;
}

void index_cleanup(void) {
    for(uint32_t i = 0; i < modData.listLen; i++) {
        // Unload all modules
    }
    free(modData.list);
	free(modDir.path);
}