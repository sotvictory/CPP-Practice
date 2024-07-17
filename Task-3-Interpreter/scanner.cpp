#include <cctype>
#include "scanner.h"

std::vector<std::string> Scanner::TW = {
    "", "program", "int", "string", "if", "else", // 0-5
    "do", "while", "read", "write", "not",        // 6-10
    "and", "or", "goto", "boolean", "true",       // 11-15
    "false", "break"                              // 16-17
};

std::vector<std::string> Scanner::TD = {
    "@", "==", "{", "}", "=", "+", // 0-5
    "-", "%", ",", "!=", "(",      // 6-10
    ")", "*", "/", "<", ">",       // 11-15
    ">=", "<=", ";", ":", "."      // 16-20
};

Lex Scanner::get_lex() {
    enum state{ H, IDENT, NUMB, SLSH, COM_ST, COM_FN, NEQ, ALE, STR };

    int d, j;
    std::string buf;
    state CS = H;

    do {
        gc();
        switch(CS) {
            case H:
                if (feof(fp)) {
                    return Lex(LEX_FIN);
                } else if (!isspace(c)) {
                    if (isalpha(c)) {
                        buf.push_back(c);
                        CS = IDENT;
                    } else if (isdigit(c)) {
                        d = c - '0';
                        CS = NUMB;
                    } else if (c == '/') {
                        buf.push_back(c);
                        CS = SLSH;
                    } else if (c == '!') {
                        buf.push_back(c);
                        CS = NEQ;
                    } else if (c == '=' || c == '<' || c == '>') {
                        buf.push_back(c);
                        CS = ALE;
                    } else if (c == '"') {
                        CS = STR;
                    } else {
                        buf.push_back(c);
                        if ((j = look(buf, TD))) {
                            return Lex((type_of_lex)(j + (int)LEX_FIN), j);
                        } else {
                            throw std::invalid_argument("Unknown operator " + buf);
                        }
                    }
                }
                break;

            case IDENT:
                if (isalpha(c) || isdigit(c)) {
                    buf.push_back(c);
                } else {
                    while(isspace(c))
                        gc();
                    if (c == ':') {
                        j = put_label(buf);
                        return Lex((type_of_lex)LEX_LABEL, j);
                    }
                    ungetc(c,fp);
                    if ((j = look(buf, TW))) {
                        return Lex((type_of_lex)j, j);
                    } else {
                        j = put_ident(buf);
                        return Lex((type_of_lex)LEX_ID, j);
                    }
                }
                break;

            case NUMB:
                if (isdigit(c)) {
                    d = d * 10 + (c - '0');
                } else if (isalpha(c)) {
                    throw std::invalid_argument("Unexpected symbol in NUMB");
                } else {
                    ungetc(c,fp);
                    return Lex(LEX_NUM, d);
                }
                break;

            case SLSH:
                if (c == '*') {
                    buf.pop_back();
                    CS = COM_ST;
                } else {
                    ungetc(c,fp);
                    j = look(buf, TD);
                    return Lex((type_of_lex)(j + (int)LEX_FIN), j);
                }
                break;
            
            case COM_ST:
                if (c == '*') {
                    CS = COM_FN;
                } else if (feof(fp)) {
                    throw std::invalid_argument("Unexpected end of file in COM_ST");
                }
                break;

            case COM_FN:
                if (c == '/') {
                    CS = H;
                } else if (feof(fp)) {
                    throw std::invalid_argument("Unexpected end of file in COM_FN");
                } else if (c != '*') {
                    CS = COM_ST;
                }
                break;
            
            case NEQ: 
                if (c == '=') {
                    buf.push_back(c);
                    j = look(buf, TD);
                    return Lex((type_of_lex)(j + (int)LEX_FIN), j);
                } else {
                    throw std::invalid_argument("Unexpected symbol after '!'");
                }
                break;
            
            case ALE: 
                if (c == '=') {
                    buf.push_back(c);
                    j = look(buf, TD);
                    return Lex((type_of_lex)(j + (int)LEX_FIN), j);
                } else {
                    ungetc(c, fp);
                    j = look(buf, TD);
                    return Lex((type_of_lex)(j + (int)LEX_FIN), j);
                }
                break;
            
            case STR:
                if (c == '"') {
                    std::string res = "";
                    for (size_t i = 0; i < buf.size(); ++i)
                        res += buf[i];
                    j = put_str(res);
                    return Lex((type_of_lex)LEX_STR_CONST, j);
                } else if (feof(fp)) {
                    throw std::invalid_argument("Unexpected end of file in STR");
                }
                buf.push_back(c);
                break;
        }
    } while (1);
};