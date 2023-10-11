// C stdlib headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stpcpy.h>
#include <stdbool.h>
// POSIX headers
#ifdef _WIN32
#include <windows.h>
#include <libloaderapi.h>
#include <errhandlingapi.h>
#else
#include <dlfcn.h>
#endif
// Local headers
#include <loader.h>

#define MEM_ERR					"Unable to allocate additional memory"
#define FUNC_NOT_FOUND			"Failed to load module symbol - "
#define DEFAULT_MODULE_PATH		"./addons/"
#define IS_NULL(_x, _y)			if((_x) == NULL) { _y; }
#define S_LEN(_string)			(sizeof(_string) - 1)

#ifdef _WIN32
#define EXTENSION							".dll"
#define ERR_SIZE							256
#define LIB_ERR								"Load Library failed to load - "

#define LOAD_SYMBOL(_var, _handle, _symbol)	_var = GetProcAddress((HMODULE) _handle, (LPCSTR) _symbol)
#define OPEN_LIB(_var, _libPath)			_var = (void*) LoadLibraryA(_libPath)
#else
#define EXTENSION							".so"

#define LOAD_SYMBOL(_var, _handle, _symbol)	_var = dlsym(_handle, _symbol)
#define OPEN_LIB(_var, _libPath)			_var = dlopen(_libPath, RTLD_LAZY)
#endif

#define LOAD_FUNC(_interface, _var, _symbol, _errVal) \
LOAD_SYMBOL(_interface->_var, _interface->objectHandle, _symbol); \
IS_NULL(_interface->_var, *_errVal = FUNC_NOT_FOUND _symbol; free(_interface); return NULL)

ModuleInterface *load_module(const char *modName, const char **errVal) {
	ModuleInterface *newMod = malloc(sizeof(ModuleInterface));

	const char *modDir = DEFAULT_MODULE_PATH;
	IS_NULL(newMod, *errVal = MEM_ERR; return NULL)

	char *modPath = malloc(strlen(modDir) + strlen(modName) + S_LEN(EXTENSION) + 1);
	local_stpcpy(local_stpcpy(local_stpcpy(modPath, DEFAULT_MODULE_PATH), modName), EXTENSION);

	OPEN_LIB(newMod->objectHandle, modPath);
	free(modPath);
	#ifdef _WIN32
	if(newMod->objectHandle == NULL) {
		DWORD errorCode = GetLastError();
		static char errorMessage[ERR_SIZE];
		
		DWORD formatResult = FormatMessageA(
			FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			errorCode,
			0,
			errorMessage,
			sizeof(errorMessage),
			NULL
		);
		*errVal = errorMessage;
		char *newLine;
		if(formatResult == 0) sprintf(errorMessage, "%s%s", LIB_ERR, modName);
		else if((newLine = strrchr(errorMessage, '\n')) != NULL) *newLine = '\0';
		return NULL;
	}
	#else
	char *afterPath = strrchr(dlerror(), '/') + 1;
	IS_NULL(newMod->objectHandle, *errVal = (afterPath == NULL) ? dlerror() : afterPath; free(newMod); return NULL)
	#endif

	LOAD_FUNC(newMod, mod_init, MOD_INIT, errVal);
	LOAD_FUNC(newMod, mod_exec, MOD_EXEC, errVal);
	LOAD_FUNC(newMod, mod_status, MOD_STATUS, errVal);
	LOAD_FUNC(newMod, mod_get_err, MOD_GET_ERR, errVal);
	LOAD_FUNC(newMod, mod_get_ver, MOD_GET_VER, errVal);
	LOAD_FUNC(newMod, mod_cleanup, MOD_CLEANUP, errVal);
	LOAD_FUNC(newMod, mod_set_output, MOD_SET_OUTPUT, errVal);
	return newMod;
}