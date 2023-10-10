// C stdlib headers
#include <stdio.h>
// Local headers
#include <index.h>
#include <input.h>

#define INPUT_SIZE			4096
#define GROWTH_FACTOR		2
#define LIST_INIT_SIZE		1
#define EXIT_SUCCESS		0
#define EXIT_FAILURE 		1

#define PS1					">> "
#define INIT_ERR			"ERROR: Program initialization failed\n%s\n"
#define MODULE_CREATE_ERR	"ERROR: Module creation failed\n%s\n"

static const char *errVal;
static char readBuff[INPUT_SIZE];

int main(void) {
	if(index_init(LIST_INIT_SIZE, GROWTH_FACTOR, &errVal)) {
		fprintf(stderr, INIT_ERR, errVal);
		return EXIT_FAILURE;
	}

	bool running = true;
	while(running) {
		printf(PS1);
		input_get(readBuff, INPUT_SIZE, stdin);
		if(index_attempt_load(readBuff, &errVal)) 
			fprintf(stderr, MODULE_CREATE_ERR, errVal);
		else
			printf("Loaded module - %s\n", readBuff);
	}
	index_cleanup();
	return EXIT_SUCCESS;
}
