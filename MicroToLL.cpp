#include "MicroToLL.h"
#include "lib/common.h"
// #include "lib/micro.h"
#include <string>
#include <regex>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include "llvm/IR/Module.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/AsmParser/Parser.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/ExecutionEngine/Orc/Core.h"
#include "llvm/ExecutionEngine/Orc/LLJIT.h"
using namespace llvm;
using namespace MicroLLVM;

std::shared_ptr<Statement> parse(std::string s) {
    std::string var = "([a-zA-Z][a-zA-Z0-9]*)";
    std::string value = "([a-zA-Z0-9]+)";
    std::regex assignment(var + " += +([^ ].*) *$");
    std::regex ret("ret +" + value + " *$");
    std::regex store("store +" + value + " +" + var + " *$");
    std::regex branch_cond("branch +" + value + " +" + var + " +" + var + " *$"); // br <condition> <target>
    std::regex branch_uncond("branch +" + value + " *$");
    std::regex label(var + ": *$");

    std::smatch m;
    // bool is_asgn = std::regex_match(s, m, assignment);


    if (std::regex_match(s, m, assignment)) {
        // TODO: ADD CHECKS FOR TOKEN LENGTH
        auto toks = tokenize(m[2]);
        std::string target = m[1], opcode = toks[0];
        if (opcode == "add" || opcode == "sub" ||
            opcode == "mul" || opcode == "div" ||
            opcode == "or" || opcode == "and" || opcode == "xor")
        {
            // binary
            return std::make_shared<BinaryStatement>(target, opcode, make_operand(toks[1]), make_operand(toks[2]));
        } else if (opcode == "icmp" && toks.size() == 4) {
            if (toks[1] == "gt" || toks[1] == "lt" || toks[1] == "eq" ||
                toks[1] == "ge" || toks[1] == "le")
            {
                return std::make_shared<CompareStatement>(target, toks[1], make_operand(toks[2]), make_operand(toks[3]));
            }
        } else if (opcode == "alloc") {
            // a = alloc <type>
            return std::make_shared<NewStatement>(target, toks[1]);
        } else if (opcode == "load") {
            return std::make_shared<LoadStatement>(target, make_operand(toks[1]));
        } else if (opcode == "call") {
            std::vector<std::string> args(toks.begin() + 1, toks.end());
            return std::make_shared<CallStatement>(target, toks[1], args);
        } else if (opcode == "new") {
            return std::make_shared<NewStatement>(target, toks[1]);
        } else if (opcode == "get") {
            return std::make_shared<GetStatement>(target, make_operand(toks[1]), toks[2]);
        }

    } else if (std::regex_match(s, m, ret)) {
        return std::make_shared<ReturnStatement>(make_operand(m[1]));
    } else if (std::regex_match(s, m, store)) {
        std::string stored_field = m[1], addr_field = m[2];
        std::cout << stored_field << "\n" << addr_field << std::endl;
    } else if (std::regex_match(s, m, branch_cond)) {
        return std::make_shared<BranchStatement>(make_operand(m[1]), m[2], m[3]);
    } else if (std::regex_match(s, m, branch_uncond)) {
        return std::make_shared<BranchStatement>(m[1]);
    } else if (std::regex_match(s, m, label)) {
        return std::make_shared<BlockLabel>(m[1]);
    }

    return nullptr;
}

void parse_repl() {
    std::string input;
    std::cout << "> ";
    while (std::getline(std::cin, input)) {
        auto p = parse(input);
        if (p == nullptr) {
            std::cout << "\n< No valid parse\n\n";
        } else {
            std::cout << "\n< Parsed " + p->typestr() + " statement"
                      << "\n< " + p->str() << "\n\n";
        }

        std::cout << "> ";
    }
}

void get_fns(FnTypeResolver& resolver) {
    std::ifstream infile;
    std::string buf;
    infile.open("fns.txt");
    while (std::getline(infile, buf)) {
        auto toks = tokenize(buf);
        if (toks.size() > 1) {
            auto types = std::vector<std::string>(toks.begin() + 1, toks.end());
            std::string fname = toks[0];
            resolver.add_fn(fname, types);
        }
    }
    infile.close();
}

void get_structs(FnTypeResolver& resolver) {
    std::ifstream infile;
    std::string buf;
    infile.open("structs.txt");
    while (std::getline(infile, buf)) {
        auto toks = tokenize(buf);
        if (toks.size() % 2 == 1 && toks.size() > 2) {
            auto fields = std::vector<std::string>(toks.begin() + 1, toks.end());
            std::string sname = toks.at(0);
            resolver.add_struct(sname, fields);
        }
    }
    infile.close();
}

std::vector<Operand> make_header(std::vector<std::pair<std::string, std::string>> args) {
    std::vector<Operand> header;
    // for (auto it = p.begin(); it != p.end(); ++it) {
    for (auto pair : args) {
        header.push_back(Operand(pair.first, pair.second));
    }
    return header;
}

void infer_repl(bool echo=false) {
    // code repetition...
    std::string input;
    FnTypeResolver resolver("FN", make_header({{"_ret", "i32"}, {"x", "i32"}, {"y", "i32"}}));
    // resolver.add_fn("f", {"i32", "i32", "i32*"});
    get_fns(resolver);
    get_structs(resolver);
    while (std::getline(std::cin, input)) {
        auto p = parse(input);
        if (p != nullptr) {
            resolver.add_stmt(p);
        }

        // std::cout << "> ";
        if (echo)
            std::cout << input << std::endl;
    }
    auto typemap = resolver.get_types();
    resolver.perform_pass();
    if (echo) 
        std::cout << "\n\n";
    std::cout << resolver.emit_str();
    resolver.get_module();
}

int main() {
    infer_repl(true);
}
