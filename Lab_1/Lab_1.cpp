#include <array>
#include <iostream>
#include <regex>


int const NUM_OF_COEF = 3;

using namespace std;

double get_coefficient(const string& str) {
    if (str.empty() || str == "+") {
        return 1.0;
    }
    if (str == "-") {
        return -1.0;
    }
    return stod(str);
}

void ReadEquation(array<double, NUM_OF_COEF>& equation) {
    string input;
    cout << "Enter a quadratic equation of the form ax^2+bx+c=0:" << endl;
    getline(cin, input);

    regex format_full_eq(R"(^\s*([+-]?\d*(?:\.\d*)?)\s*x\^2\s*([+-]\d*(?:\.\d*)?)\s*x\s*([+-]\d*(?:\.\d*)?)\s*=\s*0\s*$)");
    regex format_without_b(R"(^\s*([+-]?\d*(?:\.\d*)?)\s*x\^2\s*([+-]\d*(?:\.\d*)?)\s*=\s*0\s*$)");
    regex format_without_c(R"(^\s*([+-]?\d*(?:\.\d*)?)\s*x\^2\s*([+-]\d*(?:\.\d*)?)\s*x\s*=\s*0\s*$)");
    smatch match;

    if (regex_match(input, match, format_full_eq)) {
        //coef a
        equation[0] = get_coefficient(match[1].str());
        //coef b
        equation[1] = get_coefficient(match[2].str());
        //coef c
        equation[2] = get_coefficient(match[3].str());
    }
    else if((regex_match(input, match, format_without_b))){
        //coef a
        equation[0] = get_coefficient(match[1].str());
        //coef b
        equation[1] = 0.0;
        //coef c
        equation[2] = get_coefficient(match[2].str());
    }
    else if ((regex_match(input, match, format_without_c))) {
        //coef a
        equation[0] = get_coefficient(match[1].str());
        //coef b
        equation[1] = get_coefficient(match[2].str());
        //coef c
        equation[2] = 0.0;
    }
    else {
        cerr << "The entered equation does not satisfy the format ax^2+bx+c=0" << endl;
    }
}

int main() {
    array<double, NUM_OF_COEF> equation;
    ReadEquation(equation);
}
