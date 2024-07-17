#pragma once

inline void error(const char *msg) { 
    throw msg; 
}

class Stack {
	struct Link {
		char info;
		Link *pNext;
		Link(char x, Link *next) : info(x), pNext(next) {}
	};
	Link *pStack = nullptr;

public:
	Stack() = default;
	Stack(const Stack&) = delete;
	Stack& operator = (const Stack&) = delete;

	void push(char x) { 
        pStack = new Link(x, pStack); 
    }

	char pop() {
		if (!pStack) 
            error("Pop from empty stack");
		Link *link = pStack; 
        pStack = pStack->pNext; 
        char retval = link->info;
		delete link;
		return retval;
	}

	bool is_empty() {
        return pStack == nullptr; 
    }

	~Stack() { 
        while (!is_empty()) 
            pop(); 
    }
};