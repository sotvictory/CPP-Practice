#include <iomanip>
#include "parser.h"

void Parser::analyze() {
    gl();
    S();
    gl();
    if (c_type != LEX_FIN) {
        throw std::invalid_argument("Incorrect program end");
    } else {
        print_poliz();
        std::cout << std::endl;
    }
};

// Handles the "PROGRAM" construct
void Parser::S() {
    if (c_type == LEX_PROGRAM) {
        gl();
        if (c_type != LEX_LBRACE) {
            throw std::invalid_argument("No '{' after 'program'");
        } else {
            gl();
            D();
        }
        if (c_type != LEX_RBRACE)
            throw std::invalid_argument("No '}' for 'program'");
        check_labels();
    } else {
        throw std::invalid_argument("No PROGRAM");
    }
};

// Handles declarations
void Parser::D() {
    if (c_type == LEX_INT || c_type == LEX_STRING || c_type == LEX_BOOLEAN) {
        type_of_lex type = c_type;
        st_lex.push(type);
        gl();
        if (c_type == LEX_ID) {
            int index = curr_lex.get_value();
            dec(type, index);
            st_assign.push(curr_lex);
            gl();
            I();
        } else {
            throw std::invalid_argument("No identifier");
        }
    } else {
        while (c_type != LEX_RBRACE)
            O();
    }
};

// Handles sequences of identifiers and constants
void Parser::I() {
    int curr_id = st_assign.top().get_value();
    type_of_lex type = st_lex.top();
    if (c_type == LEX_EQ) {
        TID[curr_id].put_assign();
        gl();
        if (c_type == LEX_NUM || c_type == LEX_STR_CONST || c_type == LEX_TRUE || c_type == LEX_FALSE) {
            if ((type == LEX_INT && c_type != LEX_NUM) || (type == LEX_STRING && c_type != LEX_STR_CONST) || (type == LEX_BOOLEAN && (c_type != LEX_TRUE && c_type != LEX_FALSE)))
                   throw std::invalid_argument("Invalid constant type");
            // TODOOO
            if (type == LEX_BOOLEAN) {
                    if (c_val == 15) {
                        c_val = 1;
                    } else if (c_val == 16) {
                        c_val = 0;
                    }
            }
            ///
            TID[curr_id].put_value(c_val);
            //std::cout << "value =" << c_val << std::endl;
            gl();
            I();
        } else {
            throw std::invalid_argument("No value for constant");
        }
    } else if (c_type == LEX_COMMA) {
        gl();
        if (c_type == LEX_ID) {
            int index = curr_lex.get_value();
            dec(type, index);
            st_assign.push(curr_lex);
            gl();
            I();
        } else {
            throw std::invalid_argument("No identifier");
        }
    } else if (c_type == LEX_SEMICOLON) {
        st_lex.pop();
        st_assign.pop();
        gl();
        D();
    } else { 
        throw std::invalid_argument("Unexpected operator");
    }
};

// Handles operators
void Parser::O() { // IF (a > b)
    if (c_type == LEX_IF) {
        gl();
        if (c_type != LEX_LBRACKET)
            throw std::invalid_argument("No '(' in IF");
        gl();
        E();
        type_of_lex value;
        value = st_lex.top();
        if (value != LEX_BOOLEAN)
            throw std::invalid_argument("Not BOOLEAN in IF statement");
        poliz.push_back(Lex(POLIZ_LABEL, 0));
        int fgo = poliz.size() - 1;
        poliz.push_back(Lex(POLIZ_FGO));
        if (c_type != LEX_RBRACKET)
            throw std::invalid_argument("No ')' in IF");
        gl();
        O();
        if (c_type == LEX_ELSE) {
            poliz.push_back(Lex(POLIZ_LABEL, 0));
            int go = poliz.size() - 1;
            poliz.push_back(Lex(POLIZ_GO));
            poliz[fgo].put_value(poliz.size());
            gl();
            O();
            poliz[go].put_value(poliz.size());
        } else {
            poliz[fgo].put_value(poliz.size());
        }
    } else if (c_type == LEX_READ) {
        gl();
        if (c_type != LEX_LBRACKET)
            throw std::invalid_argument("No '(' in IF");
        gl();
        if (c_type != LEX_ID)
            throw std::invalid_argument("No ID for READ");
        check_id();
        int index = curr_lex.get_value();
        if (TID[index].get_type() == LEX_BOOLEAN)
                throw std::invalid_argument("BOOLEAN in READ");
        TID[index].put_assign();
        poliz.push_back(Lex(POLIZ_ADDRESS, index));
        gl();
        if (c_type != LEX_RBRACKET)
            throw std::invalid_argument("No ')' in IF");
        poliz.push_back(Lex(LEX_READ));
        gl();
        if (c_type != LEX_SEMICOLON)
            throw std::invalid_argument("No ';'");
        gl();
    } else if (c_type == LEX_WHILE) {
        gl();
        if (c_type != LEX_LBRACKET)
            throw std::invalid_argument("No '(' in IF");
        int go = poliz.size();        
        gl();
        E();
        type_of_lex value;
        value = st_lex.top();        
        if (value != LEX_BOOLEAN){
            throw std::invalid_argument("Invalid operand");
        }
        poliz.push_back(Lex(POLIZ_LABEL, 0));
        int fgo = poliz.size() - 1;
        poliz.push_back(Lex(POLIZ_FGO));
        if (c_type != LEX_RBRACKET)
            throw std::invalid_argument("No ')' in IF");
        gl();
        O();
        poliz.push_back(Lex(POLIZ_LABEL, go));
        poliz.push_back(Lex(POLIZ_GO));
        poliz[fgo].put_value(poliz.size());
    } else if (c_type == LEX_WRITE) {
        gl();
        if (c_type != LEX_LBRACKET)
            throw std::invalid_argument("No '(' in IF");
        gl();
        E();
        int args_cnt = 1;
        while (c_type == LEX_COMMA) {
            gl();
            E();
            args_cnt++;
        }
        if (c_type != LEX_RBRACKET)
            throw std::invalid_argument("No ')' in IF");
        poliz.push_back(Lex(LEX_WRITE, args_cnt));
        gl();
        if (c_type != LEX_SEMICOLON)
            throw std::invalid_argument("No ;");
        gl();
    } else if (c_type == LEX_LABEL) {
        TOL[c_val].put_value(poliz.size());
        gl();
        O();
    } else if (c_type == LEX_GOTO) {
        gl();
        if (c_type != LEX_ID)
            throw std::invalid_argument("No LABEL for GOTO");
        st_labels.push(curr_lex.get_value());
        poliz.push_back(Lex(POLIZ_LABEL, 0));
        st_labels_position.push(poliz.size() - 1);
        poliz.push_back(Lex(POLIZ_GO));
        gl();
        if (c_type != LEX_SEMICOLON)
            throw std::invalid_argument("No ';'");
        gl();
    } else if (c_type == LEX_LBRACE) {
        gl();
        while (c_type != LEX_RBRACE)
            O();
        gl();
    } else if (c_type != LEX_RBRACE) {
        E();
        if (c_type != LEX_SEMICOLON)
            throw std::invalid_argument("No ';'");
        poliz.push_back(Lex(LEX_SEMICOLON));
        gl();
    }
}

// Handles factors
void Parser::F() {
    if (c_type == LEX_LBRACKET) {
        gl();
        E();
        if (c_type != LEX_RBRACKET)
            throw std::invalid_argument("No ')'");
        gl();
    } else if (c_type == LEX_ID || c_type == LEX_NUM || c_type == LEX_STR_CONST || c_type == LEX_TRUE || c_type == LEX_FALSE) {
        if (c_type == LEX_ID) {
            check_id();
            st_lex.push(LEX_ID);
            poliz.push_back(curr_lex);
        } else if (c_type == LEX_NUM) {
            st_lex.push(LEX_INT);
            poliz.push_back(curr_lex);
        } else if (c_type == LEX_STR_CONST) {
            st_lex.push(LEX_STRING);
            poliz.push_back(curr_lex);
        } else if (c_type == LEX_TRUE || c_type == LEX_FALSE) {
            st_lex.push(LEX_BOOLEAN);
            poliz.push_back(curr_lex);
        }
        gl();
    } else if (c_type == LEX_NOT) { // NOT a = true; LEX_BOOLEAN
        gl();
        F();
        type_of_lex value, prev;
        prev = st_lex.top();
        if (prev == LEX_ID)
            st_lex.pop();
        value = st_lex.top();
        if (value != LEX_BOOLEAN)
            throw std::invalid_argument("Invalid operand");
        poliz.push_back(Lex(LEX_NOT));
    } else {
        throw std::invalid_argument("Unknown operator in FACTOR");
    }
};

// Handles terms
void Parser::T() {
    F();
    while (c_type == LEX_MUL || c_type == LEX_DIV) {
        st_lex.push(c_type);
        gl();
        T();
        check_op();
    }
};

// Handles relational expressions
void Parser::R() {
    T();
    while (c_type == LEX_PLUS || c_type == LEX_MINUS || c_type == LEX_PERCENT) {
        st_lex.push(c_type);
        gl();
        R();
        check_op();
    }
};

// Handles additive expressions
void Parser::A() {
    R();
    if (c_type == LEX_LESS || c_type == LEX_MORE || c_type == LEX_LESSEQ || c_type == LEX_MOREEQ || c_type == LEX_EQEQ || c_type == LEX_NEQ) {
        st_lex.push(c_type);
        gl();
        A();
        check_op();
    }
};

// Handles logical expressions
void Parser::L() {
    A();
    while (c_type == LEX_OR || c_type == LEX_AND) {
        st_lex.push(c_type);
        gl();
        L();
        check_op();
    }
};

// a = b;
// a + b = c;

// Handles expressions
void Parser::E() {
    L();

    if (c_type == LEX_EQ) {
        type_of_lex prev;
        prev = st_lex.top();
        if (prev != LEX_ID)
            throw std::invalid_argument("Is not lvalue");
        st_lex.pop();
        st_lex.push(c_type);
        Lex last_id;
        last_id = poliz.back();
        poliz.pop_back();
        poliz.push_back(Lex(POLIZ_ADDRESS, last_id.get_value()));
        gl();
        E();
        check_op();
    } 
    type_of_lex prev;
    prev = st_lex.top();

    if (prev == LEX_ID)
        st_lex.pop();
};

////////////////////////////////////////////////////////

template <class T, class T_EL>
void from_st ( T & st, T_EL & i ) {
    i = st.top(); 
    st.pop();
}

void Parser::dec(type_of_lex type, int i) {
    if (TID[i].get_declare()) {
        throw std::invalid_argument("Identifier was declared twice");
    } else {
        TID[i].put_declare();
        TID[i].put_type(type);
    }
}

void Parser::check_id() {
    if (TID[c_val].get_declare())
        st_lex.push(TID[c_val].get_type());
    else 
        throw std::invalid_argument("Not declared");
}

void Parser::check_op() {
    type_of_lex op1, op2, op;
    type_of_lex lex_1 = LEX_NULL, lex_2 = LEX_NULL;

    lex_2 = st_lex.top();
    if (lex_2 == LEX_ID)
        st_lex.pop();

    from_st(st_lex, op2);
    from_st(st_lex, op);
    
    lex_1 = st_lex.top();
    if (lex_1 == LEX_ID)
        st_lex.pop();
    from_st(st_lex, op1);

    if (op == LEX_PLUS or op == LEX_MINUS or op == LEX_PERCENT) {
        if (op1 == LEX_BOOLEAN or op2 == LEX_BOOLEAN)
            throw std::invalid_argument("Invalid operand type");
        if (op1 != op2)
            throw std::invalid_argument("Different types of operands");
        st_lex.push(op1);
    } else if (op == LEX_MUL or op == LEX_DIV) {
        if (op1 != LEX_INT or op2 != LEX_INT)
            throw std::invalid_argument("Invalid operand type");
        st_lex.push(op1);
    } else if (op == LEX_LESSEQ or op == LEX_MOREEQ) {
        if (op1 != LEX_INT or op2 != LEX_INT)
            throw std::invalid_argument("Invalid operand type");
        st_lex.push(LEX_BOOLEAN);
    } else if (op == LEX_EQEQ or op == LEX_NEQ or op == LEX_LESS or op == LEX_MORE) {
        if (op1 == LEX_BOOLEAN or op2 == LEX_BOOLEAN)
            throw std::invalid_argument("Invalid operand type");
        if (op1 != op2)
            throw std::invalid_argument("Different types of operands");
        st_lex.push(LEX_BOOLEAN);
    } else if (op == LEX_AND or op == LEX_OR) {
        if (op1 != LEX_BOOLEAN or op2 != LEX_BOOLEAN)
            throw std::invalid_argument("Invalid operand type");
        st_lex.push(LEX_BOOLEAN);
    } else if (op == LEX_EQ) {
        if (op1 == LEX_INT and op2 == LEX_INT) { 
            st_lex.push(LEX_INT);
        } else if (op1 == LEX_STRING and op2 == LEX_STRING) { 
            st_lex.push(LEX_STRING);
        } else if (op1 == LEX_BOOLEAN and op2 == LEX_BOOLEAN) { 
            st_lex.push(LEX_BOOLEAN);
        } else {
            throw std::invalid_argument("Different types of operands");}
    }

    poliz.push_back(Lex(op));
};

int find_label(const std::string& buf) { 
    std::vector <Ident>::iterator iter; 

    if ((iter = std::find(TOL.begin(), TOL.end(), buf)) != TOL.end()) 
        return (*iter).get_value(); // POLIZ position
    else
        return -1; 
}

void Parser::check_labels() {
    int ind, poliz_pos, res;
    std::string name;

    while (st_labels.empty() == 0) {
        from_st(st_labels, ind);
        from_st(st_labels_position, poliz_pos);

        name = TID[ind].get_name();
        res = find_label(name);

        if (res == -1)
            throw std::invalid_argument("Undefined label " + name);

        poliz[poliz_pos].put_value(res);
    }
};

void Parser::print_poliz() {
    std::vector <Lex>::iterator iter = poliz.begin();
    int index = 0;

    while (iter != poliz.end()){
        std::cout << std::left << index << " " << str[(*iter).get_type()] << ' ';
        if ((*iter).get_type() == LEX_ID) {
            std::cout << TID[(*iter).get_value()].get_name();
        } else if ((*iter).get_type() == LEX_NUM) {
            std::cout << (*iter).get_value();
        } else if ((*iter).get_type() == LEX_STR_CONST) {
            std::cout << '"' << TCS[(*iter).get_value()] << '"';
        } else if ((*iter).get_type() == POLIZ_LABEL) {
            std::cout << (*iter).get_value();
        } else if ((*iter).get_type() == POLIZ_ADDRESS) {
            std::cout << TID[(*iter).get_value()].get_name();
        } else if ((*iter).get_type() == LEX_WRITE) {
            std::cout << (*iter).get_value();
        }
        std::cout << std::endl;
        iter++;
        index++;
    };
};