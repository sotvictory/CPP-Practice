#if !defined(__STACKjasdajsofkajsdljaosgoaisjdoiafd_H__)
#define __STACKjasdajsofkajsdljaosgoaisjdoiafd_H__

#define N 1024

struct Stack {
	char body[N];
	int top;
};

extern void push(struct Stack *t, int x);
extern char pop(struct Stack *t);
extern int is_empty(struct Stack *t);
extern int is_full(struct Stack *t);
extern int size(struct Stack *t);
extern void init(struct Stack *t);

extern int a;

#endif
