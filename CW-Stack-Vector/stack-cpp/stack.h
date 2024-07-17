#if !defined(__STACKjasdajsofkajsdljaosgoaisjdoiafd_H__)
#define __STACKjasdajsofkajsdljaosgoaisjdoiafd_H__

#define N 1024

class Stack {
	char body[N];
	int top = -1;

public:
	//Stack() { top = 0; }
	void push(int x) { body[++top] = x; }
	char pop() { return body[top--]; }
	int is_empty() { return top == -1; }
	int is_full() { return top == N - 1;  }
	int size() { return top + 1; }
};

#endif
