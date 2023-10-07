// C stdlib headers
#include <stdio.h>
#include <stdbool.h>
// Local headers
#include <input.h>

#define INPUT_SIZE 256

int main(void) {
	while(true) {
		char buffer[INPUT_SIZE];
		input_get(buffer, INPUT_SIZE, stdin);

		printf("your string is - %s\n", buffer);
	}
	return 0;
}
