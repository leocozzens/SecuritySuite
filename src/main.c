// C stdlib headers
#include <stdio.h>
#include <stdlib.h>
// Local headers
#include <loader.h>
#include <index.h>
#include <input.h>

#define INPUT_SIZE			4096
#define EXIT_SUCCESS		0
#define EXIT_FAILURE 		1

#define PS1					">> "
#define INIT_ERR			"ERROR: Program initialization failed\n%s\n"
#define MODULE_ERR			"ERROR: Module error\n%s\n"

static char *errVal;
static char readBuff[INPUT_SIZE];

int main(void) {
	if(index_init(&errVal)) {
		fprintf(stderr, INIT_ERR, errVal);
		return EXIT_FAILURE;
	}

	bool running = true;
	int localArgc;
	char **localArgv;
	while(running) {
		printf(PS1);
		input_get(readBuff, INPUT_SIZE, stdin);
		localArgv = input_arglist(readBuff, &localArgc);
		if(index_call_module(localArgc, localArgv, &errVal))
			fprintf(stderr, MODULE_ERR, errVal);
		free(localArgv);
	}
	index_cleanup();
	return EXIT_SUCCESS;
}
