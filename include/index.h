#ifndef INDEX_H
#define INDEX_H

#include <stdint.h>
#include <stdbool.h>

typedef ModuleInterface _ModuleInterface;

bool index_init(char **errVal);
char *index_get_path(const char *modName);
bool index_call_module(int argc, char **argv, char **errVal);
bool index_store_mod(_ModuleInterface *newMod, const char *modName, char **errVal);
_ModuleInterface *index_remove_module(const char *modName);
void index_cleanup(void);

#endif