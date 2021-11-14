#include "llvm/IR/Module.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/AsmParser/Parser.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/ExecutionEngine/Orc/Core.h"
#include "llvm/ExecutionEngine/Orc/LLJIT.h"
// #include "LLVMContextImpl.h"

#include "MiniCompiler.h"
#include "MiniFrontend.h"
#include "MiniInterpreter.h"
#include "MiniJIT.h"

#include <cstdint>
#include <iostream>
#include <regex>
#include <functional>

using namespace llvm;

namespace jited {

extern "C" void printInt(int x) {
    std::cout << x;
}

extern "C" void printIntEndl(int x) {
    std::cout << x << std::endl;
}

extern "C" int readInt() {
    int x;
    std::cin >> x;
    return x;
}

bool defaultHeatFunction(std::string called, std::string lastCalled, std::map<std::string, int> callCounts) {
    return true;
}

void ModuleCompiler::dontLogErrors(llvm::Error Err) {};

ModuleCompiler::ModuleCompiler(std::unique_ptr<orc::LLJIT> jit, orc::MangleAndInterner mangler) : jit(std::move(jit)), mangler(mangler) {
    loadCommon();
}

std::unique_ptr<ModuleCompiler> ModuleCompiler::create() {
    auto jit = orc::LLJITBuilder().create();
    orc::MangleAndInterner mangler((*jit)->getExecutionSession(), (*jit)->getDataLayout());

    if (!jit)
        return nullptr;
        // return jit.takeError();
    
    (*jit)->getExecutionSession().setErrorReporter(dontLogErrors);

    return std::make_unique<ModuleCompiler>(std::move(*jit), mangler);
}

void ModuleCompiler::loadCommon() {
    auto DG = orc::DynamicLibrarySearchGenerator::GetForCurrentProcess(
        jit->getDataLayout().getGlobalPrefix(),
        [&](const orc::SymbolStringPtr &S) {
            DenseSet<orc::SymbolStringPtr> allowed({
                mangler("printf"),
                mangler("malloc"),
                mangler("free")
            }); 
            return allowed.count(S);
        }
    );

    auto &jd = jit->getMainJITDylib();
    jd.addGenerator(std::move(DG.get()));
    auto s = orc::absoluteSymbols({
        {
            mangler("readInt"),
            JITEvaluatedSymbol(pointerToJITTargetAddress(&readInt), JITSymbolFlags::Exported)
        },
        {
            mangler("printInt"),
            JITEvaluatedSymbol(pointerToJITTargetAddress(&printInt), JITSymbolFlags::Exported)
        },
        {
            mangler("printIntEndl"),
            JITEvaluatedSymbol(pointerToJITTargetAddress(&printIntEndl), JITSymbolFlags::Exported)
        }
    });
    Error e = jd.define(s);
    if (e) {
        errs() << e;
        return;
    }
}

int ModuleCompiler::addFile(char* fname) {
    orc::ThreadSafeContext tsc(std::make_unique<LLVMContext>());
    SMDiagnostic err;
    auto m = parseIRFile(fname, err, *tsc.getContext());
    if (!m) {
        std::cout << "<" << fname << "> ";
        std::cout << err.getMessage().str() << std::endl;
        return -1;
    }

    orc::ThreadSafeModule tsm(std::move(m), std::move(tsc));

    if (auto err = this->jit->addIRModule(std::move(tsm))) {
        errs() << err;
        return 1;
    }
    return 0;
}

int ModuleCompiler::addString(std::string input) {
    orc::ThreadSafeContext tsc(std::make_unique<LLVMContext>());
    SMDiagnostic err;
    auto m = parseAssemblyString(input, err, *tsc.getContext());
    if (!m) {
        std::cout << err.getMessage().str() << std::endl;
        return -1;
    }
    
    orc::ThreadSafeModule tsm(std::move(m), std::move(tsc));

    // StringMap<StructType*> st = tsm.getContext().getContext()->pImpl->NamedStructTypes;
    // StructType* stp = StructType::getTypeByName(tsm.getContext(), "struct.union_t");

    if (auto err = this->jit->addIRModule(std::move(tsm))) {
        errs() << err;
        throw std::runtime_error("incomplete compilation");
    }
    return 0;
}

void* ModuleCompiler::getSym(std::string name) {
    auto res = jit->lookup(name);
    if (auto e = res.takeError()) {
        // errs() << e;
        return NULL;
    } else {
        // std::cout << typeid(res->getAddress()).name() << std::endl;
        return (void*) res->getAddress();
    }
}

JIT::JIT(jited::ast::ProgramPtr program, std::shared_ptr<jited::ast::ASTVisitor> compiler) :
    jited::MiniInterpreter(program),
    mc(std::move(jited::ModuleCompiler::create())),
    compiler(compiler),
    heatFunction(defaultHeatFunction)
{
    makeGlobals();
    for (auto pair : funcs) {
        callCounts[pair.first] = 0;
    }
};

JIT::~JIT() {};

// WE PROBABLY NEED A LOOKUP/EVALUATEDSYMBOL FOR STRUCTS
antlrcpp::Any JIT::visit(jited::ast::InvocationExpression* expression) {
    // branch based on whether it's compiled already or not
    if (mc->getSym(expression->name)) {
        // int8_t* args = new int8_t[expression->arguments.size()];
        #ifdef DEBUGJIT
        std::cout << "running jitted symbol...\n";
        #endif
        
        std::vector<jited::TypedValue> vals;
        int totalSize = 0;
        for (jited::ast::ExpressionPtr e : expression->arguments) {
            jited::TypedValue tv = e->accept(this);
            if (tv.isStruct()) {
                totalSize += sizeof(intptr_t) / sizeof(int32_t);
            } else {
                totalSize += 1;
            }
            vals.push_back(tv);
        }
        // std::cout << totalSize << std::endl;
        int32_t* args = new int32_t[totalSize]();
        // int8_t* args = new int8_t[totalBytes]();
        int currentOffset = 0;
        for (jited::TypedValue tv : vals) {
            if (tv.type == jited::ast::IntType::name() || tv.type == jited::ast::BoolType::name()) {
                int32_t i32 = tv.asI32();
                args[currentOffset] = i32;
                currentOffset += 1;
            } else {
                jited::PackedStruct* ps = tv.asStruct();
                *((uint8_t**) (args + currentOffset)) = ps->buf;
                currentOffset += sizeof(intptr_t) / sizeof(int32_t);
            }
        }
        
        std::string returnType = funcs.at(expression->name)->retType->toMiniString();
        antlrcpp::Any retVal;
        if (returnType == jited::ast::IntType::name()) {
            auto fptr = (int32_t (*)(int32_t*)) mc->getSym(expression->name + "ENTRY");
            int rawRetVal = fptr(args);
            retVal = jited::TypedValue(returnType, rawRetVal);
        } else if (returnType == jited::ast::BoolType::name()) {
            auto fptr = (int32_t (*)(int32_t*)) mc->getSym(expression->name + "ENTRY");
            bool rawRetVal = fptr(args);
            retVal = jited::TypedValue(returnType, rawRetVal);
        } else if (returnType == jited::ast::VoidType::name()) {
            auto fptr = (void (*)(int32_t*)) mc->getSym(expression->name + "ENTRY");
            fptr(args);
            retVal = nullptr;
        } else {
            auto fptr = (uint8_t* (*)(int32_t*)) mc->getSym(expression->name + "ENTRY");
            uint8_t* rawRetVal = fptr(args);
            retVal = jited::TypedValue(returnType, reverseLookup(rawRetVal, returnType));
        }
        delete[] args;


        return retVal;
    } else {
        antlrcpp::Any rv = MiniInterpreter::visit(expression);
        
        if (heatFunction(expression->name, lastCalled, callCounts)) {
            compileFunction(expression->name);
        }

        callCounts[expression->name] += 1;
        lastCalled = expression->name;
        return rv;
    }
}

jited::PackedStruct* JIT::reverseLookup(uint8_t* buf, std::string structName) {
    if (jited::PackedStruct::lookupTable.count(buf)) {
        return jited::PackedStruct::lookupTable.at(buf);
    } else {
        if (structs.count(structName)) {
            return new jited::PackedStruct(structs.at(structName), buf);
        } else {
            throw std::runtime_error("struct name lookup failed in reverseLookup");
        }
        // return new PackedStruct();
    }
}

// contains the LLVM for defining necessary structs
std::string JIT::structsString() {
    std::ostringstream oss;

    for (auto pair : structs) {
        jited::ast::TypeDeclarationPtr typeDec = pair.second;
        oss << "%struct." << typeDec->name << " = type <{";
        std::string typeList = "";
        for (jited::ast::DeclarationPtr field : typeDec->fields) {
            typeList += field->type->toString() + ", ";
        }
        if (typeList.length() > 0) {
            oss << typeList.substr(0, typeList.length() - 2);
        }
        oss << "}>\n";
    }
    return oss.str();
}

std::string JIT::globalString() {
    std::ostringstream oss;
    for (jited::ast::DeclarationPtr dec : program->decls) {
        if (funcs.count(dec->name) == 0) {
            oss << "@" << dec->name << " = external dso_local global " << dec->type->toString() << ", align 4\n";
        }
    }
    return oss.str();
}

// contains the declarations of external functions required
// this may need to take a function, as well? need access to all functions
// that may be possible to fudge using error handlers tho
std::string JIT::declareString(std::string fname) {
    std::ostringstream oss;
    oss << "declare i8* @malloc(i32)\n"
        << "declare void @free(i8*)\n"
        << "declare void @printInt(i32)\n"
        << "declare void @printIntEndl(i32)\n"
        << "declare i32 @readInt()\n";

    for (auto pair : funcs) {
        jited::ast::FunctionPtr f = pair.second;
        if (f->name != fname) {
            oss << "declare " << f->retType->toString() << " @" << f->name << "(";
            std::string paramList = "";
            for (jited::ast::DeclarationPtr param : f->params) {
                paramList += param->type->toString() + " %" + param->name + ", ";
            }
            if (f->params.size() > 0) {
                oss << paramList.substr(0, paramList.length() - 2) << ")\n";
            } else {
                oss << ")\n";
            }
        }
    }

    return oss.str();
}

std::string JIT::moduleString(std::string fname) {
    std::ostringstream oss;
    // har har effin string
    std::string fnString = funcs.at(fname)->accept(compiler.get());


    oss << structsString();
    oss << globalString();
    oss << entryFunction(fname);
    oss << fnString;
    oss << declareString(fname);

    return oss.str();
}

std::string JIT::entryFunction(std::string name) {
    if (funcs.count(name)) {
        std::string argsName = "%argsarray";
        int ctr = 0;
        int i = 0;
        int ptrStep = sizeof(intptr_t) / sizeof(int32_t);
        std::ostringstream oss;
        jited::ast::FunctionPtr function = funcs.at(name);
        oss << "define " + function->retType->toString() + " @" + function->name + "ENTRY(i32* " + argsName + ") {\nL0:\n";
        for (jited::ast::DeclarationPtr param : function->params) {
            oss << "%" << param->name << "PTR = getelementptr i32, i32* " << argsName << ", i64 " << i << "\n";
            if (param->type->toString() == "i32") {
                // dereference the i32
                oss << "%" << param->name << " = load i32, i32* %" << param->name << "PTR\n";
                i += 1;
            } else {
                // bitcast it to a struct pointer
                oss << "%" << param->name << "PTR2 = bitcast i32* %" << param->name << "PTR to " << param->type->toString() << "*\n"
                    << "%" << param->name << " = load " << param->type->toString() << ", " << param->type->toString() << "* %"
                    << param->name << "PTR2\n";
                // oss << " = bitcast i32* %" << param->name << "PTR to " << "i32*" << "\n";
                i += ptrStep;
            }
        }

        std::string paramList = "(";
        for (jited::ast::DeclarationPtr param : function->params) {
            paramList += param->type->toString() + " %" + param->name + ", ";
        }
        if (function->params.size() > 0) {
            paramList = paramList.substr(0, paramList.length() - 2) + ")";
        } else {
            paramList += ")";
        }
        
        if (function->retType->toString() != "void") {
            oss << "%_retval_ = call " << function->retType->toString() + " @" + function->name << paramList << "\n"
                << "ret " << function->retType->toString() << " %_retval_\n";
        } else {
            oss << "call " << function->retType->toString() + " @" + function->name << paramList << "\n"
                << "ret void\n";
        }
        
        oss << "}\n";
        return oss.str();
    } else {
        return "";
    }
}

void JIT::compileFunction(std::string fname) {
    if (funcs.count(fname)) {
        if (!mc->getSym(fname)) {
            jited::ast::FunctionPtr f = funcs.at(fname);
            std::string moduleStr = moduleString(fname);
            // #ifdef DEBUGJIT
            // std::cout << moduleStr << std::endl;
            // #endif
            mc->addString(moduleStr);
        }
    } else {
        throw std::runtime_error("compilation requested for nonexistent function");
    }
}

void JIT::makeGlobals() {
    auto &jd = mc->jit->getMainJITDylib();
    for (auto pair : globals) {
        auto mangled = mc->mangler(pair.first);
        jited::TypedValuePtr tv = pair.second;
        if (tv->value.is<jited::IntPtr>()) {
            int* ip = tv->value.as<jited::IntPtr>().get();
            Error e = jd.define(orc::absoluteSymbols({{
                mangled,
                JITEvaluatedSymbol(pointerToJITTargetAddress(ip), JITSymbolFlags::Exported)
            }}));
            if (e) {
                errs() << e;
                throw std::runtime_error("error in global definition");
            }
        } else if (tv->value.is<jited::BoolPtr>()) {
            bool* bp = tv->value.as<jited::BoolPtr>().get();
            Error e = jd.define(orc::absoluteSymbols({{
                mangled,
                JITEvaluatedSymbol(pointerToJITTargetAddress(bp), JITSymbolFlags::Exported)
            }}));
            if (e) {
                errs() << e;
                throw std::runtime_error("error in global definition");
            }
        } else {
            jited::PackedStruct* ps = tv->value.as<jited::PackedStruct*>();
            Error e = jd.define(orc::absoluteSymbols({{
                mangled,
                JITEvaluatedSymbol(pointerToJITTargetAddress(ps), JITSymbolFlags::Exported)
            }}));
            if (e) {
                errs() << e;
                throw std::runtime_error("error in global definition");
            }
        }
    }
}

void JIT::setHeatFunction(std::function<bool(std::string, std::string, std::map<std::string, int>)> newHeatFn) {
    heatFunction = newHeatFn;
}

void JIT::initialize() {
    InitializeAllTargetInfos();
    InitializeAllTargets();
    InitializeAllTargetMCs();
    InitializeAllAsmPrinters();
    return;
}

}

