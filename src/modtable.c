// C stdlib headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Local headers
#include <loader.h>
#include <modtable.h>
#include <errors.h>

#define LIST_INIT_SIZE      8
#define GROWTH_FACTOR       2
#define FNV_PRIME           0x100000001b3
#define FNV_OFFSET          0xcbf29ce48422325UL

#define IS_NULL(_x, _y)     if((_x) == NULL) { _y; }

typedef struct {
    char **list;
    uint64_t length;
    uint64_t capacity;
} KeyList;

static KeyList keyData;

// Static functions
static uint64_t hash_fnv1a(const char *key) {
    uint64_t hashVal = FNV_OFFSET;
    for(uint64_t i = 0; key[i] != '\0'; i++) {
        hashVal ^= key[i];
        hashVal *= FNV_PRIME;
    }
    return hashVal;
}

static bool check_if_dup(Instance *checkInst, const void *key, size_t keySize) {
    if(checkInst->key == NULL) return false;
    while(checkInst != NULL) {
        if(strlen(checkInst->key) == keySize && memcmp(checkInst->key, key, keySize) == 0) return true;
        checkInst = checkInst->nextInst;
    }
    return false;
}

// Public functions
bool table_init(uint64_t tableSize, HashTable **table) {
    *table = malloc(sizeof(HashTable) + sizeof(Instance) * tableSize);
    IS_NULL(*table, return true)
    (*table)->tableSize = tableSize;
    (*table)->set = (Instance*) ((*table) + 1);
    for(uint64_t i = 0; i < tableSize; i++) {
        (*table)->set[i].key = NULL;
        (*table)->set[i].nextInst = NULL;
    }

    keyData.length = 0;
    keyData.capacity = LIST_INIT_SIZE;
    keyData.list = malloc(sizeof(char**) * keyData.capacity);
    IS_NULL(keyData.list, free(*table); return true)
    return false;
}

bool table_insert(HashTable *table, const char *key, ModuleInterface *interface, exec mainExec, get_err diagnostic, get_help helpData, char **errVal) {
    if(table == NULL || key == NULL || mainExec == NULL || diagnostic == NULL) {
        *errVal = NULL_ERR;
        return true;
    }

    if(keyData.length >= keyData.capacity) {
        char **tmp = realloc(keyData.list, keyData.capacity * GROWTH_FACTOR * sizeof(char*));
        IS_NULL(tmp, *errVal = MEM_ERR; return true)
        keyData.list = tmp;
        keyData.capacity *= GROWTH_FACTOR;
    }
    keyData.list[keyData.length] = malloc(strlen(key) + 1);
    IS_NULL(keyData.list[keyData.length], *errVal = MEM_ERR; return true)
    strcpy(keyData.list[keyData.length++], key);

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
    activeInst->helpData = helpData;
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

bool table_delete(HashTable *table, const char *key, ModuleInterface **retInterface) {
    if(table == NULL || key == NULL) return true;
    uint64_t index = hash_fnv1a(key) % table->tableSize;

    if(table->set[index].key == NULL) return true;
    uint64_t keySize = strlen(key);
    Instance *prevInst = NULL;
    Instance *tempInst = &table->set[index];
    while(tempInst != NULL && !(strlen(tempInst->key) == keySize && strcmp(tempInst->key, key) == 0)) {
        prevInst = tempInst;
        tempInst = tempInst->nextInst;
    }
    if(tempInst == NULL) return true;

    if(retInterface != NULL) *retInterface = tempInst->interface;
    free(tempInst->key);
    if(prevInst == NULL) {
        if(tempInst->nextInst == NULL) {
            tempInst->key = NULL;
            tempInst->interface = NULL;
            tempInst->nextInst = NULL;
        }
        else {
            Instance *oldInst = tempInst->nextInst;
            *tempInst = *oldInst;
            free(oldInst);
        }
    }
    else {
        prevInst->nextInst = tempInst->nextInst;
        free(tempInst);
    }
    return false;
}

void table_kill(HashTable **table, clear_inst termInter) {
    for(uint64_t i = 0; i < keyData.length; i++) {
        ModuleInterface *tempInterface = NULL;
        if(!table_delete(*table, keyData.list[i], &tempInterface)) {
            printf("Removing - %s\n", keyData.list[i]);
            termInter(tempInterface);
        }
        free(keyData.list[i]);
    }

    free(*table);
    free(keyData.list);
    *table = NULL;
    keyData.list = NULL;
}