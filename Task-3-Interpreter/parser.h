#pragma once

#include <iostream>
#include <stack>
#include <vector>
#include "scanner.h"
#include "scanner.cpp"

class Parser {
    Scanner scan;
    
    Lex curr_lex;
    type_of_lex c_type;
    int c_val;

    std::stack <Lex> st_assign; // стек объявленных идентификаторов, для проверки в =
    std::stack <type_of_lex> st_lex; // стек типов лексем, для проверки операций
    std::stack <int> st_labels; // стек для индексов меток
    std::stack <int> st_labels_position; // стек для хреанения позиций меток для заполнения полиза

    void S(); // Handles the "PROGRAM" construct

    void D(); // Handles declarations
    void I(); // Handles sequences of identifiers and constants

    void O(); // Handles operators
    void F(); // Handles factors
    void T(); // Handles terms
    void R(); // Handles relational expressions
    void A(); // Handles additive expressions
    void L(); // Handles logical expressions
    void E(); // Handles expressions

    /* была ли объявлена переменная ранее, если нет,
    то помечает ее как объявленную и устанавливает ее тип*/
    void dec(type_of_lex type, int i);

    /* проверяет, был ли объявлен идентификатор ранее,
    если да, то помещает его тип в стек */
    void check_id();

    /* анализирует типы операндов */
    void check_op();

    /* проверяет, были ли определены все необходимые метки */
    void check_labels();

    void print_poliz();

    void gl() {
        curr_lex = scan.get_lex();
        c_type = curr_lex.get_type();
        c_val = curr_lex.get_value();
    };

public:
    std::vector <Lex> poliz;
    Parser(const char* program) : scan(program) {};
    void analyze();
};                       