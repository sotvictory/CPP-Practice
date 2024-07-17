#pragma once

#include <iostream>
#include <algorithm>
#include <stack>
#include <string>
#include <vector>

enum type_of_lex {
    LEX_NULL,       // 0
    LEX_PROGRAM,    // 1
    LEX_INT,        // 2
    LEX_STRING,     // 3
    LEX_IF,         // 4
    LEX_ELSE,       // 5
    LEX_DO,         // 6
    LEX_WHILE,      // 7
    LEX_READ,       // 8
    LEX_WRITE,      // 9
    LEX_NOT,        // 10
    LEX_AND,        // 11
    LEX_OR,         // 12
    LEX_GOTO,       // 13
    LEX_BOOLEAN,    // 14
    LEX_TRUE,       // 15
    LEX_FALSE,      // 16
    LEX_BREAK,      // 17
    LEX_FIN,        // 18
    LEX_EQEQ,       // 19
    LEX_LBRACE,     // 20
    LEX_RBRACE,     // 21
    LEX_EQ,         // 22
    LEX_PLUS,       // 23
    LEX_MINUS,      // 24
    LEX_PERCENT,    // 25
    LEX_COMMA,      // 26
    LEX_NEQ,        // 27
    LEX_LBRACKET,   // 28
    LEX_RBRACKET,   // 29
    LEX_MUL,        // 30
    LEX_DIV,        // 31
    LEX_LESS,       // 32
    LEX_MORE,       // 33
    LEX_MOREEQ,     // 34
    LEX_LESSEQ,     // 35
    LEX_SEMICOLON,  // 36
    LEX_COLON,      // 37
    LEX_DOT,        // 38
    LEX_NUM,        // 39
    LEX_STR_CONST,  // 40
    LEX_LABEL,      // 41
    POLIZ_ADDRESS,  // 42
    POLIZ_LABEL,    // 43
    POLIZ_GO,       // 44
    POLIZ_FGO,      // 45
    LEX_ID          // 46
};

static const char* str[] = {
    "LEX_NULL", "LEX_PROGRAM", "LEX_INT", "LEX_STR", "LEX_IF", "LEX_ELSE", "LEX_DO", "LEX_WHILE", "LEX_READ", "LEX_WRITE",
    "!", "&&", "||", "LEX_GOTO", "LEX_BOOLEAN", "true", "false", "LEX_BREAK", "LEX_FIN",
    "==", "{", "}", "=", "+", "-", "%", "LEX_COMMA", "!=", "LEX_LBRACKET", "LEX_RBRACKET",
    "*", "/", "<", ">", ">=", "<=", ";", "LEX_COLON", "LEX_DOT", "int", "str", "label",
    "ID addr", "LABEL addr", "unconditional jump !", "jump false !F", "ID",
};

class Lex {
    type_of_lex t_lex;
    int v_lex;

public:
    Lex(type_of_lex t = LEX_NULL, int v = 0) { t_lex = t; v_lex = v; }

    type_of_lex get_type() const { return t_lex; }

    int get_value() const { return v_lex; }

    void put_value(int i) { v_lex = i; }

    friend std::ostream& operator << (std::ostream& os, Lex L);
};

std::ostream& operator << (std::ostream& os, Lex L) {
    os << "(" << L.t_lex << "," << L.v_lex << ")";
    return os;
};

class Ident {
    std::string name;
    bool declare;
    type_of_lex type;
    bool assign;
    int value;

public:
    Ident() { declare = false; assign = false; };

    Ident(const std::string& n) {
        name = n;
        declare = false;
        assign = false;
    };

    bool operator == (const Ident& other) const { return name == other.name; }

    bool operator == (const std::string& s) const { return name == s; }

    std::string get_name() const { return name; };
    
    bool get_declare() const{ return declare; };

    bool get_assign() const { return assign; };

    int get_value() const { return value; };

    type_of_lex get_type() const { return type; };

    void put_declare() { declare = true; };
    
    void put_type(type_of_lex t) { type = t; };

    void put_assign() { assign = true; };
    
    void put_value(int v) { value = v; };
};

std::vector <Ident> TID, TOL;
std::vector <std::string> TCS;

class Scanner {
    FILE* fp;
    char c;

    int look(const std::string& buf, std::vector<std::string> list) {
        size_t i = 0;

        while (i != list.size()) {
            if (buf == list[i]) 
                return i;
            i++;
        }

        return 0;
    }

    void gc() { c = fgetc(fp); }
    bool check() { return std::cin.eof(); }

public:
    static std::vector<std::string> TW, TD;

    Lex get_lex();

    Scanner(const char* program) {
        if ((fp = fopen(program, "r")) == 0)
            throw std::invalid_argument("Can't open file");
    }

    ~Scanner() { pclose(fp); }

    template <typename T>
    static int put_element(std::vector<T>& container, const T& element) {
        typename std::vector<T>::iterator it;

        if ((it = std::find(container.begin(), container.end(), element)) != container.end())
            return it - container.begin();

        container.push_back(element);
        return container.size() - 1;
    }

    int put_ident(const std::string& buf) { return put_element(TID, Ident(buf)); }
    int put_label(const std::string& buf) { return put_element(TOL, Ident(buf)); }
    static int put_str(const std::string& buf) { return put_element(TCS, buf); }

    void print_TID() {
        for (Ident i: TID)
            std::cout << i.get_name() << std::endl;
    }

    void print_TOL() {
        for (Ident i: TOL)
            std::cout << i.get_name() <<std::endl;
    }

    void print_TCS() {
        for (std::string i: TCS)
            std::cout << i << std::endl;
    }
};