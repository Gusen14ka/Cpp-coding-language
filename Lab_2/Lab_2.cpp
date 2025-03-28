#include <array>
#include <fstream>
#include <iostream>
#include <optional>
#include <regex>
#include <iomanip>
#include <sstream>
#include <random>
#include <vector>
#include <unordered_map>

bool RealEqualityDouble(double a, double b) {
    constexpr double epsilon = 1e-9;
    return std::abs(a - b) < epsilon;
}

struct Solution
{
    int numRoots = NAN;
    double root1 = NAN;
    double root2 = NAN;

    Solution() = default;
    Solution(int roots, double r1, double r2)
        : numRoots(roots), root1(r1), root2(r2) {}

    bool isValid() const {
        if (numRoots < 0 || numRoots > 2)
            return false;

        switch (numRoots) {
            case 0:
                return std::isnan(root1) && std::isnan(root2);
            case 1:
                return !std::isnan(root1) && RealEqualityDouble(root1, root2);
            case 2:
                return !std::isnan(root1) && !std::isnan(root2) &&
                    (root1 <= root2);
            default:
                return false;
        }
    }
};

struct EquationCoefs
{
    double a = NAN;
    double b = NAN;
    double c = NAN;
};

class Equation
{
private:
    EquationCoefs equation;
    Solution solution;

    static inline std::regex const format_full_eq{
        R"(^\s*([+-]?\d*(?:\.\d*)?)\s*x\^2\s*([+-]\d*(?:\.\d*)?)\s*x\s*([+-]\d*(?:\.\d*)?)\s*=\s*0\s*$)"};
    static inline std::regex const format_without_b{
        R"(^\s*([+-]?\d*(?:\.\d*)?)\s*x\^2\s*([+-]\d*(?:\.\d*)?)\s*=\s*0\s*$)"};
    static inline std::regex const format_without_c{
        R"(^\s*([+-]?\d*(?:\.\d*)?)\s*x\^2\s*([+-]\d*(?:\.\d*)?)\s*x\s*=\s*0\s*$)"};
    static inline std::regex const format_without_b_and_c{
        R"(^\s*([+-]?\d*(?:\.\d*)?)\s*x\^2\s*=\s*0\s*$)"};

    static double get_coefficient(const std::string &str)
    {
        if (str.empty() || str == "+")
        {
            return 1.0;
        }
        if (str == "-")
        {
            return -1.0;
        }
        return stod(str);
    }

    int CountNumberOfNaturalDigits(double n)
    {
        int digits = 1;
        while (n > 10)
        {
            n /= 10.0;
            digits++;
        }
        return digits;
    }


public:
    EquationCoefs const &getEquation() const { return equation;}

    Solution const &getSolution() const { return solution;}

    void setSolution(const Solution& newSolution) {
        if (!newSolution.isValid()) {
            throw std::invalid_argument("Invalid solution data");
        }
        solution = newSolution;
    }

    void ReadEquationFromFile(std::ifstream &file)
    {
        std::string input;
        getline(file, input);
        if (input.empty())
        {
            throw std::runtime_error("Empty file");
            return;
        }

        std::smatch match;

        if (regex_match(input, match, format_full_eq))
        {
            // coef a
            equation.a = get_coefficient(match[1].str());
            // coef b
            equation.b = get_coefficient(match[2].str());
            // coef c
            equation.c = get_coefficient(match[3].str());
        }
        else if (regex_match(input, match, format_without_b))
        {
            // coef a
            equation.a = get_coefficient(match[1].str());
            // coef b
            equation.b = 0.0;
            // coef c
            equation.c = get_coefficient(match[2].str());
        }
        else if (regex_match(input, match, format_without_c))
        {
            // coef a
            equation.a = get_coefficient(match[1].str());
            // coef b
            equation.b = get_coefficient(match[2].str());
            // coef c
            equation.c = 0.0;
        }
        else if (regex_match(input, match, format_without_b_and_c))
        {
            // coef a
            equation.a = get_coefficient(match[1].str());
            // coef b
            equation.b = 0.0;
            // coef c
            equation.c = 0.0;
        }
        else
        {
            throw std::runtime_error("The entered equation does not satisfy the format ax^2+bx+c=0");
            return;
        }
        return;
    }

    static EquationCoefs ReadEquationFromStr(std::string const& eqStr) {
        EquationCoefs coefs;

        std::smatch match;
        std::regex_match(eqStr, match, format_full_eq);
        coefs.a = get_coefficient(match[1].str());
        coefs.b = get_coefficient(match[2].str());
        coefs.c = get_coefficient(match[3].str());

        return coefs;
    }

    void SolveEquation()
    {
        if (std::isnan(equation.a) || std::isnan(equation.b) || std::isnan(equation.c))
        {
            throw std::runtime_error("Empty equation");
            return;
        }
        double discriminant = pow(equation.b, 2) - 4 * equation.a * equation.c;
        if (discriminant < 0)
        {
            solution.numRoots = 0;
            return;
        }
        double sqrt_of_discrim = std::sqrt(discriminant);
        double root1 = ((-1.0) * equation.b + sqrt_of_discrim) / (2 * equation.a);
        double root2 = ((-1.0) * equation.b - sqrt_of_discrim) / (2 * equation.a);
        // Checking for a root match
        if (RealEqualityDouble(root1, root2))
        {
            solution.numRoots = 1;
            solution.root1 = root1;
            solution.root2 = root1;
            return;
        }
        else
        {
            solution.numRoots = 2;
            solution.root1 = std::min(root1, root2);
            solution.root2 = std::max(root1, root2);
            return;
        }
    }
};

class Student
{
private:
    std::string name;

public:
    Student(std::string val) : name(val) {}

    void WriteAnswer(std::ofstream &file, Equation& equation)
    {
        std::ostringstream oss;
        EquationCoefs coefs = equation.getEquation();
        Solution solution = equation.getSolution();
        oss << std::fixed << std::setprecision(9) << coefs.a << "x^2";
        if (coefs.b >= 0) {
            oss << "+";
        }
        oss << std::fixed << std::setprecision(9) << coefs.b << "x";
        if (coefs.c >= 0) {
            oss << "+";
        }
        oss << std::fixed << std::setprecision(9) << coefs.c << "=0";
        std::string equationStr = oss.str();
        file << name << "|" << equationStr << "|" << solution.numRoots << "|"
             << solution.root1 << "|" << solution.root2 << std::endl;
    }

    std::string const& getName() const { return name;}
};

class BadStudent : public Student {
  public:
      BadStudent(std::string val) : Student(val) {}

      void DoTask(std::ofstream& file, Equation& equation) {
          try {
              equation.setSolution(Solution{1, 0.0, 0.0});
          } catch (const std::invalid_argument& e) {
              throw std::invalid_argument(e.what());
          }
          WriteAnswer(file, equation);
      }
};

class GoodStudent : public Student {
  private:
    double probability = 0.5;
  public:
    GoodStudent(std::string val) : Student(val) {}

    void DoTask(std::ofstream& file, Equation& equation) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<double> dist(0.0, 1.0);
        if (dist(gen) < probability) {
            try {
                equation.setSolution(Solution{1, 0.0, 0.0});
            } catch (const std::invalid_argument& e) {
                throw std::invalid_argument(e.what());
            }
        }
        WriteAnswer(file, equation);
    }
};

class Teacher {
  private:
    std::vector<Equation> equationsList;

    struct WorkData {
        std::vector<Solution> solutions;
        std::vector<EquationCoefs> equations;
    };
    std::unordered_map<std::string, WorkData> worksData;

    void ReadWorks(std::ifstream& file) {
        std::string line;
        while (std::getline(file, line)) {
            if (line.empty())
                continue;

            std::istringstream iss(line);
            std::string name, equationStr, numRootsStr, root1Str, root2Str;

            if (!std::getline(iss, name, '|') ||
                !std::getline(iss, equationStr, '|') ||
                !std::getline(iss, numRootsStr, '|') ||
                !std::getline(iss, root1Str, '|') ||
                !std::getline(iss, root2Str)) {
                continue;
            }

            int numRoots = std::stoi(numRootsStr);
            double root1 = std::stod(root1Str);
            double root2 = std::stod(root2Str);

            EquationCoefs coefs = Equation::ReadEquationFromStr(equationStr);

            Solution sol(numRoots, root1, root2);

            worksData[name].solutions.push_back(sol);
            worksData[name].equations.push_back(coefs);
        }
    }
  public:
    Teacher(std::vector<Equation> val) : equationsList(val) {}

    void CheckingWorks(std::ifstream& worksFile, std::ofstream& resultFile) {

    }
};
/*
Идея в том, чтобы сначала решить все уравнения из файла. А далее уже идёт работа классов студентов и учителя.
Таким образом мы сэкономим время работы и повторение кода - уравнения не будут решаться несколько раз
*/

int main()
{
    Equation student;
}
