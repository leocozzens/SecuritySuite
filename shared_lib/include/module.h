#ifndef MODULE_H
#define MODULE_H

typedef FILE FILE_DEF;

bool mod_init(void); // Consider passing in the file directory for the module
bool mod_exec(int argc, char **argv);
bool mod_status(void);
char *mod_get_help(void);
char *mod_get_err(void);
char *mod_get_ver(void);
void mod_cleanup(void);
void mod_set_output(FILE_DEF *newStream);

#endif