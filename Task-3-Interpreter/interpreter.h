#include "parser.h"
#include "parser.cpp"

class Executer {
public:
    void execute (std::vector<Lex>& poliz);
};

class Interpretator {
    Parser pars;
    Executer E;
public:
    Interpretator(const char* program): pars(program) {};
    void interpretation();
};

void Interpretator::interpretation () {
    pars.analyze();
    E.execute(pars.poliz);
}