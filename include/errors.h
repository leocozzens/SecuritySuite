#ifndef ERRORS_H
#define ERRORS_H

#define MEM_ERR             "Unable to allocate additional memory"
#define INPUT_MEM_ERR       "Unable to allocate sufficient memory to contain user input"
#define INPUT_STREAM_ERR    "Input stream closed"
#define FEW_ARG_ERR         "Insufficient arguments"
#define MANY_ARG_ERR        "Excessive arguments"
#define DIR_ERR             "Failed to locate executable path"
#define OP_ERR              "Failed to load initial operations"
#define NULL_ERR            "Attempted to store null pointer in table"
#define DUP_ERR             "Tool identifier already exists"
#define NONE_ERR            "Tool identifier does not exist"
#define OBJ_ERR             "Object not found"
#define UNLISTED_ERR        "Unlisted error / Error not provided by tool"
#define CALL_MODULE_ERR     "Failure to call module"
#define HELP_ERR            "No help function associated with module"
#define HELP_NONE           "Module helper failed to produce valid data"

#define LIB_ERR             "Failed to load module - "
#define FUNC_NOT_FOUND      "Failed to load module symbol - "

#endif