#ifndef MODTABLE_H
#define MODTABLE_H

#include <stdint.h>
#include <stdbool.h>

typedef ModuleInterface _ModuleInterface;
typedef bool (*exec)(int argc, char **argv);
typedef char *(*get_err)(void);

typedef struct _Instance {
    char *key;
    ModuleInterface *interface;
    exec mainExec;
    get_err diagnostic;
    struct _Instance *nextInst;
} Instance;

typedef bool (*clear_inst)(_ModuleInterface *targetInter);

typedef struct {
    uint64_t tableSize;
    Instance *set;
} HashTable;

bool table_init(uint64_t tableSize, HashTable **table);
bool table_insert(HashTable *table, const char *key, _ModuleInterface *interface, exec mainExec, get_err diagnostic, char **errVal);
Instance *table_get(HashTable *table, const char *key);
_ModuleInterface *table_delete(HashTable *table, const char *key);
void table_kill(HashTable **table, clear_inst termInst);

#endif