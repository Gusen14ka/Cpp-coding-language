#pragma warning(disable : 4996)
#include <array>
#include <iostream>
#include <regex>


int const NUM_OF_COEF = 3;
int const CONST_FOR_SOLUTION = 3;
int const NUM_OF_DIGITS_IN_DOUBLE = 15;

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

int CountNumberOfNaturalDigits(double n) {
    int digits = 1;
    while (n > 10) {
        n /= 10.0;
        digits++;
    }
    return digits;
}

bool RealEqualityDouble(double a, double b) {
    double max_of_two = max(a, b);
    int accuracy = NUM_OF_DIGITS_IN_DOUBLE - CountNumberOfNaturalDigits(max_of_two);
    return max_of_two - min(a, b) < accuracy;
}

void SolveEquation(const array<double, NUM_OF_COEF>& equation, array<double, CONST_FOR_SOLUTION>& solution) {
    double discriminant = pow(equation[1],2) - 4 * equation[0] * equation[2];
    if (discriminant < 0) {
        solution[0] = 0;
        return;
    }
    double sqrt_of_discrim = pow(discriminant, 0.5);
    double root1 = ((-1.0) * equation[1] + sqrt_of_discrim) / (2 * equation[0]);
    double root2 = ((-1.0) * equation[1] - sqrt_of_discrim) / (2 * equation[0]);
    // Checking for a root match
    if (RealEqualityDouble(root1, root2)) {

    }


}

int main() {
    array<double, NUM_OF_COEF> equation;
    ReadEquation(equation);
    array<double, CONST_FOR_SOLUTION> solution; //The value of an element with the index zero means the number of roots.
    SolveEquation(equation, solution);
    
}
