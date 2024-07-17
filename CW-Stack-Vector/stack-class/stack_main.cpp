#include <stdio.h>
#include "stack.h"

int main() {
	int a;
	Stack s;

	while ((a = getchar()) != EOF) {
		s.push(a);
	}

	//Stack saved = s;
	while (!s.is_empty()) {
		a = s.pop();
		putchar(a);
	}

	//printf("\n%c\n", saved.pop());
    
	return 0;
}