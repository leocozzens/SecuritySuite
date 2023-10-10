// C stdlib headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
// POSIX headers
#ifdef _WIN32

#else
#include <dlfcn.h>
#endif
// Local headers
#include <loader.h>

#define MEM_ERR					"Unable to allocate additional memory"
#define FUNC_NOT_FOUND			"Failed to load module symbol - "
#define DEFAULT_MODULE_PATH		"./addons/"
#define IS_NULL(_x, _y)			if((_x) == NULL) { _y; }

#define LOAD_FUNC(_interface, _var, _symbol, _errVal) \
_interface->_var = dlsym(_interface->objectHandle, _symbol); \
IS_NULL(_interface->_var, *_errVal = FUNC_NOT_FOUND _symbol; free(_interface); return NULL)

ModuleInterface *load_module(const char *modName, const char **errVal) {
	ModuleInterface *newMod = malloc(sizeof(ModuleInterface));

	const char *modDir = DEFAULT_MODULE_PATH;
	IS_NULL(newMod, *errVal = MEM_ERR; return NULL)

	char *modPath = malloc(strlen(modDir) + strlen(modName) + 1);
	stpcpy(stpcpy(modPath, DEFAULT_MODULE_PATH), modName);

	newMod->objectHandle = dlopen(modPath, RTLD_LAZY);
	free(modPath);
	char *afterPath = strrchr(dlerror(), '/') + 1;
	IS_NULL(newMod->objectHandle, *errVal = (afterPath == NULL) ? dlerror() : afterPath; free(newMod); return NULL)

	LOAD_FUNC(newMod, mod_init, MOD_INIT, errVal);
	LOAD_FUNC(newMod, mod_exec, MOD_EXEC, errVal);
	LOAD_FUNC(newMod, mod_status, MOD_STATUS, errVal);
	LOAD_FUNC(newMod, mod_get_err, MOD_GET_ERR, errVal);
	LOAD_FUNC(newMod, mod_get_ver, MOD_GET_VER, errVal);
	LOAD_FUNC(newMod, mod_cleanup, MOD_CLEANUP, errVal);
	LOAD_FUNC(newMod, mod_set_output, MOD_SET_OUTPUT, errVal);
	return newMod;
}