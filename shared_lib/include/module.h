#ifndef MODULE_H
#define MODULE_H

typedef struct _IO_FILE FILE;

bool mod_init(void);
bool modexec(int argc, char **argv);
bool mod_status(void);
char *mod_get_err(void);
char *mod_get_ver(void);
void mod_cleanup(void);
void mod_set_output(FILE *newStream);

#endif