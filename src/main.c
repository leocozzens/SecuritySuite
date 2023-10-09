// C stdlib headers
#include <stdio.h>
#include <stdbool.h>
// Local headers
#include <input.h>
#include <loader.h>

#define INPUT_SIZE 256
#define PS1		   ">> "

static void attempt_load(const char *modName) {
	if(load_module(modName)) printf("Could not locate module - %s\n", modName);
	else printf("Loaded module - %s", modName);
}

int main(void) {
	char buffer[INPUT_SIZE];
	while(true) {
		printf(PS1);
		input_get(buffer, INPUT_SIZE, stdin);
		attempt_load(buffer);
	}
	return 0;
}
