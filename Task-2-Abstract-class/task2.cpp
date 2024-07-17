#include <iostream>
#include <cstring>

class Iterable {
public:
    virtual int GetLength() const = 0;
    virtual char *Next() = 0;
    virtual char *Reset() = 0;
    virtual ~Iterable() {}
};

class String : public Iterable {
    char *str, *cur;
public:
    String() : str(nullptr), cur(nullptr) {}

    String(const char *s) : cur(nullptr) {
        int length = strlen(s);
        str = new char[length + 1];
        strcpy(str, s);
    }

    String(const String& other) : cur(nullptr) {
        int length = strlen(other.str);
        str = new char[length + 1];
        strcpy(str, other.str);
    }

    ~String() {
        if (str != nullptr) {
            delete[] str;
            str = nullptr;
        }
    }

    String& operator = (const String& other) {
        if (this != &other) {
            if (str != nullptr) {
                delete[] str;
            }
            int length = strlen(other.str);
            str = new char[length + 1];
            strcpy(str, other.str);
            cur = nullptr;
        }
        return *this;
    }

    friend std::ostream& operator << (std::ostream& os, const String& s) {
        if (s.str != nullptr) {
            os << s.str;
        }
        return os;
    }

    int GetLength() const {
        return strlen(str);
    }

    char *Next() {
        if (cur == nullptr) {
            cur = str;
            return cur++;
        } else {
            if (*cur != '\0') {
                return cur++;
            }
        }
        cur = nullptr;
        return nullptr;
    }

    char *Reset() {
        cur = nullptr;
        return cur;
    }

    char& operator [] (int index) {
        if (index >= 0 && index < static_cast<int>(strlen(str))) {
            return str[index];
        } else {
            throw std::out_of_range("Index out of bounds");
        }
    }
};

class List : public Iterable {
    struct Node {
        char elem;
        Node *next;

        Node (char elem = '_', Node *next = nullptr) {
			this->elem = elem;
			this->next = next;
       }
    };
    Node *lst, *cur;
public:
    List () : lst(nullptr), cur(nullptr) {}

    List (const char *s) : cur(nullptr) {
        if (*s == '\0') {
            lst = nullptr;
            return;
        }
        Node *p;
        lst = p = new Node(*s++);
				
        while (*s != '\0') {
            p = p->next = new Node(*s++);
        }
    }

    List(const List& other) : lst(nullptr), cur(nullptr) {
        if (other.lst) {
            Node *p = other.lst;
            Node *q = lst = new Node(p->elem);
            p = p->next;

            while (p) {
                q = q->next = new Node(p->elem);
                p = p->next;
            }
        }
    }

    int GetLength() const {
        int length = 0;
        Node *p = lst;
        while (p) {
            length++;
            p = p->next;
        }
        return length;
    }

    char *Next() {
        if (cur == nullptr) {
            cur = lst;
        } else {
            if (cur->next != nullptr) {
                cur = cur->next;
            } else {
                cur = nullptr;
                return nullptr;
            }
        }
        return &(cur->elem);
    }

    char *Reset() {
        cur = nullptr;
        return nullptr;
    }

    void clear() {
		Node *p;
		while (lst != NULL) {
			p = lst;
			lst = lst->next;
			delete p;
		}
	}

	~List() {
		this->clear();
    }

	List& operator = (const List& l) {
        Node *p,*q;
        
        if (&l == this) {
                return *this;
        }
        
        this->clear();

        if (l.lst == nullptr) {
		 	return *this;
        }
        p = lst = new Node(l.lst->elem);
        q = l.lst->next;
        while (q != nullptr) {
		 	p->next = new Node(q->elem);
		 	p = p->next;
		 	q = q->next;
        }

		return *this;
	}

    friend std::ostream& operator << (std::ostream& os, const List& l) {
		if (l.lst == nullptr) {
			os << "";
			return os;
		}

		Node *p = l.lst;
		while (p != nullptr) {  
			os << p->elem;
			p = p->next;
		}
		return os;
	}
};

int main() {
    // Testing String class
    String s1("Hello");
    String s2 = s1; // cc
    String s3;
    s3 = s1;

    std::cout << "s1: " << s1 << std::endl;
    std::cout << "s2: " << s2 << std::endl;
    std::cout << "s3: " << s3 << std::endl;

    std::cout << "Length of s1: " << s1.GetLength() << std::endl;

    std::cout << "Characters of s1: ";
    char *p;
    while ((p = s1.Next()) != nullptr) {
        std::cout << *p << " ";
    }
    std::cout << std::endl;

    p = s1.Next();
    std::cout << *p << " ";
    s1.Reset();
    while ((p = s1.Next()) != nullptr) {
        std::cout << *p << " ";
    }
    std::cout << std::endl;    

    try {
        std::cout << "Character at index 1 in s1: " << s1[1] << std::endl;
        // Test out of bounds access
        std::cout << "Character at index 10 in s1: " << s1[10] << std::endl;
    } catch (const std::out_of_range& e) {
        std::cerr << e.what() << std::endl;
    }

    // Testing List class
    List l1("World");
    List l2 = l1;
    List l3;
    l3 = l1;

    std::cout << "l1: " << l1 << std::endl;
    std::cout << "l2: " << l2 << std::endl;
    std::cout << "l3: " << l3 << std::endl;

    std::cout << "Length of l1: " << l1.GetLength() << std::endl;

    std::cout << "Characters of l1: ";
    char *q;
    while ((q = l1.Next()) != nullptr) {
        std::cout << *q << " ";
    }
    std::cout << std::endl;

    return 0;
}