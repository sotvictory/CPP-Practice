#include <cstdio>
#include "stack.h"

int main()
{
	printf("Input string>\n");
	Stack s;
	int ch;

	while ((ch = getchar()) != EOF) {
		s.push(ch);
	}

	while (!s.is_empty()) {
		putchar(s.pop());
	}
    
	return 0;
}
