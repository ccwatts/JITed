#ifndef MICRO_JIT__H
#define MICRO_JIT__H

#include "llasm.h"

// micro to llvm
#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <map>

#define STRBUF std::vector<std::string>

std::unique_ptr<LLVMOperand> resolveType(std::map<std::string, LLVMOperand> typemap, std::string name) {
    if (typemap.count(name)) {
        std::unique_ptr<LLVMOperand> retval = std::unique_ptr<LLVMOperand>(new LLVMOperand(typemap.at(name)));
        return retval;
    } else {
        // not in the type map; may be an immediate value?
        // string checks later...
        // for (auto it = name.begin(); it != name.end(); ++it) {
            
        // }
        return std::unique_ptr<LLVMOperand>(nullptr);
    }
}

std::vector<std::string> tokenize(std::string s, const char delimiter=' ') {
    size_t start;
    size_t end = 0;

    std::vector<std::string> output;

    while ((start = s.find_first_not_of(delimiter, end)) != std::string::npos) {
        end = s.find(delimiter, start);
        output.push_back(s.substr(start, end - start));
    }

    return output;
}

bool contains(std::string s, std::string sub) {
    return s.find(sub) != std::string::npos;
}

bool is_assign(STRBUF toks) {
    // x = 3;
    return toks.at(1) == "=" && toks.size() == 3;
}

bool is_alloc(STRBUF toks) {
    // a = alloca i32
    if (toks.at(1) != "=" || toks.size() < 3) return false;

    // iterate, see if everything's valid
    // what should the syntax look like?

    return true;
}

bool is_arith(STRBUF toks) {
    // x = add y z
    bool code_matched = toks.at(2) == "add" || toks.at(2) == "sub" ||
                            toks.at(2) == "mul" || toks.at(2) == "div";
    return toks.at(1) == "=" && code_matched && toks.size() == 5;
}

bool is_store(STRBUF toks) {
    // needs more; typechecking, see the fields more?
    return toks.at(0) == "store" && toks.size() == 3;
}

bool is_load(STRBUF toks) {
    // x = load y
    return toks.at(1) == "=" && toks.size() == 4;
}

std::unique_ptr<LLVMStatement> line_to_llvm(std::string line) {
    if (line.length() == 0) {
        return std::unique_ptr<LLVMStatement>(new NullStatement({}));
    }

    auto toks = tokenize(line);

    if (is_assign(toks)) {
        std::cout << "assign";
    } else if (is_arith(toks)) {
        std::string operation = toks.at(2);
        std::cout << "arith";
        toks.erase(toks.begin() + 1, toks.begin() + 2);
        // toks to somethin
        auto x = std::unique_ptr<LLVMStatement>(new ArithmeticStatement({}, operation));
    } else if (is_alloc(toks)) {
        std::cout << "alloc";
    } else if (is_store(toks)) {
        std::cout << "store";
    } else if (is_load(toks)) {
        std::cout << "load";
    }

    // placeholder
    return std::unique_ptr<LLVMStatement>(new NullStatement({}));
}

std::vector<std::string> to_llvm(std::vector<std::string>& lines) {

    return lines;
}

#endif