#ifndef INDEX_H
#define INDEX_H

#include <stdint.h>
#include <stdbool.h>

typedef ModuleInterface _ModuleInterface;

bool index_init(char **errVal, bool *running);
char *index_get_path(const char *modName);
bool index_call_module(int argc, char **argv, char **currMod, char **errVal);
bool index_store_mod(_ModuleInterface *newMod, const char *modName, char **errVal);
bool index_remove_module(const char *modName, _ModuleInterface **retInterface);
char *index_get_helper(char *modName, char **errVal);
void index_cleanup(void);

#endif