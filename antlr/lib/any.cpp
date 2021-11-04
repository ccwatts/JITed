// necessary to fix some weird bug with antlrcpp segfaulting on function exit because of Any
// might have something to do with the use of shared pointers inside Any
#include "antlr4-runtime.h"
antlrcpp::Any::~Any() {};