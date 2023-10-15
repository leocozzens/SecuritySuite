#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <stdbool.h>

#define LOAD    load
#define UNLOAD  unload
#define CLEAR   clear
#define DONE    done

#define BASE_OP(_name) bool _name(int argc, char **argv)

BASE_OP(LOAD);
BASE_OP(UNLOAD);
BASE_OP(CLEAR);
BASE_OP(DONE);
char *get_op_err(void);
char *get_last_op(void);
void set_run_var(bool *runVar);

#endif