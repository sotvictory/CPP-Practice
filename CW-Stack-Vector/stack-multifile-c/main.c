#include <stdio.h>
#include "stack.h"

int main() {
	struct Stack s;
	init(&s);
	int ch;

	while ((ch = getchar()) != -1) {
		push(&s, ch);
	}

	while (!is_empty(&s)) {
		putchar(pop(&s));
	}
    
	return 0;
}
