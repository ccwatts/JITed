#ifndef TYPEINF_H
#define TYPEINF_H

#include "llvm/IR/Type.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"

#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/AsmParser/Parser.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/ExecutionEngine/Orc/Core.h"
#include "llvm/ExecutionEngine/Orc/LLJIT.h"
using namespace llvm;
// pointertype, integertype, structtype

#include <string>
#include <set>
#include <map>
#include <vector>
#include <algorithm>
#include <iostream>
#include <memory>
#include <cassert>
#include <sstream>
#include <regex>

namespace MicroLLVM {

const std::string UNKNOWN_TYPE = "<no type>";
const std::string BAD_DEREF = "<bad deref>";
const std::string PTR_STR_T = "i8";

const unsigned ADDR_BITS = 8;
const unsigned INT_BITS = 32;

/*
alloca
binary
branch
cast
invocation
load/store
phi (?)
return
*/

class Operand {
private:
    std::string type;
    std::string name;
    // int pcount;
    int immediate;
public:
    Operand(std::string name="", std::string type=UNKNOWN_TYPE) : type(type), name(name),/* pcount(pcount), */immediate(0) {};
    Operand(int immediate) : type("i32"), name(""),/* pcount(0),*/ immediate(immediate) {};
    std::string varname() { return name; }
    std::string type_str() { return type/* + std::string(pcount, '*')*/; }

    std::string str() { 
        return (name == "") ? std::to_string(immediate) : name;
    }

    std::string get_name() {
        return name;
    }

    std::string llvm() {
        return (is_immediate()) ? type + " " + str() : type + " %" + str();
    }

    std::string derefed_type() {
        // if (pcount > 0) {
        //     std::string t = type;
        //     t = t.substr(0, t.size());
        //     return t;
        // }
        if (type[type.size() - 1] == '*') {
            return type.substr(0, type.size() - 1);
        }
        return BAD_DEREF;
    }

    std::string llvm_notype() {
        return (is_immediate()) ? std::to_string(immediate) : "%" + name;
    }

    bool is_immediate() {
        return name == "";
    }

    void set_type(std::string new_type) {
        type = new_type;
    }

    void apply_type(std::map<std::string, std::string> typemap) {
        if (typemap.count(name)) {
            this->set_type(typemap.at(name));
        }
    }
};

Operand make_operand(std::string s) {
    std::regex num("[0-9]+");
    std::smatch m;
    if (std::regex_match(s, m, num)) {
        return Operand(std::stoi(s));
    } else {
        return Operand(s);
    }
}

class Statement {
protected:
    std::string target;
public:
    Statement(std::string target="") : target(target) {}
    virtual std::string typestr() = 0;
    // virtual bool uses(std::string) = 0; // can probably be reduced down to "is it in vars()"
    std::string target_name() { return target; }
    virtual std::string str() = 0;
    virtual std::vector<std::string> vars() = 0;
    virtual std::string llvm() = 0;
    virtual void apply_types(std::map<std::string, std::string>) = 0;
    bool uses(std::string var_name) {
        auto v = vars();
        return std::find(v.begin(), v.end(), var_name) != v.end();
    }
};

class BlockLabel : public Statement {
protected:
public:
    BlockLabel(std::string target) : Statement(target) {}
    std::string typestr() {
        return "bl";
    }
    std::string str() {
        return target + ":";
    }
    std::vector<std::string> vars() {
        return {};
    }
    std::string llvm() {
        return str();
    }
    void apply_types(std::map<std::string, std::string> typemap) {
        return;
    }
};

class AllocStatement : public Statement {
protected:
    std::string mem_type;
public:
    AllocStatement(std::string target, std::string mem_type) : mem_type(mem_type) {};

    std::string typestr() { return "alloc"; }

    std::string target_type() { return mem_type + "*"; }

    // bool uses(std::string var) {
    //     return target == var;
    // }

    std::string str() {
        return target + " = alloc " + target_type(); //may be incorrect
    }

    std::string llvm() {
        return "%" + target + " = alloca " + mem_type;
    }

    void apply_types(std::map<std::string, std::string> typemap) {
        return;
    }

    std::vector<std::string> vars() {
        return {target};
    }
};

class NewStatement : public Statement {
    static int ctr;
protected:
    std::string struct_name;
public:
    NewStatement(std::string target, std::string struct_name) : Statement(target), struct_name(struct_name) {}

    std::string typestr() {
        return "new";
    }

    std::string str() {
        return target + " = new " + struct_name;
    }

    std::vector<std::string> vars() {
        return {target};
    }

    std::string target_type() {
        return "\%struct." + struct_name + "*";
    }

    std::string llvm() {
        std::ostringstream oss;
        oss << "%TEMP_" << ctr << " = call i8* @malloc(i32 8)\n\t";
        oss << "%" << target << " = bitcast i8* %TEMP_" << ctr << " to \%struct." << struct_name << "*";
        ctr++;
        return oss.str();
    }

    void apply_types(std::map<std::string, std::string> typemap) {
        return;
    }
};
int NewStatement::ctr = 0;

class GetStatement : public Statement {
protected:
    Operand derefed;
    std::string field;
    std::string field_type;
    int idx;
public:
    GetStatement(std::string target, Operand derefed, std::string field, int idx=-1) : Statement(target), derefed(derefed), field(field), idx(idx), field_type(UNKNOWN_TYPE) {}

    std::string typestr() {
        return "get";
    }

    std::string str() {
        return target + " = get " + field + " " + derefed.str();
    }

    std::vector<std::string> vars() {
        return {derefed.str(), target};
    }

    std::string llvm() {
        return "%" + target + " = getelementptr " + derefed.derefed_type() + ", " + derefed.llvm() + ", i1 0, i32 " + std::to_string(idx);
    }

    void apply_types(std::map<std::string, std::string> typemap) {
        derefed.apply_type(typemap);
        return;
    }

    void apply_indices(std::map<std::string, std::vector<Operand>> structs) {
        if (structs.count(derefed.derefed_type())) {
            std::vector<Operand> field_list = structs.at(derefed.derefed_type());
            for (int i = 0; i < field_list.size(); ++i) {
                if (field_list.at(i).get_name() == field) {
                    idx = i;
                    field_type = field_list.at(i).type_str();
                    return;
                // } else {
                //     std::cout << "$$" << field << "v" << field_list.at(i).get_name() << "$$";
                }
            }
        }
    }

    std::string target_type() {
        return field_type;
    }
};

class BinaryStatement : public Statement {
protected:
    Operand lhs, rhs;
    std::string operation;
public:
    // probably validate that a valid operation is provided
    BinaryStatement(std::string target, std::string operation, Operand lhs, Operand rhs) : Statement(target), operation(operation), lhs(lhs), rhs(rhs) {}
    std::string typestr() { return "bin_" + operation; }

    std::string get_operation() { return operation; }

    bool is_boolean() {
        return operation == "and" || operation == "or" || operation == "xor";
    }

    // bool uses(std::string var) {
    //     return target == var || lhs.varname() == var || rhs.varname() == var;
    // }

    std::string str() {
        return target + " = " + operation + " " + lhs.str() + " " + rhs.str();
    }

    std::string llvm() {
        return "%" + target + " = " + operation + " " + lhs.llvm() + ", " + rhs.llvm_notype();
    }

    void apply_types(std::map<std::string, std::string> typemap) {
        lhs.apply_type(typemap);
        rhs.apply_type(typemap);
        return;
    }

    std::vector<std::string> vars() {
        std::vector<std::string> vs({target});
        if (!lhs.is_immediate()) vs.push_back(lhs.str());
        if (!rhs.is_immediate()) vs.push_back(rhs.str());
        return vs;
    }
};

class CompareStatement : public Statement {
protected:
    std::string type;
    Operand lhs;
    Operand rhs;
public:
    CompareStatement(std::string target, std::string type, Operand lhs, Operand rhs) : Statement(target), type(type), lhs(lhs), rhs(rhs) {}

    std::string typestr() {
        return "icmp";
    }

    std::string str() {
        return target + " = icmp " + type + " " + lhs.str() + " " + rhs.str();
    }

    std::vector<std::string> vars() {
        std::vector<std::string> vs({target});
        if (!lhs.is_immediate()) vs.push_back(lhs.str());
        if (!rhs.is_immediate()) vs.push_back(rhs.str());
        return vs;
    }

    std::string llvm() {
        return "%" + target + " = icmp s" + type + " i32 " + lhs.llvm_notype() + ", " + rhs.llvm_notype();
    }

    void apply_types(std::map<std::string, std::string> typemap) {
        lhs.apply_type(typemap);
        rhs.apply_type(typemap);
        return;
    }
};

class BranchStatement : public Statement {
protected:
    static int ctr;
    Operand condition;
    std::string loc1, loc2; // this is a block label, not an actual variable or address.
                          // it'll have to be checked against blocks later anyhow
public:
    BranchStatement(Operand condition, std::string loc1, std::string loc2) : Statement(), condition(condition), loc1(loc1), loc2(loc2) {}
    BranchStatement(std::string loc1) : Statement(), loc1(loc1) {};
    std::string typestr() { return "branch"; }

    // bool uses(std::string var) {
    //     return condition.varname() == var;
    // }

    std::string str() {
        return "branch " + condition.str() + " " + loc1 + " " + loc2;
    }
    
    std::string llvm() {
        std::ostringstream oss;
        if (loc2 == "") {
            oss << "br label %" << loc1;
        } else if (condition.llvm_notype() != "i1") {
            oss << "%BRTEMP" << ctr << " = bitcast " << condition.llvm() << " to i1\n\t";
            oss << "br i1 %BRTEMP" << ctr << ", label %" << loc1 << ", label %" << loc2;
            ctr++;
        } else {
            oss << "br i1 " << condition.llvm_notype() << ", label %" << loc1 << ", label %" << loc2; // need 2 locations
        }
        return oss.str();
    }

    void apply_types(std::map<std::string, std::string> typemap) {
        condition.apply_type(typemap);
        return;
    }

    std::vector<std::string> vars() {
        return {condition.str()};
    }
};

int BranchStatement::ctr = 0;

class CastStatement : public Statement {
protected:
    Operand casted;
    std::string cast_type;
public:
    CastStatement(std::string target, std::string cast_type, Operand casted) : Statement(target), cast_type(cast_type), casted(casted) {};
    std::string typestr() { return "cast"; }
    std::string casted_type() { return cast_type; }
    // bool uses(std::string var) {
    //     return target == var || casted.varname() == var;
    // }

    std::string str() {
        return target + " = cast " + cast_type + " " + casted.str();
    }

    std::string llvm() {
        return "%" + target + " = bitcast " + casted.llvm() + " to " + cast_type;
    }

    void apply_types(std::map<std::string, std::string> typemap) {
        casted.apply_type(typemap);
        return;
    }

    std::vector<std::string> vars() {
        return {target, casted.str()};
    }
};

// TODO: Void statements? optional target
class CallStatement : public Statement {
protected:
    std::string func;
    std::vector<Operand> args;
public:
    CallStatement(std::string target, std::string func, std::vector<std::string> args_) : Statement(target), func(func) {
        for (auto it = args_.begin(); it != args_.end(); ++it) {
            args.push_back(make_operand(*it));
        }
    };

    std::string typestr() { return "call"; }

    // bool uses(std::string var) {
    //     for (auto it = args.begin(); it != args.end(); ++it) {
    //         if (*it == var) {
    //             return true;
    //         }
    //     }
    //     return target == var;
    // }

    std::string func_name() { return func; }

    int idx_of(std::string var) {
        for (int i = 0; i < args.size(); ++i) {
            if (args.at(i).str() == var) {
                return i;
            }
        }
        return -1;
    }

    std::string str() {
        std::ostringstream oss;
        for (auto it = args.begin(); it != args.end(); ++it) {
            oss << it->str() + " ";
        }
        return target + " = call " + func + " " + oss.str();
    }

    std::string llvm() {
        std::ostringstream oss;
        if (target != "")
            oss << "%" << target << " = "; 
        oss << "call " << args.at(0).type_str() << " @" << func << "(";
        for (auto it = args.begin() + 1; it != args.end(); ++it) {
            if (it > args.begin() + 1)
                oss << ", ";
            oss << it->llvm();
        }
        oss << ")";
        return oss.str();
    }

    void apply_types(std::map<std::string, std::string> typemap) {
        for (auto it = args.begin(); it != args.end(); ++it) {
            it->apply_type(typemap);
        }
    }

    std::vector<std::string> vars() {
        auto vs = std::vector<std::string>();
        for (auto it = args.begin(); it != args.end(); ++it) {
            vs.push_back(it->str());
        }
        vs.push_back(target);
        return vs;
    }
};

class LoadStatement : public Statement {
protected:
    Operand address;
public:
    LoadStatement(std::string target, Operand address) : Statement(target), address(address) {};
    std::string typestr() { return "load"; }

    std::string addr_type() {
        return address.type_str();
    }

    // bool uses(std::string var) {
    //     return target == var || address.varname() == var;
    // }

    std::string str() {
        return target = " = load " + loaded_type() + " " + address.str();
    }
    
    std::string llvm() {
        std::string type = address.derefed_type();
        type = type.substr(0, type.size());
        return "%" + target + " = load " + type + ", " + address.llvm(); // TODO DOUBLECHECK
    }

    void apply_types(std::map<std::string, std::string> typemap) {
        address.apply_type(typemap);
        return;
    }

    std::string loaded_type() {
        return address.derefed_type();
    }

    std::vector<std::string> vars() {
        if (address.is_immediate()) {
            return {target};
        } else {
            return {target, address.str()};
        }
    }
};

class StoreStatement : public Statement {
    Operand address;
public:
    StoreStatement(std::string target, Operand address) : Statement(target), address(address) {};
    std::string typestr() { return "store"; }

    std::string addr_type() {
        return address.str();
    }

    // bool uses(std::string var) {
    //     return target == var || address.varname() == var;
    // }
    
    std::string str() {
        return "store " + target + " " + address.str();
    }
    
    std::string llvm() {
        std::string normalized = address.type_str().substr(0, address.type_str().length());
        return "store " + normalized + " " + address.llvm();
    }

    void apply_types(std::map<std::string, std::string> typemap) {
        address.apply_type(typemap);
        return;
    }

    std::vector<std::string> vars() {
        if (address.is_immediate()) {
            return {target};
        } else {
            return {target, address.str()};
        }
    }
};

class ReturnStatement : public Statement {
private:
    Operand retval;
public:
    ReturnStatement(Operand retval) : Statement(), retval(retval) {};
    std::string typestr() { return "ret"; }
    // bool uses(std::string var) {
    //     return var == target;
    // }
    std::string str() {
        return "ret " + target;
    }
    
    std::string llvm() {
        return "ret " + retval.llvm();
    }

    void apply_types(std::map<std::string, std::string> typemap) {
        retval.apply_type(typemap);
        return;
    }

    std::vector<std::string> vars() {
        std::regex num("[0-9]+");
        std::smatch m;
        std::string to_match = retval.str();
        if (std::regex_match(to_match, m, num)) {
            return {};
        } else {
            return {retval.str()};
        }
    }
};

class FnTypeResolver {
protected:
    std::vector<std::shared_ptr<Statement>> stmts;
    std::string fname;
    std::map<std::string, std::vector<std::string>> global_fns;
    std::map<std::string, std::vector<Operand>> structs;
    std::vector<Operand> header; // this needs to be changed...
    std::shared_ptr<llvm::LLVMContext> ctx;

    void try_assign_type(std::string& ft, std::string candidate) {
        if (ft != UNKNOWN_TYPE && ft != candidate) {
            return;
        } else {
            ft = candidate;
        }
    }

    // infer for a specific
    std::string _infer(std::string var_name, std::map<std::string, std::string>& m) {
        bool is_struct;
        std::string found_type = UNKNOWN_TYPE;

        for (auto it = stmts.begin(); it != stmts.end(); ++it) {
            if ((*it)->uses(var_name)) {
                std::string stmt_type = (*it)->typestr();
                if (stmt_type == "alloc") {
                    auto alloc_s = std::static_pointer_cast<NewStatement>(*it);
                    try_assign_type(found_type, alloc_s->target_type());
                } else if (stmt_type.find("bin_") != std::string::npos) {
                    // i8 or i32?
                    // i8s only used for pointers -- probably because 64bit
                    // both operands and result need to be ints in mini

                    // i1 might be used at some point during binary operations
                    // technically, there'll need to be promotions even if it's a boolean for more ops...?
                    try_assign_type(found_type, "i32");
                } else if (stmt_type == "branch") {
                    // only one operand in a branch
                    // return PTR_STR_T;
                    // is it a label, though...?
                    // YES.
                    try_assign_type(found_type, "i1");
                } else if (stmt_type == "cast") {
                    // this MIGHT be unnecessary, depending on what the casts were actually used for
                    // since type resolution is just a thing, could we look for spots to insert them automatically?
                    // yeah, prolly
                    auto cast_s = std::static_pointer_cast<CastStatement>(*it);
                    if (cast_s->target_name() == var_name) {
                        try_assign_type(found_type, cast_s->casted_type());
                        // return cast_s->casted_type();
                    }
                } else if (stmt_type == "call") {
                    auto call_s = std::static_pointer_cast<CallStatement>(*it);
                    auto fname = call_s->func_name();
                    if (global_fns.count(fname)) {
                        if (call_s->target_name() == var_name) {
                            // look for fn return type
                            try_assign_type(found_type, global_fns.at(fname).at(0));
                        } else {
                            int idx = call_s->idx_of(var_name);
                            try_assign_type(found_type, global_fns.at(fname).at(idx));
                        }
                    }
                    // should be an error if this happens
                } else if (stmt_type == "load") {
                    auto load_s = std::static_pointer_cast<LoadStatement>(*it);
                    if (load_s->target_name() == var_name) {
                        // loading into this variable; nothing can be said from it
                        try_assign_type(found_type, load_s->loaded_type());
                    } else {
                        // try_assign_type(found_type, PTR_STR_T);
                        // false; this should be a pointer type, but WHICH needs specification
                        try_assign_type(found_type, load_s->addr_type());
                        // try_assign_type(found_type, "i8");
                    }
                } else if (stmt_type == "store") {
                    // intentionally blank, here for completeness of types (or not)
                    // the addresses we load from may or may not be able to be vars...?
                    auto store_s = std::static_pointer_cast<StoreStatement>(*it);
                    if (store_s->target_name() == var_name and m.count(var_name)) {
                        try_assign_type(found_type, m.at(var_name)); // TODO: this is actually a pointer
                        // idk if this works?
                    }
                    // continue; // this makes me feel kinda gross
                } else if (stmt_type == "ret") {
                    // is unnecessary since the type of the return value should be known by the end of inference
                    // try_assign_type(found_type, header.at(0).type_str());
                } else if (stmt_type == "new") {
                    auto new_s = std::static_pointer_cast<NewStatement>(*it);
                    try_assign_type(found_type, new_s->target_type());
                } else if (stmt_type == "get") {
                    auto get_s = std::static_pointer_cast<GetStatement>(*it);
                    get_s->apply_indices(structs);
                    if (get_s->target_type() != UNKNOWN_TYPE) {
                        try_assign_type(found_type, get_s->target_type());
                    }
                } else if (stmt_type == "icmp") {
                    auto comp_s = std::static_pointer_cast<CompareStatement>(*it);
                    if (comp_s->target_name() == var_name) {
                        try_assign_type(found_type, "i1");
                    } else {
                        // only valid comparison is between ints.
                        try_assign_type(found_type, "i32");
                    }
                }
            }
        }
        return found_type;
    }
public:
    FnTypeResolver(std::string fname, std::vector<Operand> header) : fname(fname), header(header), ctx(std::make_shared<llvm::LLVMContext>()) {};

    void add_stmt(std::shared_ptr<Statement> stmt) {
        stmts.push_back(stmt);
    }

    void add_fn(std::string name, std::vector<std::string> header) {
        global_fns.insert({name, header});
    }

    void add_struct(std::string name, std::vector<std::string> fields) {
        if (fields.size() % 2 != 0) return;
        
        std::vector<Operand> typed_fields;
        for (auto it = fields.begin(); it != fields.end(); it += 2) {
            typed_fields.push_back(Operand(*(it+1), *it));
        }
        structs.insert({name, typed_fields});
    }

    std::map<std::string, std::string> get_types() {
        std::map<std::string, std::string> m; // mapping
        // std::set<std::string> untyped; // set of untyped vars

        for (auto it = header.begin(); it != header.end(); ++it) {
            m.insert({it->varname(), it->type_str()});
        }

        for (auto it = stmts.begin(); it != stmts.end(); ++it) {
            auto vs = (*it)->vars();

            for (auto v_it = vs.begin(); v_it != vs.end(); ++v_it) {
                auto type = _infer(*v_it, m);
                m.insert({*v_it, type});
                // std::cout << *v_it << "=" << type << std::endl;
            }
        }

        return m;
    }

    void perform_pass(bool print=false) {
        std::map<std::string, std::string> typemap = get_types();
        // auto typemap = this->get_types();
        for (auto it = stmts.begin(); it != stmts.end(); ++it) {
            for (auto it = stmts.begin(); it != stmts.end(); ++it) {
                (*it)->apply_types(typemap);
                typemap = get_types();
            }
        }
        if (print) {
            std::cout << "\ninferred types:\n";
            for (auto it = typemap.begin(); it != typemap.end(); ++it) {
                std::cout << it->first << " = " << it->second << std::endl;
            }
        }
    }

    void emit_stdout() {
        std::cout << "\nllvm:\n";
        for (auto it = stmts.begin(); it != stmts.end(); ++it) {
            std::cout << (*it)->llvm() << std::endl;
        }
    }

    std::string emit_str() {
        std::ostringstream oss;
        oss << fn_headers();
        oss << "define " << header.at(0).type_str() << " @" << fname << "(";
        for (auto it = header.begin() + 1; it != header.end(); ++it) {
            oss << it->llvm();
            if (it + 1 != header.end())
                oss << ", ";
        }
        oss << ")\n{\n";
        for (auto it = stmts.begin(); it != stmts.end(); ++it) {
            if ((*it)->typestr() != "bl") oss << "\t";
            oss << (*it)->llvm() << "\n";
        }
        oss << "}";
        return oss.str();
    }

    std::string fn_headers() {
        std::ostringstream oss;
        for (auto fn = global_fns.begin(); fn != global_fns.end(); ++fn) {
            oss << "declare " << fn->second.at(0) << " @" << fn->first << "(";
            for (auto it = fn->second.begin() + 1; it != fn->second.end(); ++it) {
                oss << *it;
                if (it + 1 != fn->second.end())
                    oss << ", ";
            }
            oss << ")\n";
        }
        return oss.str();
    }

    orc::ThreadSafeModule get_module() {
        orc::ThreadSafeContext tsc(std::make_unique<LLVMContext>());
        SMDiagnostic err;
        auto m = parseAssemblyString(emit_str(), err, *tsc.getContext());
        if (!m) {
            // std::cerr << "\nParse unsuccessful.\n";
            std::cerr << err.getLineContents().str() << std::endl;
            std::cerr << err.getMessage().str() << std::endl;
            return orc::ThreadSafeModule();
        } else {
            // std::cout << "\nParse successful.\n";
            return orc::ThreadSafeModule(std::move(m), std::move(tsc));
        }
        
    }
};

}

#endif
