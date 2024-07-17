#include "stack.h"

void push(struct Stack *t, int x) { t->body[t->top++] = x; }
char pop(struct Stack *t) { return t->body[--t->top]; }
int is_empty(struct Stack *t) { return t->top == 0; }
int is_full(struct Stack *t) { return t->top == N; }
int size(struct Stack *t) { return t->top; }
void init(struct Stack *t) { t->top = 0; }
