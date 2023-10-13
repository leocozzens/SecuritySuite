#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <stdbool.h>

#define LOAD    load
#define UNLOAD  unload

char *get_op_err(void);
bool LOAD(int argc, char **argv);
bool UNLOAD(int argc, char **argv);

#endif