// C stdlib headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Local headers
#include <loader.h>
#include <modtable.h>
#include <errors.h>

#define IS_NULL(_x, _y)     if((_x) == NULL) { _y; }

#define FNV_PRIME 0x100000001b3
#define FNV_OFFSET 0xcbf29ce48422325UL

static uint64_t hash_fnv1a(const char *key) {
    uint64_t hashVal = FNV_OFFSET;
    for(uint64_t i = 0; key[i] != '\0'; i++) {
        hashVal ^= key[i];
        hashVal *= FNV_PRIME;
    }
    return hashVal;
}

static bool check_if_dup(Instance *checkInst, const void *key, size_t keySize) {
    Instance *tempInst = checkInst;
    while(tempInst != NULL) {
        if(strlen(tempInst->key) == keySize && memcmp(checkInst->key, key, keySize) == 0) return true;
        tempInst = tempInst->nextInst;
    }
    return false;
}

bool table_init(uint64_t tableSize, HashTable **table) {
    *table = malloc(sizeof(HashTable) + sizeof(Instance) * tableSize);
    IS_NULL(*table, return true)
    (*table)->tableSize = tableSize;
    (*table)->set = (Instance*) (*table) + 1;
    for(uint64_t i = 0; i < tableSize; i++) {
        (*table)->set[i].key = NULL;
        (*table)->set[i].nextInst = NULL;
    }
    return false;
}

bool table_insert(HashTable *table, const char *key, ModuleInterface *interface, exec mainExec, get_err diagnostic, char **errVal) {
    if(table == NULL || key == NULL || mainExec == NULL || diagnostic == NULL) {
        *errVal = NULL_ERR;
        return true;
    }
    uint64_t index = hash_fnv1a(key) % table->tableSize;
    Instance *activeInst;
    char *newKey = malloc(strlen(key) + 1);
    IS_NULL(newKey, *errVal = MEM_ERR; return true)
    strcpy(newKey, key);

    if(table->set[index].key == NULL)
        activeInst = &table->set[index];
    else {
        if(check_if_dup(&table->set[index], key, strlen(key))) {
            free(newKey);
            *errVal = DUP_ERR;
            return true;
        }
        activeInst = malloc(sizeof(Instance));
        IS_NULL(activeInst, free(newKey); *errVal = MEM_ERR; return true)
        activeInst->nextInst = table->set[index].nextInst;
        table->set[index].nextInst = activeInst;
    }

    activeInst->key = newKey;
    activeInst->interface = interface;    
    activeInst->mainExec = mainExec;
    activeInst->diagnostic = diagnostic;
    return false;
}

Instance *table_get(HashTable *table, const char *key) {
    if(table == NULL || key == NULL) return NULL;
    uint64_t index = hash_fnv1a(key) % table->tableSize;

    if(table->set[index].key == NULL) return NULL;
    uint64_t keySize = strlen(key);
    Instance *tempInst = &table->set[index];
    while(tempInst != NULL && !(strlen(tempInst->key) == keySize && strcmp(tempInst->key, key) == 0)) {
        tempInst = tempInst->nextInst;
    }

    return tempInst;
}

ModuleInterface *table_delete(HashTable *table, const char *key) {

    return NULL;
}

void table_kill(HashTable **table) {

    free(*table);
    *table = NULL;
}
