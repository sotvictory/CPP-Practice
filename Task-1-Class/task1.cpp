#include <iostream>
#include <cstring>
#include <cstdlib>

class StrNum {
private:
    char *data;
public:
    StrNum() : data(nullptr) {}
    
    StrNum(const char *str) {
        int length = strlen(str);
        data = new char[length + 1];
        strcpy(data, str);
    }
    
    StrNum(const StrNum& other) {
        int length = strlen(other.data);
        data = new char[length + 1];
        strcpy(data, other.data);
    }
    
    ~StrNum() {
        if (data != nullptr) {
            delete[] data;
            data = nullptr;
        }
    }
    
    StrNum& operator = (const StrNum& other) {
        if (this != &other) {
            if (data != nullptr) {
                delete[] data;
            }
            int length = strlen(other.data);
            data = new char[length + 1];
            strcpy(data, other.data);
        }
        return *this;
    }
    
    StrNum operator + (const StrNum& other) {
        int totalLength = strlen(data) + strlen(other.data);
        char *tmp = new char[totalLength + 1];
        strcpy(tmp, data);
        strcat(tmp, other.data);
        
        StrNum result(tmp);
        delete[] tmp;
        
        return result;
    }

    bool isNumber(const char *str) const {
        int dotCount = 0;
        int startIndex = 0;

        if (str[0] == '-') {
            startIndex = 1;
        }

        for (int i = startIndex; str[i] != '\0'; i++) {
            if (!isdigit(str[i])) {
                if (str[i] == '.' && dotCount == 0) {
                    dotCount++;
                } else {
                    return false;
                }
            }
        }

        return true;
    }

    bool operator == (const StrNum& other) const {
        if (isNumber(data) && isNumber(other.data)) {
            return atof(data) == atof(other.data);
        } else {
            return strcmp(data, other.data) == 0;
        }
    }
    
    bool operator < (const StrNum& other) const {
        if (isNumber(data) && isNumber(other.data)) {
            return atof(data) < atof(other.data);
        } else {
            return strcmp(data, other.data) < 0;
        }
    }

    bool operator > (const StrNum& other) const {
        if (isNumber(data) && isNumber(other.data)) {
            return atof(data) > atof(other.data);
        } else {
            return strcmp(data, other.data) > 0;
        }
    }
    
    template <typename T>
    operator T() const {
        if (!isNumber(data)) {
            std::cerr << "Not a number\n";
            return static_cast<T>(-1);
        } else {
            if (strchr(data, '.') != nullptr) {
                return static_cast<T>(atof(data));
            } else {
                return static_cast<T>(atoi(data));
            }
        }
    }
    
    void print(std::ostream& os, const char *format) const {
        if (strcmp(format, "%s") == 0) {
            os << data;
        } else if (strcmp(format, "%d") == 0) {
            if (!isNumber(data)) {
                std::cerr << "Not a number";
            } else {
                os << atoi(data);
            }
        } else if (strcmp(format, "%x") == 0) {
            if (!isNumber(data)) {
                std::cerr << "Not a number";
            } else {
                int i_value = atoi(data);
                char *tmp = new char[strlen(data) + 1];
                snprintf(tmp, strlen(data), "%x", i_value);
                os << tmp;
                delete[] tmp;
            }
        } else if (strcmp(format, "%u") == 0) {
            if (!isNumber(data)) {
                std::cerr << "Not a number";
            } else {
                unsigned int value = static_cast<unsigned int>(atoi(data));
                os << value;
            }
        } else if (format[1] == 'f') {
            if (!isNumber(data)) {
                std::cerr << "Not a number";
            } else {
                double d_value = atof(data);
                if (format[2] == '.') {
                    int len = strlen(data);
                    int precision = atoi(format + 3);
                    int i = 0;
                    while (data[i] != '.') {
                        os << data[i];
                        i++;
                        len--;
                    }
                    if (precision != 0) {
                        os << '.';
                        i++;
                        len--;
                        while (precision > 0 && len > 0) {
                            os << data[i];
                            i++;
                            precision--;
                            len--;
                        }
                        while (precision > 0) {
                            os << 0;
                            precision--;
                        }
                    }
                } else {
                    char *tmp = new char[strlen(data) + 1];
                    snprintf(tmp, strlen(data) + 1, "%f", d_value);
                    os << tmp;
                    delete[] tmp;
                }
            }
        } else {
            std::cerr << "Available formats: %s, %d, %x, %u, %f.N";
        }
    }

    friend std::istream& operator >> (std::istream& is, StrNum& obj) {
        char input[100];
        is >> input;
        obj = StrNum(input);
        return is;
    }
};

int main() {
    StrNum str1, str2;
    char format[4];
    int choice = 0, type = 0;
    signed int inum = 0;
    double dnum = 0;

    std::cout << "0.Exit\n";
    std::cout << "1.Input first string \n";
    std::cout << "2.Input second string\n";
    std::cout << "3.Output first string\n";
    std::cout << "4.Output second string \n";
    std::cout << "5.First string = second string\n";
    std::cout << "6.First string == second string?\n";
    std::cout << "7.First string > second string?\n";
    std::cout << "8.First string < second string?\n";
    std::cout << "9.First string = first string + second string\n";
    std::cout << "10.Format first string (%s, %d, %x, %u, %f.N)\n";
    std::cout << "11.Convert first string\n";              

    do {
        std::cin >> choice;
        switch (choice) {
            case 0:
                break;
            case 1:
                std::cout << "Input string: ";
                std::cin >> str1;
                break;
            case 2:
                std::cout << "Input string: ";
                std::cin >> str2;
                break;
            case 3:
                std::cout << "First string: ";
                str1.print(std::cout, "%s");
                std::cout << std::endl;
                break;
            case 4:
                std::cout << "Second string: ";
                str2.print(std::cout, "%s");
                std::cout << std::endl;
                break;
            case 5:
                str1 = str2;
                std::cout << "First string: ";
                str1.print(std::cout, "%s");
                std::cout << std::endl;
                break;
            case 6:
                if (str1 == str2) {
                    std::cout << "Yes" << std::endl;
                } else {
                    std::cout << "No" << std::endl;
                }
                break;
            case 7:
                if (str1 > str2) {
                    std::cout << "Yes" << std::endl;
                } else {
                    std::cout << "No" << std::endl;
                }
                break;
            case 8:
                if (str1 < str2) {
                    std::cout << "Yes" << std::endl;
                } else {
                    std::cout << "No" << std::endl;
                }
                break;
            case 9:
                str1 = str1 + str2;
                std::cout << "Concatenation: ";
                str1.print(std::cout, "%s");
                std::cout << std::endl;
                break;
            case 10:
                std::cout << "Input format: ";
                std::cin >> format;
                str1.print(std::cout, format);
                std::cout << std::endl;
                break;
            case 11:
                std::cout << "1.Signed int\n";
                std::cout << "2.Double\n";
                std::cin >> type;
                if (type == 1) {
                    inum = static_cast<signed int>(str1);
                    std::cout << "Integer: " << inum << std::endl;
                } else {
                    dnum = static_cast<double>(str1);
                    std::cout << "Double: " << dnum << std::endl;
                }
                break;
            default:
                break;
            }
    } while (choice != 0);

    return 0;
}