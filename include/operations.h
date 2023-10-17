#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <stdbool.h>

#define LOAD        load
#define UNLOAD      unload
#define CLEAR       clear
#define HELP        help
#define DONE        done

#define LOAD_HELP   load_help
#define UNLOAD_HELP unload_help
#define CLEAR_HELP  clear_help
#define HELP_HELP   help_help
#define DONE_HELP   done_help

#define BASE_OP(_name) bool _name(int argc, char **argv)
#define OP_HELP(_name) char *_name(void)

BASE_OP(LOAD);
OP_HELP(LOAD_HELP);

BASE_OP(UNLOAD);
OP_HELP(UNLOAD_HELP);

BASE_OP(CLEAR);
OP_HELP(CLEAR_HELP);

BASE_OP(HELP);
OP_HELP(HELP_HELP);

BASE_OP(DONE);
OP_HELP(DONE_HELP);
 // TODO: Add set output func
char *get_op_err(void);
char *get_last_op(void);
void set_run_var(bool *runVar);

#endif