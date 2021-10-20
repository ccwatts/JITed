#include "lib/llasm.h"
#include "lib/program.h"
#include "lib/userjit.h"
// #include "lib/micro.h"
#include <iostream>
#include "llvm/IR/Module.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/AsmParser/Parser.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/ExecutionEngine/Orc/Core.h"
#include "llvm/ExecutionEngine/Orc/LLJIT.h"
#include "inputs/generic.h"
using namespace llvm;
std::string INP = 
    "%struct.IntHolder = type {i32}\n"
    "define void @divideBy8(%struct.IntHolder* %num)\n"
    "{\n"
    "%u16 = getelementptr %struct.IntHolder, %struct.IntHolder* %num, i1 0, i32 0\n"
	"%u17 = load i32, i32* %u16\n"
	"%u18 = sdiv i32 %u17, 2\n"
	"%u19 = getelementptr %struct.IntHolder, %struct.IntHolder* %num, i1 0, i32 0\n"
	"store i32 %u18, i32* %u19\n"
	"%u20 = getelementptr %struct.IntHolder, %struct.IntHolder* %num, i1 0, i32 0\n"
	"%u21 = load i32, i32* %u20\n"
	"%u22 = sdiv i32 %u21, 2\n"
	"%u23 = getelementptr %struct.IntHolder, %struct.IntHolder* %num, i1 0, i32 0\n"
	"store i32 %u22, i32* %u23\n"
	"%u24 = getelementptr %struct.IntHolder, %struct.IntHolder* %num, i1 0, i32 0\n"
	"%u25 = load i32, i32* %u24\n"
	"%u26 = sdiv i32 %u25, 2\n"
	"%u27 = getelementptr %struct.IntHolder, %struct.IntHolder* %num, i1 0, i32 0\n"
	"store i32 %u26, i32* %u27\n"
	"br label %LU5\n"
    "LU5:\n"
    "ret void\n"
    "}\n";

int main() {
    // auto vec = tokenize("HELLO world HOW are you  ?");
    // for (auto it = vec.begin(); it != vec.end(); ++it) {
    //     std::cout << *it << std::endl;
    // }
    // line_to_llvm("x = add y x");
    // std::cout << std::endl;
    orc::ThreadSafeContext tsc(std::make_unique<LLVMContext>());
    SMDiagnostic err;
    auto m = parseAssemblyString(INP, err, *tsc.getContext());
    if (!m) {
        std::cout << err.getLineContents().str() << std::endl;
        std::cout << err.getMessage().str() << std::endl;
        return -1;
    } else {
        std::cout << "Successfully parsed function.\n";
    }
    return 0;
}
