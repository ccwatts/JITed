#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <functional>
#include <regex>
#include "lib/interpreter/ast.h"
#include "lib/common.h"

using std::string;
using namespace ast;



typedef std::shared_ptr<Expression> EXP_P;
typedef std::pair<EXP_P, string> EXP_PARSE_RES;

std::pair<string, string> consume(string input, string to_consume) {
    string temp;
    int i = 0;
    while (std::isspace(input.at(i))) {
        i++;
    }
    temp = input.substr(i);
    // std::cout << temp;

    std::regex finder("^(" + to_consume + ")([\\s\\S]*)\n?");
    std::smatch m;
    if (std::regex_match(temp, m, finder)) {
        return {m[m.size() - 1], m[1]};
    } else {
        return {input, ""};
    }
}

std::shared_ptr<BinaryOp> make_bop(string op, EXP_P l, EXP_P r) {
    return std::make_shared<BinaryOp>(op, l, r);
}

// TODO: the check for whether or not to continue needs to be more robust
//       ie it needs to be able to check for MULTIPLE
//       ...just use a regex?
class ExpressionParser {
private:
    // binary operations 
    static EXP_PARSE_RES extend(std::function<EXP_PARSE_RES(string)> parse_fn, string join, EXP_P e1, string rest1) {
        std::string rest;
        if ((rest = consume(rest1, join).first).length() != rest1.length()) {
            EXP_P e2;
            string rest2;
            std::tie(e2, rest2) = parse_fn(rest);
            return {std::make_shared<BinaryOp>(join, e1, e2), rest2};
            // return {make_bop(join, e1, e2), rest2};
        } else {
            return {e1, rest1};
        }
    };

    static EXP_PARSE_RES parse_expression(string input) {
        EXP_P e;
        string rest;
        std::tie(e, rest) = parse_boolterm(input);
        std::tie(e, rest) = extend(parse_boolterm, "||", e, rest);
    }

    static EXP_PARSE_RES parse_boolterm(string input) {
        EXP_P e;
        string rest;
        std::tie(e, rest) = parse_eqterm(input);
        std::tie(e, rest) = extend(parse_eqterm, "&&", e, rest);
    }

    static EXP_PARSE_RES parse_eqterm(string input) {
        EXP_P e;
        string rest;
        std::tie(e, rest) = parse_relterm(input);
        std::tie(e, rest) = extend(parse_relterm, "&&", e, rest);
    }

    static EXP_PARSE_RES parse_relterm(string input) {
        EXP_P e;
        string rest;
        std::tie(e, rest) = parse_simple(input);
        // the order is important here, at least the leq/geq before g/l
        std::tie(e, rest) = extend(parse_simple, ">=|<=|>|<", e, rest);
    }

    static EXP_PARSE_RES parse_simple(string input) {
        EXP_P e;
        string rest;
        std::tie(e, rest) = parse_term(input);
        std::tie(e, rest) = extend(parse_term, "-|+", e, rest);
    }

    static EXP_PARSE_RES parse_term(string input) {
        EXP_P e;
        string rest;
        std::tie(e, rest) = parse_unary(input);
        std::tie(e, rest) = extend(parse_unary, "\\*|/", e, rest);
    }

    // unary operations and terminals
    static EXP_PARSE_RES parse_unary(string input) {
        EXP_P e;
        string rest, found;
        std::tie(rest, found) = consume(input, "!|-");
        std::tie(e, rest) = parse_selector(rest);
        return {std::make_shared<UnaryOp>(found, e), rest};
    }

    // dot operation/access
    static EXP_PARSE_RES parse_selector(string input) {
        return {nullptr, ""};
    }

    static EXP_PARSE_RES parse_factor(string input) {
        return {nullptr, ""};
    }

    static EXP_PARSE_RES parse_terminal(string input) {
        const string id = "[a-zA-Z][a-zA-Z0-9]*";
        const string number = "[0-9]";
        const string bool_const = "true|false";
        const string nullexp = "null";
        return {nullptr, ""};
    }
public:
    static EXP_P parse() {
        return nullptr;
    }
};

int main() {
    string a = "   WTF LOL who even knows\nabc";
    auto consumed = consume(a, "LOL|WTF");
    std::cout << "first: " << consumed.first << "\nsecond:" << consumed.second << std::endl;
}
