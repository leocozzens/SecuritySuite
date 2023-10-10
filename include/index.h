#ifndef INDEX_H
#define INDEX_H

#include <stdbool.h>

bool index_init(unsigned int listInitSize, unsigned int newGrowthFactor, const char **errVal);
bool index_attempt_load(const char *modName, const char **errVal);

#endif