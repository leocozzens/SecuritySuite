// C stdlib headers
#include <stdio.h>
#include <stdlib.h>
// Local headers
#include <index.h>
#include <loader.h>

#define MEM_ERR             "Unable to allocate additional memory"
#define IS_NULL(_x, _y) if((_x) == NULL) { _y; }

typedef struct {
	ModuleInterface **list;
	unsigned int listLen;
	unsigned int listCapacity;
    unsigned int growthFactor;
} ModVals;

static ModVals modData;

bool index_init(unsigned int listInitSize, unsigned int newGrowthFactor, const char **errVal) {
	modData.listCapacity = listInitSize;
    modData.growthFactor = newGrowthFactor;
	modData.list = malloc(sizeof(ModuleInterface*) * listInitSize);
    IS_NULL(modData.list, *errVal = MEM_ERR; return true)
    return false;
}

bool index_attempt_load(const char *modName, const char **errVal) {
	ModuleInterface *newMod = load_module(modName, errVal);

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
    for(int i = 0; i < modData.listLen; i++) {
        // Unload all modules
    }
    free(modData.list);
}