#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <stdbool.h>

#define LOAD    load
#define UNLOAD  unload
#define CLEAR   clear

bool LOAD(int argc, char **argv);
bool UNLOAD(int argc, char **argv);
bool CLEAR(int argc, char **argv);
char *get_op_err(void);
char *get_last_op(void);

#endif