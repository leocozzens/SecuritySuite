// C stdlib headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
#include <errors.h>

#define IS_NULL(_x, _y)			if((_x) == NULL) { _y; }
#define ERR_SIZE				256

#ifdef _WIN32
#define LOAD_SYMBOL(_var, _handle, _symbol)	_var = (void*) GetProcAddress((HMODULE) _handle, (LPCSTR) _symbol)
#define OPEN_LIB(_var, _libPath)			_var = (void*) LoadLibraryA(_libPath)
#else
#define LOAD_SYMBOL(_var, _handle, _symbol)	_var = dlsym(_handle, _symbol)
#define OPEN_LIB(_var, _libPath)			_var = dlopen(_libPath, RTLD_LAZY)
#endif

#define LOAD_FUNC(_interface, _var, _symbol, _errVal) \
LOAD_SYMBOL(_interface->_var, _interface->objectHandle, _symbol); \
IS_NULL(_interface->_var, *_errVal = FUNC_NOT_FOUND _symbol; free(_interface); return NULL)

ModuleInterface *load_module(const char *restrict modPath, char **errVal) {
	ModuleInterface *newMod = malloc(sizeof(ModuleInterface));
	IS_NULL(newMod, *errVal = MEM_ERR; return NULL)

	OPEN_LIB(newMod->objectHandle, modPath);
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
	if(newMod->objectHandle == NULL) {
		char *error = dlerror();
		free(newMod);
		if(error == NULL) {
			static char errorMessage[ERR_SIZE];
			sprintf(errorMessage, "%s%s", LIB_ERR, modPath);
			errorMessage[ERR_SIZE - 1] = '\0';
			*errVal = errorMessage;
			return NULL;
		}
		char *afterPath = strrchr(error, '/') + 1;
 		*errVal = (afterPath == NULL) ? error : afterPath;
		return NULL;
	}
	#endif

	LOAD_FUNC(newMod, mod_init, MOD_INIT, errVal);
	LOAD_FUNC(newMod, mod_exec, MOD_EXEC, errVal);
	LOAD_FUNC(newMod, mod_status, MOD_STATUS, errVal);
	LOAD_FUNC(newMod, mod_get_help, MOD_GET_HELP, errVal);
	LOAD_FUNC(newMod, mod_get_err, MOD_GET_ERR, errVal);
	LOAD_FUNC(newMod, mod_get_ver, MOD_GET_VER, errVal);
	LOAD_FUNC(newMod, mod_cleanup, MOD_CLEANUP, errVal);
	LOAD_FUNC(newMod, mod_set_output, MOD_SET_OUTPUT, errVal);
	return newMod;
}

void unload_module(ModuleInterface *targetMod) {
	if(targetMod == NULL) return;
	targetMod->mod_cleanup();
	dlclose(targetMod->objectHandle);
	free(targetMod);
}