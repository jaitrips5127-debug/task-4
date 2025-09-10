#include <iostream>
#include <string>
#include <cctype>
#include <stdexcept>

using namespace std;

class ExpressionParser {
    string input;
    size_t pos;

public:
    ExpressionParser(const string& expr) : input(expr), pos(0) {}

    // Entry point
    int parse() {
        int result = parseExpression();
        if (pos < input.size()) {
            throw runtime_error("Unexpected character at end");
        }
        return result;
    }

private:
    // Skip whitespace
    void skipWhitespace() {
        while (pos < input.length() && isspace(input[pos])) pos++;
    }

    // Parse an expression: term (+|-) term
    int parseExpression() {
        int result = parseTerm();
        skipWhitespace();
        while (pos < input.length() && (input[pos] == '+' || input[pos] == '-')) {
            char op = input[pos++];
            skipWhitespace();
            int rhs = parseTerm();
            if (op == '+') result += rhs;
            else result -= rhs;
            skipWhitespace();
        }
        return result;
    }

    // Parse a term: factor (*|/) factor
    int parseTerm() {
        int result = parseFactor();
        skipWhitespace();
        while (pos < input.length() && (input[pos] == '*' || input[pos] == '/')) {
            char op = input[pos++];
            skipWhitespace();
            int rhs = parseFactor();
            if (op == '*') result *= rhs;
            else {
                if (rhs == 0) throw runtime_error("Division by zero");
                result /= rhs;
            }
            skipWhitespace();
        }
        return result;
    }

    // Parse a factor: number or (expression)
    int parseFactor() {
        skipWhitespace();
        if (pos >= input.length()) throw runtime_error("Unexpected end of input");

        if (input[pos] == '(') {
            pos++;  // skip '('
            int result = parseExpression();
            if (pos >= input.length() || input[pos] != ')') {
                throw runtime_error("Missing closing parenthesis");
            }
            pos++;  // skip ')'
            return result;
        } else if (isdigit(input[pos]) || (input[pos] == '-' && isdigit(input[pos + 1]))) {
            return parseNumber();
        }

        throw runtime_error(string("Unexpected character: ") + input[pos]);
    }

    // Parse an integer number
    int parseNumber() {
        skipWhitespace();
        int sign = 1;
        if (input[pos] == '-') {
            sign = -1;
            pos++;
        }
        int num = 0;
        if (!isdigit(input[pos])) throw runtime_error("Expected a number");
        while (pos < input.length() && isdigit(input[pos])) {
            num = num * 10 + (input[pos++] - '0');
        }
        return sign * num;
    }
};

int main() {
    string line;
    cout << "Enter arithmetic expressions (type 'exit' to quit):\n";
    while (true) {
        cout << "> ";
        getline(cin, line);
        if (line == "exit") break;
        try {
            ExpressionParser parser(line);
            int result = parser.parse();
            cout << "Result: " << result << "\n";
        } catch (exception& e) {
            cout << "Error: " << e.what() << "\n";
        }
    }
    return 0;
}