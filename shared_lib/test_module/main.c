// C stdlib headers
#include <stdio.h>
#include <stdbool.h>
// Local headers
#include <module.h>

bool mod_init(void) {
	printf("Init successful\n");
	return false;
}
bool mod_exec(int argc, char **argv) {
	printf("Execution successful\n");
	return false;
}
bool mod_status(void) {
	return false;
}

char *mod_get_help(void) {
	return NULL;
}

char *mod_get_err(void) {
	return NULL;
}
char *mod_get_ver(void) {
	return NULL;
}
void mod_cleanup(void) {
}
void mod_set_output(FILE *newStream) {

}