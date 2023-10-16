// C stdlib headers
#include <stdio.h>
#include <stdlib.h>
// Local headers
#include <loader.h>
#include <index.h>
#include <input.h>

#define INPUT_SIZE			256
#define INPUT_GROWTH_FACTOR 2
#define EXIT_SUCCESS		0
#define EXIT_FAILURE 		1

#define PS1					">> "
#define EXITING				"Cleanup finished\nExiting program...\n"
#define INIT_ERR			"ERROR: Program initialization failed\n%s\n"
#define INPUT_ERR			"ERROR: Failed to receive user input\n%s\n"
#define MODULE_ERR			"ERROR: %s - %s\n"

#define IS_NULL(_x, _y)     if((_x) == NULL) { _y; }

static char *errVal;
static char *currMod;
static char *readBuff;

int main(void) {
	bool running = true;
	if(index_init(&errVal, &running) || input_init(INPUT_SIZE, INPUT_GROWTH_FACTOR, &errVal)) {
		fprintf(stderr, INIT_ERR, errVal);
		return EXIT_FAILURE;
	}

	int localArgc;
	char **localArgv;
	while(running) {
		printf(PS1);
		readBuff = input_get(stdin, &errVal);
		IS_NULL(readBuff, fprintf(stderr, INPUT_ERR, errVal); continue)
		localArgv = input_arglist(readBuff, &localArgc);
		if(index_call_module(localArgc, localArgv, &currMod, &errVal))
			fprintf(stderr, MODULE_ERR, currMod, errVal);
		free(localArgv);
	}
	index_cleanup();
	input_cleanup();
	printf(EXITING);
	return EXIT_SUCCESS;
}
