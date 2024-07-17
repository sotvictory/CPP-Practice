#include <stdio.h>
#define N 1024

struct Stack {
	char body[N];
	int top;
};

void push(struct Stack *t, int x) { t->body[t->top++] = x; }
char pop(struct Stack *t) { return t->body[--t->top]; }
int is_empty(struct Stack *t) { return t->top == 0; }
int is_full(struct Stack *t) { return t->top == N; }
int size(struct Stack *t) { return t->top; }
void init(struct Stack *t) { t->top = 0; }

int main() {
	struct Stack s;
	init(&s);
	int ch;

	while ((ch = getchar()) != EOF) {
		push(&s, ch);
	}

	while (!is_empty(&s)) {
		putchar(pop(&s));
	}

	return 0;
}