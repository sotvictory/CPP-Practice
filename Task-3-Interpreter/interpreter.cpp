#include "interpreter.h"
#include "scanner.h"
 
void Executer::execute(std::vector<Lex>& poliz) {
    Lex pc_el, op1, op2;

    std::stack <Lex> args;
    std::stack <Lex> buf;
    std::string str;

    int i, index = 0, size = poliz.size();

    while (index < size) {
        pc_el = poliz[index];
        //std::cout << "poliz index: " << index << std::endl;
        //std::cout << "poliz pc_el: " << poliz[index] << std::endl;

        switch (pc_el.get_type()) {
            case LEX_TRUE: case LEX_FALSE: case LEX_NUM: case LEX_STR_CONST: case POLIZ_LABEL: case POLIZ_ADDRESS:
                args.push(pc_el);
                break;

            case LEX_ID:
                i = pc_el.get_value();
                if (TID[i].get_assign()) {
                    args.push(Lex(TID[i].get_type(), TID[i].get_value()));
                    break;
                } else {
                    throw std::invalid_argument("Indefinite identifier " + TID[i].get_name());
                }

            case LEX_SEMICOLON:
                while (!args.empty())
                    args.pop();
                break;
 
           case LEX_NOT:
                from_st(args, op1);
                args.push(Lex(LEX_BOOLEAN, !(op1.get_value())));
                break;
 
            case LEX_OR:
                from_st(args, op2);
                from_st(args, op1);
                args.push(Lex(LEX_BOOLEAN, (op1.get_value() || op2.get_value())));
                break;

            case LEX_AND:
                from_st(args, op2);
                from_st(args, op1);
                args.push(Lex(LEX_BOOLEAN, (op1.get_value() && op2.get_value())));
                break;
 
            case POLIZ_GO:
                from_st(args, op1);
                index = op1.get_value() - 1;
                break;

            case POLIZ_FGO:
                from_st(args, op2);
                from_st(args, op1);
                if (!(op1.get_value())) 
                    index = op2.get_value() - 1;
                break;

            case LEX_WRITE:
                i = pc_el.get_value();
                while (i != 0) {
                    from_st(args, op1);
                    buf.push(op1);
                    i--;
                }
                while (!buf.empty()) {
                    from_st(buf, op1);
                    if (op1.get_type() == LEX_STR_CONST or op1.get_type() == LEX_STRING){
                        std::cout << TCS[op1.get_value()];
                    } else if (op1.get_type() == LEX_BOOLEAN or op1.get_type() == LEX_TRUE or op1.get_type() == LEX_FALSE) {
                        //std::cout << "WRITE: LEX_BOOLEAN" << std::endl;
                        //std::cout << op1.get_value() << std::endl;
                        if (op1.get_value() == 1) {
                            std::cout << "true";
                        } else {
                            std::cout << "false";
                        }
                    } else {
                        std::cout << op1.get_value();
                    }
                }
                std::cout << std::endl;
                break;
 
            case LEX_READ:
                from_st(args, op1);
                if (TID[op1.get_value()].get_type() == LEX_INT) {
                    if (!(std::cin >> i))
                        throw std::invalid_argument("Incorrect input for int variable");
                } else {
                    std::cin >> str;
                    i = Scanner::put_str(str);
                }
                TID[op1.get_value()].put_value(i);
                TID[op1.get_value()].put_assign();
                break;

            case LEX_PLUS:
                from_st(args, op2);
                from_st(args, op1);
                if (op1.get_type() == LEX_STR_CONST or op1.get_type() == LEX_STRING) {
                    i = Scanner::put_str(TCS[op1.get_value()]+TCS[op2.get_value()]);
                    args.push(Lex(LEX_STRING, i));
                } else {
                    args.push(Lex(LEX_INT, op1.get_value() + op2.get_value()));
                }
                break;

            case LEX_PERCENT:
                from_st(args, op2);
                from_st(args, op1);
                args.push(Lex(LEX_INT, op1.get_value() % op2.get_value()));
                break;

            case LEX_MINUS:
                from_st(args, op2);
                from_st(args, op1);
                args.push(Lex(LEX_INT, op1.get_value() - op2.get_value()));
                break;

            case LEX_MUL:
                from_st(args, op2);
                from_st(args, op1);
                args.push(Lex(LEX_INT, op1.get_value() * op2.get_value()));
                break;

            case LEX_DIV:
                from_st(args, op2);
                from_st(args, op1);
                if (op2.get_value() == 0) {
                    throw std::invalid_argument("Divide by zero");
                }
                else args.push(Lex(LEX_INT, op1.get_value() / op2.get_value()));
                break;

            case LEX_EQEQ:
                from_st(args, op2);
                from_st(args, op1);
                if (op1.get_type() == LEX_STR_CONST or op1.get_type() == LEX_STRING) {
                    args.push(Lex(LEX_BOOLEAN, TCS[op1.get_value()] == TCS[op2.get_value()]));
                } else {
                    args.push(Lex(LEX_BOOLEAN, op1.get_value() == op2.get_value()));
                }
                break;

            case LEX_NEQ:
                from_st(args, op2);
                from_st(args, op1);
                if (op1.get_type() == LEX_STR_CONST or op1.get_type() == LEX_STRING) {
                    args.push(Lex(LEX_BOOLEAN, TCS[op1.get_value()] != TCS[op2.get_value()]));
                } else {
                    args.push(Lex(LEX_BOOLEAN, op1.get_value() != op2.get_value()));
                }
                break;

            case LEX_LESS:
                from_st(args, op2);
                from_st(args, op1);
                if (op1.get_type() == LEX_STR_CONST or op1.get_type() == LEX_STRING) {
                    args.push(Lex(LEX_BOOLEAN, TCS[op1.get_value()] < TCS[op2.get_value()]));
                } else {
                    args.push(Lex(LEX_BOOLEAN, op1.get_value() < op2.get_value()));
                }
                break;

            case LEX_MORE:
                from_st(args, op2);
                from_st(args, op1);
                if (op1.get_type() == LEX_STR_CONST or op1.get_type() == LEX_STRING) {
                    args.push(Lex(LEX_BOOLEAN, TCS[op1.get_value()] > TCS[op2.get_value()]));
                } else {
                    args.push(Lex(LEX_BOOLEAN, op1.get_value() > op2.get_value()));
                }
                break;

            case LEX_MOREEQ:
                from_st(args, op2);
                from_st(args, op1);
                args.push(Lex(LEX_BOOLEAN, op1.get_value() >= op2.get_value()));
                break;

            case LEX_LESSEQ:
                from_st(args, op2);
                from_st(args, op1);
                args.push(Lex(LEX_BOOLEAN, op1.get_value() <= op2.get_value()));
                break;

            case LEX_EQ:
                from_st(args, op2);
                from_st(args, op1);
                TID[op1.get_value()].put_value(op2.get_value());
                TID[op1.get_value()].put_assign();
                break;
 
            default:
                throw "POLIZ: unexpected elem";
        }
        ++index;
    };

    //std::cout << "Finish of executing!!!" << std::endl;
}

int main(int argc, char** argv) {
    try {
        Interpretator I(argv[1]);
        I.interpretation();
        return 0;
    } catch(const std::invalid_argument& e) {
        std::cout << e.what() << std::endl;
        return -1;
    }
}