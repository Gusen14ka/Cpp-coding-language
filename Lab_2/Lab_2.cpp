#include <array>
#include <iostream>
#include <regex>
#include <fstream>
#include <optional>



class Equation {

    public:
        struct Solution {
            int numRoots = NAN;
            double root1 = NAN;
            double root2 = NAN;
        };

        struct EquationCoefs {
            double a = NAN;
            double b = NAN;
            double c = NAN;
        };

    EquationCoefs equation;
    Solution solution;

    private:
        double get_coefficient(const std::string& str) {
            if (str.empty() || str == "+") {
                return 1.0;
            }
            if (str == "-") {
                return -1.0;
            }
            return stod(str);
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
            constexpr double epsilon = 1e-9;
            return std::abs(a - b) < epsilon;
        }

    public:
        void ReadEquation(std::ifstream& file) {
            std::string input;
            getline(file, input);
            if (input.empty()) {
                throw std::runtime_error("Empty file");
                return;
            }

            std::regex format_full_eq(R"(^\s*([+-]?\d*(?:\.\d*)?)\s*x\^2\s*([+-]\d*(?:\.\d*)?)\s*x\s*([+-]\d*(?:\.\d*)?)\s*=\s*0\s*$)");
            std::regex format_without_b(R"(^\s*([+-]?\d*(?:\.\d*)?)\s*x\^2\s*([+-]\d*(?:\.\d*)?)\s*=\s*0\s*$)");
            std::regex format_without_c(R"(^\s*([+-]?\d*(?:\.\d*)?)\s*x\^2\s*([+-]\d*(?:\.\d*)?)\s*x\s*=\s*0\s*$)");
            std::regex format_without_b_and_c(R"(^\s*([+-]?\d*(?:\.\d*)?)\s*x\^2\s*=\s*0\s*$)");
            std::smatch match;

            if (regex_match(input, match, format_full_eq)) {
                //coef a
                equation.a = get_coefficient(match[1].str());
                //coef b
                equation.b = get_coefficient(match[2].str());
                //coef c
                equation.c = get_coefficient(match[3].str());
            }
            else if(regex_match(input, match, format_without_b)){
                //coef a
                equation.a = get_coefficient(match[1].str());
                //coef b
                equation.b = 0.0;
                //coef c
                equation.c = get_coefficient(match[2].str());
            }
            else if (regex_match(input, match, format_without_c)) {
                //coef a
                equation.a = get_coefficient(match[1].str());
                //coef b
                equation.b = get_coefficient(match[2].str());
                //coef c
                equation.c = 0.0;
            }
            else if (regex_match(input, match, format_without_b_and_c)) {
                //coef a
                equation.a = get_coefficient(match[1].str());
                //coef b
                equation.b = 0.0;
                //coef c
                equation.c = 0.0;
            }
            else {
                throw std::runtime_error("The entered equation does not satisfy the format ax^2+bx+c=0");
                return;
            }
            return;
        }

        void SolveEquation() {
            if (std::isnan(equation.a) || std::isnan(equation.b) || std::isnan(equation.c)) {
                throw std::runtime_error("Empty equation");
                return;
            }
            double discriminant = pow(equation.b,2) - 4 * equation.a * equation.c;
            if (discriminant < 0) {
                solution.numRoots = 0;
                return;
            }
            double sqrt_of_discrim = std::sqrt(discriminant);
            double root1 = ((-1.0) * equation.b + sqrt_of_discrim) / (2 * equation.a);
            double root2 = ((-1.0) * equation.b - sqrt_of_discrim) / (2 * equation.a);
            // Checking for a root match
            if (RealEqualityDouble(root1, root2)) {
                solution.numRoots = 1;
                solution.root1 = root1;
                solution.root2 = root1;
                return;
            }
            else {
                solution.numRoots = 2;
                solution.root1 = std::min(root1, root2);
                solution.root2 = std::max(root1, root2);
                return;
            }
        }

};