// C stdlib headers
#include <stdio.h>
#include <stdbool.h>
// Local headers
#include <module.h>

bool mod_init(void) {
	return false;
}
bool modexec(int argc, char **argv) {
	return false;
}
bool mod_status(void) {
	return false;
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